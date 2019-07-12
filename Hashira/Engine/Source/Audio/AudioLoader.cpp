
#include "AudioLoader.h"
#include "AudioWaveSource.h"
#include "AudioSourceManager.h"
#include "Engine/Source/Math/Math.h"
#include "Engine/Source/Utility/Utility.h"
#include "Engine/Source/Debug/Logger.h"

//���[�h���ԑ���p
#include <Windows.h>
#include <ks.h>
#include <ksmedia.h>
#include <iostream>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <mmiscapi.h>


//���̃N���X�Ŏg��thread�̐�
constexpr unsigned int AUDIO_LOADER_THREAD_NUM = 12;
constexpr float AUDIO_8BIT_NORMALIZE_FACTOR = 127.0f;
constexpr float AUDIO_16BIT_NORMALIZE_FACTOR = 32768.0f;
constexpr float AUDIO_24BIT_NORMALIZE_FACTOR = 0xFFFF;
constexpr float AUDIO_32BIT_NORMALIZE_FACTOR = 1.0f;


namespace Hashira {

	enum AUDIO_BITS_PER_SAMPLE {
		AUDIO_BITS_PER_SAMPLE_8,	//128�𖳉��Ƃ���0�`255�̒l���g�p����
		AUDIO_BITS_PER_SAMPLE_16,	//0�𖳉��Ƃ���-32768�`32767�̒l���g�p����
		AUDIO_BITS_PER_SAMPLE_24,
		AUDIO_BITS_PER_SAMPLE_32,
	};

	const DWORD SPEAKER_SETTINGS[] = {
		KSAUDIO_SPEAKER_MONO,
		KSAUDIO_SPEAKER_STEREO,
		KSAUDIO_SPEAKER_STEREO | SPEAKER_LOW_FREQUENCY,
		KSAUDIO_SPEAKER_QUAD,
		0,
		KSAUDIO_SPEAKER_5POINT1,
		0,
		KSAUDIO_SPEAKER_7POINT1_SURROUND
	};

	void LoadStereo(HMMIO mmio, AudioWaveSource & wave, MMCKINFO& dataChunk, unsigned int rawFormatSampleBit, unsigned int firstReadIndex);
	void LoadMonaural(HMMIO mmio, AudioWaveSource & wave, MMCKINFO& dataChunk, unsigned int rawFormatSampleBit, unsigned int firstReadIndex);

	void LoadStereo8bitSound(HMMIO mmio, AudioWaveSource & wave, MMCKINFO& dataChunk, unsigned int rawFormatSampleBit, unsigned int firstReadIndex = 0);
	void LoadStereo16bitSound(HMMIO mmio, AudioWaveSource & wave, MMCKINFO& dataChunk, unsigned int rawFormatSampleBit, unsigned int firstReadIndex = 0);
	void LoadStereo24bitSound(HMMIO mmio, AudioWaveSource & wave, MMCKINFO& dataChunk, unsigned int rawFormatSampleBit, unsigned int firstReadIndex = 0);
	void LoadStereo32bitSound(HMMIO mmio, AudioWaveSource & wave, MMCKINFO& dataChunk, unsigned int rawFormatSampleBit, unsigned int firstReadIndex = 0);
	void LoadMonaural8bitSound(HMMIO mmio, AudioWaveSource & wave, MMCKINFO& dataChunk, unsigned int rawFormatSampleBit, unsigned int firstReadIndex = 0);
	void LoadMonaural16bitSound(HMMIO mmio, AudioWaveSource & wave, MMCKINFO& dataChunk, unsigned int rawFormatSampleBit, unsigned int firstReadIndex = 0);
	void LoadMonaural24bitSound(HMMIO mmio, AudioWaveSource & wave, MMCKINFO& dataChunk, unsigned int rawFormatSampleBit, unsigned int firstReadIndex = 0);
	void LoadMonaural32bitSound(HMMIO mmio, AudioWaveSource & wave, MMCKINFO& dataChunk, unsigned int rawFormatSampleBit, unsigned int firstReadIndex = 0);
}


