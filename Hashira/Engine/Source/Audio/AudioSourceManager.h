#pragma once
#include "Engine/Source/Utility/NonCopyable.h"
#include "Engine/Source/Utility/ManagerComponent.h"

namespace Hashira {
	class AudioWaveSource;
	class AudioSourceManager : private NonCopyable
	{

	private:

		//!波形の生データを保持するクラス
		UnorderedManagerComponent<AudioWaveSource> _resourceMap;

	public:

	private:

		AudioSourceManager() {};

		AudioSourceManager(const AudioSourceManager&value) {};

		AudioSourceManager(AudioSourceManager&&value) {};

		void operator = (const AudioSourceManager& value) {};

		void operator = (AudioSourceManager&&value) {};

	public:

		~AudioSourceManager() { DiscardManager(); };

		static AudioSourceManager& GetInstance() { static AudioSourceManager instance; return instance; };

		/**
		* @fn
		* @brief 波形データのセット
		* @oparam[in] resourceName 波形の名前
		* @oparam[in] source 波形の生データ
		*/
		void SetResource(std::string resourceName, std::shared_ptr<AudioWaveSource> source);

		/**
		* @fn
		* @brief 波形データのセット
		* @oparam[in] resourceName 波形の名前
		* @return 波形データへの弱参照
		*/
		std::weak_ptr<Hashira::AudioWaveSource> GetResource(std::string resourceName);
	
		/**
		* @fn
		* @brief 波形削除
		*/
		void EraseResource(std::string name);

		/**
		* @fn
		* @brief 波形がロード済みかどうか
		* @oparam[in] resourceName 波形の名前
		* @return trueでロード済み、Falseで未ロード
		*/
		bool IsLoaded(std::string resourceName);

		/**
		* @fn
		* @brief 保持データの破棄
		*/
		void DiscardManager();
	};
}

