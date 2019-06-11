#pragma once

#include <xaudio2.h>
#include <memory>
#include <array>
#include <deque>
#include "AudioCallBack.h"

namespace Hashira {

	class SubMixVoice;
	class AudioWaveSource;
	class NormalAudioWav;
	class AudioCallBack;


	class Audio
	{
		friend class AudioManager;
	public:

	private:
		//�L���[�̍ő�l
		inline static unsigned int AUDIO_BUFFER_QUEUE_MAX = 2;
		//�T�E���h���Ƃ̃f�[�^
		XAUDIO2_BUFFER _audioBuffer;
		//�R�~�b�g�p�\�[�X�{�C�X
		IXAudio2SourceVoice* _sourceVoice;
		//���g�`�f�[�^�ւ̎�Q��
		std::weak_ptr<AudioWaveSource> _rawData;
		//�R�[���o�b�N
		AudioCallBack _callBack;
		//���̏��
		XAUDIO2_VOICE_STATE _voiceState;
		//�g�`�f�[�^�̃t�H�[�}�b�g
		WAVEFORMATEXTENSIBLE _format;
		//���f�[�^�̂ǂ̈ʒu�����b�Ԃ̃T���v�����O���s���Ă��邩��m�点��v�f�ԍ�
		unsigned int _seekPoint;
		//�Ȃ̍ő�Ȃ���
		unsigned int _audioLength;
		//�Ȃ̃��[�v�w�b�h
		unsigned int _loopHead;
		//�Ȃ̃��[�v�e�[��
		unsigned int _loopTail;
		//���[�v���邩���Ȃ����B
		bool _isLoop;
		//�j�����ꂽ��
		bool _isDiscarded;

	protected:

	public:

		Audio();

		virtual ~Audio();

		/**
		 * @fn
		 * @brief �Đ�
		 */
		virtual void Play();

		/**
		* @fn
		* @brief ���[�v�̓�����Đ�
		*/
		virtual void PlayFromBeginning();

		/**
		 * @fn
		 * @brief �ꎞ��~
		 * @param[in] pause true�Ń|�[�Y
		 */
		virtual void Pause();

		/**
		 * @fn
		 * @brief ��~
		 */
		virtual void Stop();

		/**
		 * @fn
		 * @brief ���[�v�Đ�������
		 */
		void LoopEnable();

		/**
		 * @fn
		 * @brief ���[�v�Đ���s����
		 */
		void LoopDisable();

		/**
		* @fn
		* @brief ���[�v�|�C���g��ݒ肷��
		* @param[in] headPointTime �w�b�h�n�_�̎��ԁi�b�P�ʁj
		* @param[in] tailPointTime �e�[���n�_�̎��ԁi�b�P�ʁj
		*/
		void SetLoopPoints(float headPointTime, float tailPointTime);

		/**
		* @fn
		* @brief ���[�v�|�C���g��ݒ肷��
		* @param[in] headPointTime ���[�v�w�b�h�n�_�̎��ԁi�b�P�ʁj
		*/
		void SetLoopHeadPoint(float headPointTime);

		/**
		* @fn
		* @brief ���[�v�|�C���g��ݒ肷��
		* @param[in] headPointTime ���[�v�e�[���n�_�̎��ԁi�b�P�ʁj
		*/
		void SetLoopTailPoint(float tailPointTime);

		/**
		 * @fn
		 * @brief ��Ԃ̍X�V
		 */
		void UpdateState();

		/**
		* @fn
		* @brief �{�����[���ݒ�
		* @param[in] volume 1.0�ŃQ�C���l0�i���̉��ʁj
		*/
		virtual void SetVolume(float volume);

		/**
		 * @fn
		 * @brief �Đ������ǂ���
		 * @return �Đ����Ő^�A��~���ŋU
		 */
		virtual bool CheckPlaying();

		/**
		 * @fn
		 * @brief �e�Q�ƃf�[�^�̏��L���̔j���Ɗy�Ȓ�~
		 */
		virtual void Discard();

		/**
		 * @fn
		 * @brief �o�b�t�@�[�̃T�u�~�b�g
		 */
		void SubmitBuffer();

		/**
		 * @fn
		 * @brief �ꊇ�T�u�~�b�g���o�b�t�@�G���h���ɌĂ΂��R�[���o�b�N�ɃZ�b�g
		 */
		void BulkSubmit();

		/**
		 * @fn
		 * @brief �X�g���[�~���O�T�u�~�b�g���o�b�t�@�G���h���ɌĂ΂��R�[���o�b�N�ɃZ�b�g
		 */
		void StreamSubmit();

		/**
		* @fn
		* @brief �ꊇ�T�u�~�b�g
		*/
		void DirectBulkSubmit();

		/**
		 * @fn
		 * @brief �X�g���[�~���O�T�u�~�b�g
		 */
		void DirectStreamSubmit();

	private:

		/**
		* @fn
		* @brief �I�[�f�B�I�o�b�t�@�ɔg�`�f�[�^���T�u�~�b�g����
		* @param[in] audioBytePerSec �T�u�~�b�g����I�[�f�B�I�̃o�C�g��
		* @param[in] bufferPoint �g�`�J�n�n�_
		*/
		void SubmitAudioBuffer(UINT32 audioBytePerSec, unsigned int bufferPoint);

	};
}