void Hashira::AudioLoader::StopLoad()
{
	_isStop = true;
}

void Hashira::AudioLoader::ReStartLoad()
{
	_isStop = false;
}

bool Hashira::AudioLoader::IsStopLoad()
{
	return _isStop;
}

void Hashira::AudioLoader::DiscardWorkerThreads()
{
	this->_loadThreadPool.DiscardWorkers();
}

std::shared_ptr<Hashira::AudioWaveSource> Hashira::AudioLoader::LoadAudio(std::string audioFilePath)
{
	if (AudioSourceManager::GetInstance().IsLoaded(audioFilePath)) {
		return AudioSourceManager::GetInstance().GetResource(audioFilePath).lock();
	}

	//�����16bit�����̂ݑΉ�

	HMMIO mmio{};

	MMIOINFO mmioInfo{};

	auto str = Util::StringToWString(audioFilePath);
	mmio = mmioOpen(&str[0], &mmioInfo, MMIO_READ);

	if (!mmio) {
		ERROR_LOG(std::string("mmio����I�[�f�B�I�t�@�C���̃��[�h�Ɏ��s���܂����B"));
	}

	MMRESULT ret{};

	//�`�����N���[�h
	MMCKINFO riffChunk;
	{
		riffChunk.fccType = mmioFOURCC('W', 'A', 'V', 'E');
		ret = mmioDescend(mmio, &riffChunk, NULL, MMIO_FINDRIFF);

		if (ret != MMSYSERR_NOERROR) {
			ERROR_LOG(std::string("mmioDescend�����s���܂����B(FIND_RIFF)"));
			mmioClose(mmio, 0);
			return nullptr;
		}
	}
	//�`�����N���[�h
	MMCKINFO chunk;

	{
		chunk.ckid = mmioFOURCC('f', 'm', 't', ' ');
		ret = mmioDescend(mmio, &chunk, NULL, MMIO_FINDCHUNK);

		if (ret != MMSYSERR_NOERROR) {
			ERROR_LOG(std::string("mmioDescend�����s���܂����B(FIND_CHUNK)"));
			mmioClose(mmio, 0);
			return nullptr;
		}
	}

	//�t�H�[�}�b�g���o
	WAVEFORMATEX format{};
	WAVEFORMATEXTENSIBLE exFormat{};

	{
		DWORD size = mmioRead(mmio, (HPSTR)&format, chunk.cksize);
		if (size != chunk.cksize) {
			ERROR_LOG(std::string("mmioRead�����s���܂����B(FormatRead)"));
			mmioClose(mmio, 0);
			return nullptr;

		}
		std::stringstream ss;

		ss << "Format = " << format.wFormatTag;
		DEBUG_LOG(std::string(ss.str()));
		ss.str("");
		ss << "Channel = " << format.nChannels;
		DEBUG_LOG(std::string(ss.str()));
		ss.str("");
		ss << "Sampling = " << format.nSamplesPerSec;
		DEBUG_LOG(std::string(ss.str()));
		ss.str("");
		ss << "BitPerSample = " << format.wBitsPerSample;
		DEBUG_LOG(std::string(ss.str()));
		ss.str("");
		ss << "BytePerSec = " << format.nAvgBytesPerSec;
		DEBUG_LOG(std::string(ss.str()));
		ss.str("");

	}
	//�K�w������
	mmioAscend(mmio, &chunk, 0);

	//�f�[�^�`�����N�̌���
	MMCKINFO dataChunk;
	{
		dataChunk.ckid = mmioFOURCC('d', 'a', 't', 'a');
		ret = mmioDescend(mmio, &dataChunk, &riffChunk, MMIO_FINDCHUNK);
		if (ret != MMSYSERR_NOERROR) {
			ERROR_LOG(std::string("mmioDescend�����s���܂����B(DataSerch)"));
			mmioClose(mmio, 0);
			return nullptr;
		}
	}

	std::shared_ptr<Hashira::AudioWaveSource> res = std::make_shared<Hashira::AudioWaveSource>();

	//�\�[�X�Z�b�g
	AudioSourceManager::GetInstance().SetResource(audioFilePath, res);
	//�t�H�[�}�b�g�Z�b�g
	{

		res->_format.Format = format;

		res->_format.Format.nSamplesPerSec; //1 �b������̃T���v���� (Hz)
		res->_format.Format.nChannels; // �E�F�[�u�t�H�[�� �I�[�f�B�I �f�[�^�̃`�����l����
		res->_format.Format.wFormatTag = WAVE_FORMAT_EXTENSIBLE; //�T�u�t�H�[�}�b�g�Œ�`
		res->_format.Format.wBitsPerSample = static_cast<unsigned short>(Hashira::Util::ConvertByteToBit<float>()); //1�T���v���_������̃r�b�g��
		res->_format.Format.nBlockAlign = sizeof(float) * format.nChannels; //1�T���v���_�̃f�[�^���r���ŕ�������Ȃ����߂̍ŏ��A���C����
		res->_format.Format.nAvgBytesPerSec = format.nSamplesPerSec * res->_format.Format.nBlockAlign; //�P�b������̃f�[�^�]����
		res->_format.Format.cbSize = sizeof(WAVEFORMATEXTENSIBLE) - sizeof(WAVEFORMATEX);

		//ExtendedFormat�̐ݒ�
		res->_format.Samples.wValidBitsPerSample = res->_format.Format.wBitsPerSample;
		res->_format.dwChannelMask = SPEAKER_SETTINGS[format.nChannels - 1];	//�X�s�[�J�[�z�u
		res->_format.SubFormat = KSDATAFORMAT_SUBTYPE_IEEE_FLOAT;
	}

	//�z�񐔂Ɠǂݍ��݉񐔂̐ݒ�
	{
		res->GetWave().resize(dataChunk.cksize / (Util::ConvertBitToByte(format.wBitsPerSample)));
		res->_loadingInfo.cycleMax = static_cast<unsigned int>(res->GetWave().size() / res->GetWaveFormat().nChannels);
	}

	//�q�̔g�`�f�[�^���o�������}���`�X���b�h��������
	{
		//�g�`�f�[�^���o
		{
			Stereo16Bit waveInfo = {};
			std::chrono::system_clock::time_point start, end;
			start = std::chrono::system_clock::now();

			do {

				res->_loadingInfo.loadedSize += mmioRead(mmio, (HPSTR)&waveInfo, sizeof(Stereo16Bit));
				if (res->_loadingInfo.loadedSize != sizeof(Stereo16Bit)*(++res->_loadingInfo.cycleNum)) {
					ERROR_LOG(std::string("mmioRead�����s���܂����B(WaveLoad)"));
					mmioClose(mmio, 0);
					return nullptr;
				}
				//�g�`�@���K��v����ё��
				{
					res->GetWave()[res->_loadingInfo.loadedIndex] = (static_cast<float>(waveInfo.left) / AUDIO_16BIT_NORMALIZE_FACTOR);
					res->GetWave()[res->_loadingInfo.loadedIndex + 1] = (static_cast<float>(waveInfo.right) / AUDIO_16BIT_NORMALIZE_FACTOR);
				}
				//�C���N�������g
				{
					res->_loadingInfo.loadedIndex += static_cast<unsigned int>(res->GetWaveFormat().nChannels);
				}

			} while (res->_loadingInfo.cycleMax > res->_loadingInfo.cycleNum);

			end = std::chrono::system_clock::now();
			auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
			std::stringstream ss;
			ss << "Read time [ " << elapsed << " ] ms.";
			INFO_LOG(std::string(ss.str()));

		}
	}

	//�n���h���N���[�Y
	mmioClose(mmio, 0);

	////���f�[�^�\�z

	return res;
}

