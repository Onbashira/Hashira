#pragma once
namespace Hashira {
	class Timer
	{
	public:

	private:

		//!��b���̃J�E���g
		double _secondsPerCount;

		//!�f���^�^�C��
		double _deltaTime;

		//!�x�[�X�^�C��
		__int64 _baseTime;

		//!�ꎞ��~�^�C��
		__int64 _pausedTime;

		//!��~�^�C��
		__int64 _stopTime;

		//!�O�̎���
		__int64 _prevTime;

		//!���݂̎���
		__int64 _currentTime;

		//!��~���Ă��邩
		bool _stopped;

	public:

		Timer();

		~Timer();

		/**
		* @fn
		* @brief �O�t���[���Ƃ̍������Ԃ̎擾
		* @return �f���^�^�C��
		*/
		float DeltaTime()const;

		/**
		* @fn
		* @brief �g�[�^�����Ԃ̎擾
		* @return �g�[�^������
		*/
		float TotalTime()const;

		/**
		* @fn
		* @brief �^�C�}�[���Z�b�g
		*/
		void Reset();

		/**
		* @fn
		* @brief �^�C�}�[�X�^�[�g
		*/
		void Start();

		/**
		* @fn
		* @brief �^�C�}�[�X�g�b�v
		*/
		void Stop();

		/**
		* @fn
		* @brief �v��
		*/
		void Tick();

	private:

	};
}

