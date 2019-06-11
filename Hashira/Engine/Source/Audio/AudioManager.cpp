#include "AudioManager.h"
#include "AudioLoader.h"
#include "AudioCallBack.h"
#include "AudioWaveSource.h"
#include "Audio.h"
#include "Engine/Source/Utility/Utility.h"
#include "Engine/Source/Debug/Logger.h"


Hashira::AudioManager::AudioManager()
{


}

Hashira::AudioManager::~AudioManager()
{
	Discard();
}


void Hashira::AudioManager::InitializeXAudio2()
{
	if (FAILED(XAudio2Create(&_xAudio2))) {
		ERROR_LOG(std::string("XAudio2の作成に失敗しました"));
	};
	INFO_LOG(std::string("XAudio2の作成に成功しました"));

	if (FAILED(_xAudio2->CreateMasteringVoice(&_masterVoice))) {
		ERROR_LOG(std::string("MasteringVoiceの作成に失敗しました"));
	};
	INFO_LOG(std::string("MasteringVoiceの作成に成功しました"));
}

std::unique_ptr<Hashira::Audio> Hashira::AudioManager::CreateSourceVoice(std::weak_ptr<AudioWaveSource> waveResource, AudioCallBack * callback, const XAUDIO2_VOICE_SENDS * sendList, const XAUDIO2_EFFECT_CHAIN * effectChain)
{
	std::unique_ptr<Audio> audio = std::make_unique<Audio>();

	if (callback != nullptr) {
		audio->_callBack = *callback;
	}

	_xAudio2->CreateSourceVoice(&audio->_sourceVoice, &waveResource.lock()->GetWaveFormat(), 0, 2.0f, &audio->_callBack, sendList, effectChain);

	//一括送り
	audio->_rawData = waveResource;
	audio->_audioLength = static_cast<unsigned int>(waveResource.lock()->GetWave().size());

	audio->BulkSubmit();
	return audio;
}

std::unique_ptr<Hashira::Audio> Hashira::AudioManager::CreateSourceVoiceEx(std::weak_ptr<AudioWaveSource> waveResource, AudioCallBack * callback, const XAUDIO2_VOICE_SENDS * sendList, const XAUDIO2_EFFECT_CHAIN * effectChain)
{

	std::unique_ptr<Audio> audio = std::make_unique<Audio>();

	if (callback != nullptr) {
		audio->_callBack = *callback;
	}

	_xAudio2->CreateSourceVoice(&audio->_sourceVoice, &waveResource.lock()->GetWaveFormat(), 0, 2.0f, &audio->_callBack, sendList, effectChain);

	//生のデータの参照を持たせる
	audio->_rawData = waveResource;
	//終端位置(最終要素番号+1)を代入 
	audio->_audioLength = static_cast<unsigned int>(waveResource.lock()->GetWave().size());
	audio->_loopHead = audio->_audioLength;
	audio->_loopTail = 0;
	return std::move(audio);
}

void Hashira::AudioManager::Discard()
{
	AudioLoader::GetInstance().StopLoad();
	AudioLoader::GetInstance().DiscardWorkerThreads();

	if (_masterVoice != nullptr) {
		_masterVoice->DestroyVoice();
		_masterVoice = nullptr;
		INFO_LOG(std::string("MasteringVoiceを削除しました"));
	}
	if (_xAudio2 != nullptr) {
		_xAudio2->Release();
		_xAudio2 = nullptr;
		INFO_LOG(std::string("XAudio2を削除しました"));
	}
}

void Hashira::AudioManager::StartSoundEngine()
{
	_xAudio2->StartEngine();
}

void Hashira::AudioManager::StopSoundEngine()
{
	_xAudio2->StopEngine();
}

std::unique_ptr<Hashira::Audio> Hashira::AudioManager::LoadAudio(std::string audioPath)
{
	auto audioResource = AudioLoader::GetInstance().LoadAudioEx(audioPath);

	auto audio = std::move(this->CreateSourceVoiceEx(audioResource));

	return std::move(audio);;
}
