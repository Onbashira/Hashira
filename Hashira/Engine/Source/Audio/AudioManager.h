#pragma once
#include <xaudio2.h>
#pragma comment(lib, "xaudio2.lib")
#pragma comment(lib, "winmm.lib")

#include "Engine/Source/Utility/NonCopyable.h"
#include "AudioCallBack.h"
#include <mmsystem.h>
#include <string>
#include <wrl.h>
#include <memory>


namespace Hashira {

	class Audio;
	class AudioWaveSource;
	class AudioCallBack;

	class AudioManager : private NonCopyable
	{

		friend class Framework;

	public:

	private:

		//!XAudio2
		IXAudio2* _xAudio2;

		//�I�}�X�^�{�C�X
		IXAudio2MasteringVoice* _masterVoice;

	public:

		~AudioManager();

		/**
		* @fn
		* @brief Xaudio2������
		*/
		void InitializeXAudio2();

		/**
		* @fn
		* @brief �j��
		*/
		void Discard();

		/**
		* @fn
		* @brief �T�E���h�G���W���̋N��
		*/
		void StartSoundEngine();

		/**
		* @fn
		* @brief �T�E���h�G���W���̒�~
		*/
		void StopSoundEngine();

		/**
		* @fn
		* @brief �I�[�f�B�I�̃��[�h
		* @oparam[in] audioFilePath �t�@�C���p�X
		* @return �T�E���h�f�[�^
		*/
		std::unique_ptr<Audio> LoadAudio(std::string audioPath);

	private:

		AudioManager();

		AudioManager(const AudioManager&) {};

		AudioManager(AudioManager&&) {};

		void operator = (const AudioManager& ) {};

		void operator = (AudioManager&&) {};

		/**
		* @fn
		* @brief �\�[�X�{�C�X�̍쐬
		* @oparam[in] waveResource �t�@�C���p�X
		* @oparam[in] callback �R�[���o�b�N
		* @oparam[in] sendList ���X�g
		* @oparam[in] effectChain �G�t�F�N�g
		* @return �T�E���h�f�[�^
		*/
		std::unique_ptr<Audio> CreateSourceVoice(std::weak_ptr<AudioWaveSource> waveResource, AudioCallBack *callback = nullptr,
			const XAUDIO2_VOICE_SENDS *sendList = nullptr, const XAUDIO2_EFFECT_CHAIN *effectChain = nullptr);
		
		/**
		* @fn
		* @brief �\�[�X�{�C�X�̍쐬
		* @oparam[in] waveResource �t�@�C���p�X
		* @oparam[in] callback �R�[���o�b�N
		* @oparam[in] sendList ���X�g
		* @oparam[in] effectChain �G�t�F�N�g
		* @return �T�E���h�f�[�^
		*/
		std::unique_ptr<Audio> CreateSourceVoiceEx(std::weak_ptr<AudioWaveSource> waveResource, AudioCallBack *callback = nullptr,
			const XAUDIO2_VOICE_SENDS *sendList = nullptr, const XAUDIO2_EFFECT_CHAIN *effectChain = nullptr);
	};

}