std::shared_ptr<Hashira::AudioWaveSource> Hashira::AudioLoader::LoadAudioEx(std::string audioFilePath)
{
	if (AudioSourceManager::GetInstance().IsLoaded(audioFilePath)) {
		return AudioSourceManager::GetInstance().GetResource(audioFilePath).lock();
	}

	//�����16bit�����̂ݑΉ�

	HMMIO mmio{};

	MMIOINFO mmioInfo{};

	auto str = Util::StringToWString(audioFilePath);
	mmio = mmioOpen(&str[0], &mmioInfo, MMIO_READ);

	if (!mmio) {
		ERROR_LOG(std::string("mmio����I�[�f�B�I�t�@�C���̃��[�h�Ɏ��s���܂����B"));
	}

	MMRESULT ret{};

	//�`�����N���[�h
	MMCKINFO riffChunk;
	{
		riffChunk.fccType = mmioFOURCC('W', 'A', 'V', 'E');
		ret = mmioDescend(mmio, &riffChunk, NULL, MMIO_FINDRIFF);

		if (ret != MMSYSERR_NOERROR) {
			ERROR_LOG(std::string("mmioDescend�����s���܂����B(FIND_RIFF)"));
			mmioClose(mmio, 0);
			return nullptr;
		}
	}

	//�`�����N���[�h
	MMCKINFO chunk;

	{
		chunk.ckid = mmioFOURCC('f', 'm', 't', ' ');
		ret = mmioDescend(mmio, &chunk, NULL, MMIO_FINDCHUNK);

		if (ret != MMSYSERR_NOERROR) {
			ERROR_LOG(std::string("mmioDescend�����s���܂����B(FIND_CHUNK)"));
			mmioClose(mmio, 0);
			return nullptr;
		}
	}

	//�t�H�[�}�b�g���o
	WAVEFORMATEX format{};
	WAVEFORMATEXTENSIBLE exFormat{};

	{
		DWORD size = mmioRead(mmio, (HPSTR)&format, chunk.cksize);
		if (size != chunk.cksize) {
			ERROR_LOG(std::string("mmioRead�����s���܂����B(FormatRead)"));
			mmioClose(mmio, 0);
			return nullptr;

		}
#ifdef _DEBUG
		std::stringstream ss;

		ss << "Format = " << format.wFormatTag;
		DEBUG_LOG(std::string(ss.str()));
		ss.str("");
		ss << "Channel = " << format.nChannels;
		DEBUG_LOG(std::string(ss.str()));
		ss.str("");
		ss << "Sampling = " << format.nSamplesPerSec;
		DEBUG_LOG(std::string(ss.str()));
		ss.str("");
		ss << "BitPerSample = " << format.wBitsPerSample;
		DEBUG_LOG(std::string(ss.str()));
		ss.str("");
		ss << "BytePerSec = " << format.nAvgBytesPerSec;
		DEBUG_LOG(std::string(ss.str()));
		ss.str("");
#endif
	}
	//�K�w������
	mmioAscend(mmio, &chunk, 0);

	//�f�[�^�`�����N�̌���
	MMCKINFO dataChunk;
	{
		dataChunk.ckid = mmioFOURCC('d', 'a', 't', 'a');
		ret = mmioDescend(mmio, &dataChunk, &riffChunk, MMIO_FINDCHUNK);
		if (ret != MMSYSERR_NOERROR) {
			ERROR_LOG(std::string("mmioDescend�����s���܂����B(DataSerch)"));
			mmioClose(mmio, 0);
			return nullptr;
		}
	}

	std::shared_ptr<Hashira::AudioWaveSource> res = std::make_shared<Hashira::AudioWaveSource>();

	//�\�[�X�Z�b�g
	AudioSourceManager::GetInstance().SetResource(audioFilePath, res);

	//�t�H�[�}�b�g�Z�b�g
	{

		res->_format.Format = format;

		//��{�t�H�[�}�b�g�̐ݒ�
		res->_format.Format.nSamplesPerSec; //1 �b������̃T���v���� (Hz)
		res->_format.Format.nChannels; // �E�F�[�u�t�H�[�� �I�[�f�B�I �f�[�^�̃`�����l����
		res->_format.Format.wFormatTag = WAVE_FORMAT_EXTENSIBLE; //�T�u�t�H�[�}�b�g�Œ�`
		res->_format.Format.wBitsPerSample = static_cast<unsigned short>(Hashira::Util::ConvertByteToBit<float>()); //1�T���v���_������̃r�b�g��
		res->_format.Format.nBlockAlign = sizeof(float) * format.nChannels; //1�T���v���_�̃f�[�^���r���ŕ�������Ȃ����߂̍ŏ��A���C����
		res->_format.Format.nAvgBytesPerSec = format.nSamplesPerSec * res->_format.Format.nBlockAlign; //�P�b������̃f�[�^�]����
		res->_format.Format.cbSize = sizeof(WAVEFORMATEXTENSIBLE) - sizeof(WAVEFORMATEX);

		//ExtendedFormat�̐ݒ�
		res->_format.Samples.wValidBitsPerSample = res->_format.Format.wBitsPerSample;
		res->_format.dwChannelMask = SPEAKER_SETTINGS[format.nChannels - 1];	//�X�s�[�J�[�z�u
		res->_format.SubFormat = KSDATAFORMAT_SUBTYPE_IEEE_FLOAT;
	}

	//�z�񐔂Ɠǂݍ��݉񐔂̐ݒ�
	{
		res->GetWave().resize(dataChunk.cksize / (Util::ConvertBitToByte(format.wBitsPerSample)));
		res->_loadingInfo.cycleMax =static_cast<unsigned int>(res->GetWave().size() / res->GetWaveFormat().nChannels);
	}

	//�g�`�f�[�^���o
	{
		switch (format.nChannels)
		{
		case 1:
			this->_loadThreadPool.enqueue([=]() {
				auto chunk = dataChunk;
				LoadMonaural(mmio, *res, chunk, format.wBitsPerSample, 0);
			}
			);
			break;
		case 2:
			this->_loadThreadPool.enqueue([=]() {
				auto chunk = dataChunk;
				LoadStereo(mmio, *res, chunk, format.wBitsPerSample, 0);
			}
			);
			break;
		default:
			break;
		}
	}

	//���A
	return res;
}

