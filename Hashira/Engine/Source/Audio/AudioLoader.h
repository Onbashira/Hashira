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

		//�X���b�h��
		static inline unsigned int THREAD_NUM = 8;
		//���[�J�[�X���b�h
		ThreadPool _loadThreadPool;
		//���[�h��~�t���O
		bool _isStop;

	public:

		~AudioLoader() {};

		static AudioLoader& GetInstance() {
			static AudioLoader instance;
			return instance;
		}

		/**
		* @fn
		* @brief ���[�_���~����
		*/
		void StopLoad();

		/**
		* @fn
		* @brief ���[�_���ċN��
		*/
		void ReStartLoad();

		/**
		* @fn
		* @brief ���[�h����~�����ۂ�
		* @return true�Ń��[�h��~���A����ȊO�ŉғ���
		*/
		bool IsStopLoad();

		/**
		* @fn
		* @brief �g�`�����[�h���Ă���X���b�h���~�A�j������
		*/
		void DiscardWorkerThreads();

		/**
		* @fn
		* @brief �I�[�f�B�I�̃��[�h
		* @oparam[in] audioFilePath �t�@�C���p�X
		* @return �g�`�̐��f�[�^
		*/
		std::shared_ptr<AudioWaveSource> LoadAudio(std::string audioFilePath);

		/**
		* @fn
		* @brief �I�[�f�B�I�̃��[�h
		* @oparam[in] audioFilePath �t�@�C���p�X
		* @return �g�`�̐��f�[�^
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

