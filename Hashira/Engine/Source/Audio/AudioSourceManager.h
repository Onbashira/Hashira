#pragma once
#include "Engine/Source/Utility/NonCopyable.h"
#include "Engine/Source/Utility/ManagerComponent.h"

namespace Hashira {
	class AudioWaveSource;
	class AudioSourceManager : private NonCopyable
	{

	private:

		//!�g�`�̐��f�[�^��ێ�����N���X
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
		* @brief �g�`�f�[�^�̃Z�b�g
		* @oparam[in] resourceName �g�`�̖��O
		* @oparam[in] source �g�`�̐��f�[�^
		*/
		void SetResource(std::string resourceName, std::shared_ptr<AudioWaveSource> source);

		/**
		* @fn
		* @brief �g�`�f�[�^�̃Z�b�g
		* @oparam[in] resourceName �g�`�̖��O
		* @return �g�`�f�[�^�ւ̎�Q��
		*/
		std::weak_ptr<Hashira::AudioWaveSource> GetResource(std::string resourceName);
	
		/**
		* @fn
		* @brief �g�`�폜
		*/
		void EraseResource(std::string name);

		/**
		* @fn
		* @brief �g�`�����[�h�ς݂��ǂ���
		* @oparam[in] resourceName �g�`�̖��O
		* @return true�Ń��[�h�ς݁AFalse�Ŗ����[�h
		*/
		bool IsLoaded(std::string resourceName);

		/**
		* @fn
		* @brief �ێ��f�[�^�̔j��
		*/
		void DiscardManager();
	};
}