void Hashira::LoadStereo(HMMIO mmio, AudioWaveSource & wave, MMCKINFO & dataChunk, unsigned int rawFormatSampleBit, unsigned int firstReadIndex)
{
	switch (rawFormatSampleBit)
	{
	case 8:
		LoadStereo8bitSound(mmio, wave, dataChunk, rawFormatSampleBit, firstReadIndex);
		break;
	case 16:
		LoadStereo16bitSound(mmio, wave, dataChunk, rawFormatSampleBit, firstReadIndex);
		break;
	case 24:
		DEBUG_LOG(std::string("������"));
		assert(0);
		LoadStereo24bitSound(mmio, wave, dataChunk, rawFormatSampleBit, firstReadIndex);
		break;
	case 32:
		DEBUG_LOG(std::string("������"));
		assert(0);
		LoadStereo32bitSound(mmio, wave, dataChunk, rawFormatSampleBit, firstReadIndex);
		break;
	default:
		assert(0);
		break;
	}
}

void Hashira::LoadMonaural(HMMIO mmio, AudioWaveSource & wave, MMCKINFO & dataChunk, unsigned int rawFormatSampleBit, unsigned int firstReadIndex)
{
	switch (rawFormatSampleBit)
	{
	case 8:
		LoadMonaural8bitSound(mmio, wave, dataChunk, rawFormatSampleBit, firstReadIndex);
		break;
	case 16:
		LoadMonaural16bitSound(mmio, wave, dataChunk, rawFormatSampleBit, firstReadIndex);
		break;
	case 24:
		DEBUG_LOG(std::string("������"));
		assert(0);
		LoadMonaural24bitSound(mmio, wave, dataChunk, rawFormatSampleBit, firstReadIndex);
		break;
	case 32:
		DEBUG_LOG(std::string("������"));
		assert(0);
		LoadMonaural32bitSound(mmio, wave, dataChunk, rawFormatSampleBit, firstReadIndex);
		break;
	default:
		assert(0);
		break;
	}
}

