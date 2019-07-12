#pragma once
#include "Audio.h"
#include "Engine/Source/Utility/NonCopyable.h"
#include "Engine/Source/Async/ThreadPool.h"
#include <memory>
#include <string>

namespace Hashira {

	class AudioWaveSource;
	class AudioLoader : public NonCopyable
	{

	public:

	private:

		//スレッド数
		static inline unsigned int THREAD_NUM = 8;
		//ワーカースレッド
		ThreadPool _loadThreadPool;
		//ロード停止フラグ
		bool _isStop;

	public:

		~AudioLoader() {};

		static AudioLoader& GetInstance() {
			static AudioLoader instance;
			return instance;
		}

		/**
		* @fn
		* @brief ローダを停止する
		*/
		void StopLoad();

		/**
		* @fn
		* @brief ローダを再起動
		*/
		void ReStartLoad();

		/**
		* @fn
		* @brief ロードが停止中か否か
		* @return trueでロード停止中、それ以外で稼働中
		*/
		bool IsStopLoad();

		/**
		* @fn
		* @brief 波形をロードしているスレッドを停止、破棄する
		*/
		void DiscardWorkerThreads();

		/**
		* @fn
		* @brief オーディオのロード
		* @oparam[in] audioFilePath ファイルパス
		* @return 波形の生データ
		*/
		std::shared_ptr<AudioWaveSource> LoadAudio(std::string audioFilePath);

		/**
		* @fn
		* @brief オーディオのロード
		* @oparam[in] audioFilePath ファイルパス
		* @return 波形の生データ
		*/
		std::shared_ptr<AudioWaveSource> LoadAudioEx(std::string audioFilePath);

	private:

		AudioLoader() : _loadThreadPool(THREAD_NUM) {};

		AudioLoader(const AudioLoader&value) : _loadThreadPool(THREAD_NUM) {};

		AudioLoader(AudioLoader&&value) : _loadThreadPool(THREAD_NUM) {};

		void operator = (const AudioLoader& value) {};

		void operator = (AudioLoader&&value)noexcept {};
	};


}

