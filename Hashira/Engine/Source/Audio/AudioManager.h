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

		//！マスタボイス
		IXAudio2MasteringVoice* _masterVoice;

	public:

		~AudioManager();

		/**
		* @fn
		* @brief Xaudio2初期化
		*/
		void InitializeXAudio2();

		/**
		* @fn
		* @brief 破棄
		*/
		void Discard();

		/**
		* @fn
		* @brief サウンドエンジンの起動
		*/
		void StartSoundEngine();

		/**
		* @fn
		* @brief サウンドエンジンの停止
		*/
		void StopSoundEngine();

		/**
		* @fn
		* @brief オーディオのロード
		* @oparam[in] audioFilePath ファイルパス
		* @return サウンドデータ
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
		* @brief ソースボイスの作成
		* @oparam[in] waveResource ファイルパス
		* @oparam[in] callback コールバック
		* @oparam[in] sendList リスト
		* @oparam[in] effectChain エフェクト
		* @return サウンドデータ
		*/
		std::unique_ptr<Audio> CreateSourceVoice(std::weak_ptr<AudioWaveSource> waveResource, AudioCallBack *callback = nullptr,
			const XAUDIO2_VOICE_SENDS *sendList = nullptr, const XAUDIO2_EFFECT_CHAIN *effectChain = nullptr);
		
		/**
		* @fn
		* @brief ソースボイスの作成
		* @oparam[in] waveResource ファイルパス
		* @oparam[in] callback コールバック
		* @oparam[in] sendList リスト
		* @oparam[in] effectChain エフェクト
		* @return サウンドデータ
		*/
		std::unique_ptr<Audio> CreateSourceVoiceEx(std::weak_ptr<AudioWaveSource> waveResource, AudioCallBack *callback = nullptr,
			const XAUDIO2_VOICE_SENDS *sendList = nullptr, const XAUDIO2_EFFECT_CHAIN *effectChain = nullptr);
	};

}