void Hashira::LoadStereo8bitSound(HMMIO mmio, AudioWaveSource & wave, MMCKINFO & dataChunk, unsigned int rawFormatSampleBit, unsigned int firstReadIndex)
{

	Stereo8Bit waveInfo{}; //�ǂݍ��񂾔g�`�̏��		

	do {

		wave._loadingInfo.loadedSize += mmioRead(mmio, (HPSTR)&waveInfo, sizeof(Stereo16Bit));
		if (wave._loadingInfo.loadedSize != sizeof(Stereo8Bit)*(++wave._loadingInfo.cycleNum)) {
			ERROR_LOG(std::string("mmioRead�����s���܂����B(WaveLoad)"));
			mmioClose(mmio, 0);
			return;
		}
		//�g�`�@���K��v����ё��
		{
			wave.GetWave()[wave._loadingInfo.loadedIndex] = (static_cast<float>(waveInfo.left) / AUDIO_8BIT_NORMALIZE_FACTOR) - 1.0f;
			wave.GetWave()[wave._loadingInfo.loadedIndex + 1] = (static_cast<float>(waveInfo.right) / AUDIO_8BIT_NORMALIZE_FACTOR) - 1.0f;
		}
		//�C���N�������g
		{
			wave._loadingInfo.loadedIndex += static_cast<unsigned int>(wave.GetWaveFormat().nChannels);
		}
	} while (wave._loadingInfo.cycleMax > wave._loadingInfo.cycleNum);

	wave._loadingInfo.isWaveLoaded = true;
	//�n���h���N���[�Y
	mmioClose(mmio, 0);

}

void Hashira::LoadStereo16bitSound(HMMIO mmio, AudioWaveSource & wave, MMCKINFO & dataChunk, unsigned int rawFormatSampleBit, unsigned int firstReadIndex)
{

	Stereo16Bit waveInfo{}; //�ǂݍ��񂾔g�`�̏��		
	do {
		if (AudioLoader::GetInstance().IsStopLoad()) {
			ERROR_LOG(std::string("�g�`�f�[�^�̓ǂݍ��݂𒆒f���܂��B(WaveLoad)"));
			mmioClose(mmio, 0);
			return;
		}
		wave._loadingInfo.loadedSize += mmioRead(mmio, (HPSTR)&waveInfo, sizeof(Stereo16Bit));
		if (wave._loadingInfo.loadedSize != sizeof(Stereo16Bit)*(++wave._loadingInfo.cycleNum)) {

		}
		//�g�`�@���K��v����ё��
		{
			wave.GetWave()[wave._loadingInfo.loadedIndex] = (static_cast<float>(waveInfo.left) / AUDIO_16BIT_NORMALIZE_FACTOR);
			wave.GetWave()[wave._loadingInfo.loadedIndex + 1] = (static_cast<float>(waveInfo.right) / AUDIO_16BIT_NORMALIZE_FACTOR);
		}
		//�C���N�������g
		{
			wave._loadingInfo.loadedIndex += static_cast<unsigned int>(wave.GetWaveFormat().nChannels);
		}
	} while (wave._loadingInfo.cycleMax > wave._loadingInfo.cycleNum);

	wave._loadingInfo.isWaveLoaded = true;
	//�n���h���N���[�Y
	mmioClose(mmio, 0);

}

void Hashira::LoadStereo24bitSound(HMMIO mmio, AudioWaveSource & wave, MMCKINFO & dataChunk, unsigned int rawFormatSampleBit, unsigned int firstReadIndex)
{

	Stereo16Bit waveInfo{}; //�ǂݍ��񂾔g�`�̏��		
	do {
		wave._loadingInfo.loadedSize += mmioRead(mmio, (HPSTR)&waveInfo, sizeof(Stereo16Bit));
		if (wave._loadingInfo.loadedSize != sizeof(Stereo16Bit)*(++wave._loadingInfo.cycleNum)) {
			ERROR_LOG(std::string("mmioRead�����s���܂����B(WaveLoad)"));
			mmioClose(mmio, 0);
			return;
		}
		//�g�`�@���K��v����ё��
		{
			wave.GetWave()[wave._loadingInfo.loadedIndex] = (static_cast<float>(waveInfo.left) / AUDIO_16BIT_NORMALIZE_FACTOR);
			wave.GetWave()[wave._loadingInfo.loadedIndex + 1] = (static_cast<float>(waveInfo.right) / AUDIO_16BIT_NORMALIZE_FACTOR);
		}
		//�C���N�������g
		{
			wave._loadingInfo.loadedIndex += static_cast<unsigned int>(wave.GetWaveFormat().nChannels);
		}

	} while (wave._loadingInfo.cycleMax > wave._loadingInfo.cycleNum);

	wave._loadingInfo.isWaveLoaded = true;
	//�n���h���N���[�Y
	mmioClose(mmio, 0);

}

void Hashira::LoadStereo32bitSound(HMMIO mmio, AudioWaveSource & wave, MMCKINFO & dataChunk, unsigned int rawFormatSampleBit, unsigned int firstReadIndex)
{

	Stereo16Bit waveInfo{}; //�ǂݍ��񂾔g�`�̏��		
	do {
		wave._loadingInfo.loadedSize += mmioRead(mmio, (HPSTR)&waveInfo, sizeof(Stereo16Bit));
		if (wave._loadingInfo.loadedSize != sizeof(Stereo16Bit)*(++wave._loadingInfo.cycleNum)) {
			ERROR_LOG(std::string("mmioRead�����s���܂����B(WaveLoad)"));
			mmioClose(mmio, 0);
			return;
		}
		//�g�`�@���K��v����ё��
		{
			wave.GetWave()[wave._loadingInfo.loadedIndex] = (static_cast<float>(waveInfo.left));
			wave.GetWave()[wave._loadingInfo.loadedIndex + 1] = (static_cast<float>(waveInfo.right) );
		}
		//�C���N�������g
		{
			wave._loadingInfo.loadedIndex += static_cast<unsigned int>(wave.GetWaveFormat().nChannels);
		}
	} while (wave._loadingInfo.cycleMax > wave._loadingInfo.cycleNum);

	wave._loadingInfo.isWaveLoaded = true;
	//�n���h���N���[�Y
	mmioClose(mmio, 0);
}


void Hashira::LoadMonaural8bitSound(HMMIO mmio, AudioWaveSource & wave, MMCKINFO & dataChunk, unsigned int rawFormatSampleBit, unsigned int firstReadIndex)
{
	Mono8Bit waveInfo{}; //�ǂݍ��񂾔g�`�̏��		
	do {
		wave._loadingInfo.loadedSize += mmioRead(mmio, (HPSTR)&waveInfo, sizeof(Stereo16Bit));
		if (wave._loadingInfo.loadedSize != sizeof(Mono8Bit)*(++wave._loadingInfo.cycleNum)) {
			ERROR_LOG(std::string("mmioRead�����s���܂����B(WaveLoad)"));
			mmioClose(mmio, 0);
			return;
		}
		//�g�`�@���K��v����ё��
		{
			wave.GetWave()[wave._loadingInfo.loadedIndex] = (static_cast<float>(waveInfo.wave) / AUDIO_8BIT_NORMALIZE_FACTOR);
		}
		//�C���N�������g
		{
			wave._loadingInfo.loadedIndex += static_cast<unsigned int>(wave.GetWaveFormat().nChannels);
		}
	} while (wave._loadingInfo.cycleMax > wave._loadingInfo.cycleNum);

	wave._loadingInfo.isWaveLoaded = true;
	//�n���h���N���[�Y
	mmioClose(mmio, 0);
}

void Hashira::LoadMonaural16bitSound(HMMIO mmio, AudioWaveSource & wave, MMCKINFO & dataChunk, unsigned int rawFormatSampleBit, unsigned int firstReadIndex)
{
	Mono8Bit waveInfo{}; //�ǂݍ��񂾔g�`�̏��		
	do {
		wave._loadingInfo.loadedSize += mmioRead(mmio, (HPSTR)&waveInfo, sizeof(Stereo16Bit));
		if (wave._loadingInfo.loadedSize != sizeof(Mono8Bit)*(++wave._loadingInfo.cycleNum)) {
			ERROR_LOG(std::string("mmioRead�����s���܂����B(WaveLoad)"));
			mmioClose(mmio, 0);
			return;
		}
		//�g�`�@���K��v����ё��
		{
			wave.GetWave()[wave._loadingInfo.loadedIndex] = (static_cast<float>(waveInfo.wave) / AUDIO_16BIT_NORMALIZE_FACTOR);
		}
		//�C���N�������g
		{
			wave._loadingInfo.loadedIndex += static_cast<unsigned int>(wave.GetWaveFormat().nChannels);
		}
	} while (wave._loadingInfo.cycleMax > wave._loadingInfo.cycleNum);

	wave._loadingInfo.isWaveLoaded = true;
	//�n���h���N���[�Y
	mmioClose(mmio, 0);
}

void Hashira::LoadMonaural24bitSound(HMMIO mmio, AudioWaveSource & wave, MMCKINFO & dataChunk, unsigned int rawFormatSampleBit, unsigned int firstReadIndex)
{
	Mono8Bit waveInfo{}; //�ǂݍ��񂾔g�`�̏��		
	do {
		wave._loadingInfo.loadedSize += mmioRead(mmio, (HPSTR)&waveInfo, sizeof(Stereo16Bit));
		if (wave._loadingInfo.loadedSize != sizeof(Mono8Bit)*(++wave._loadingInfo.cycleNum)) {
			ERROR_LOG(std::string("mmioRead�����s���܂����B(WaveLoad)"));
			mmioClose(mmio, 0);
			return;
		}
		//�g�`�@���K��v����ё��
		{
			wave.GetWave()[wave._loadingInfo.loadedIndex] = (static_cast<float>(waveInfo.wave) / AUDIO_16BIT_NORMALIZE_FACTOR);
		}
		//�C���N�������g
		{
			wave._loadingInfo.loadedIndex += static_cast<unsigned int>(wave.GetWaveFormat().nChannels);
		}
	} while (wave._loadingInfo.cycleMax > wave._loadingInfo.cycleNum);

	wave._loadingInfo.isWaveLoaded = true;
	//�n���h���N���[�Y
	mmioClose(mmio, 0);
}

void Hashira::LoadMonaural32bitSound(HMMIO mmio, AudioWaveSource & wave, MMCKINFO & dataChunk, unsigned int rawFormatSampleBit, unsigned int firstReadIndex)
{
	Mono8Bit waveInfo{}; //�ǂݍ��񂾔g�`�̏��		
	do {
		wave._loadingInfo.loadedSize += mmioRead(mmio, (HPSTR)&waveInfo, sizeof(Stereo16Bit));
		if (wave._loadingInfo.loadedSize != sizeof(Mono8Bit)*(++wave._loadingInfo.cycleNum)) {
			ERROR_LOG(std::string("mmioRead�����s���܂����B(WaveLoad)"));
			mmioClose(mmio, 0);
			return;
		}
		//�g�`�@���K��v����ё��
		{
			wave.GetWave()[wave._loadingInfo.loadedIndex] = (static_cast<float>(waveInfo.wave));
		}
		//�C���N�������g
		{
			wave._loadingInfo.loadedIndex += static_cast<unsigned int>(wave.GetWaveFormat().nChannels);
		}
	} while (wave._loadingInfo.cycleMax > wave._loadingInfo.cycleNum);

	wave._loadingInfo.isWaveLoaded = true;
	//�n���h���N���[�Y
	mmioClose(mmio, 0);
}
