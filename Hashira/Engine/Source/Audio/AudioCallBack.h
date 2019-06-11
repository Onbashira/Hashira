#pragma once
#include <xaudio2.h>
#include <functional>
#include <map>

namespace Hashira {
	class AudioCallBack : public IXAudio2VoiceCallback
	{	
	public:
	private:
	protected:
		
		//!�J�n���ɌĂ΂��֐��I�u�W�F�N�g
		std::function<void(UINT32)> _onVoiceProcessingPassStart;

		//!�I�����ɌĂ΂��֐��I�u�W�F�N�g
		std::function<void()> _onVoiceProcessingPassEnd;

		//!�X�g���[���I���n�_�ŌĂ΂��֐��I�u�W�F�N�g
		std::function<void()> _onStreamEnd;

		//!�o�b�t�@�̓��ŌĂ΂��֐��I�u�W�F�N�g
		std::function<void(void*)> _onBufferStart;

		//!�o�b�t�@�I�[�ŌĂ΂��֐��I�u�W�F�N�g
		std::function<void(void*)> _onBufferEnd;

		//!���[�v���ɌĂ΂��֐��I�u�W�F�N�g
		std::function<void(void*)> _onLoopEnd;

		//!�G���[�������ɌĂ΂��֐��I�u�W�F�N�g
		std::function<void(void*, HRESULT)> _onVoiceError;

	public:

		AudioCallBack();

		virtual ~AudioCallBack();

		/**
		 * @fn
		 * @brief �J�n���ɃR�[�������֐�
		 */
		virtual void STDMETHODCALLTYPE OnVoiceProcessingPassStart(UINT32 BytesRequired)override;

		/**
		 * @fn
		 * @brief �I�����ɃR�[�������
		 */
		virtual void STDMETHODCALLTYPE OnVoiceProcessingPassEnd(void)override;
		
		/**
		 * @fn
		 * @brief �X�g���[���G���h�ɃR�[��
		 */
		virtual void STDMETHODCALLTYPE OnStreamEnd(void)override;
		
		/**
		 * @fn
		 * @brief �o�b�t�@�X�^�[�g�̎��ɌĂ΂��
		 */
		virtual void STDMETHODCALLTYPE OnBufferStart(void *pBufferContext)override;

		/**
		 * @fn
		 * @brief �o�b�t�@�̏I�[�ŌĂ΂��
		 */
		virtual void STDMETHODCALLTYPE OnBufferEnd(void *pBufferContext)override;

		/**
		 * @fn
		 * @brief ���[�v�G���h���ɌĂ΂��
		 */
		virtual void STDMETHODCALLTYPE OnLoopEnd(void *pBufferContext)override;

		/**
		 * @fn
		 * @brief �G���[���N�������Ƃ��ɌĂ΂��
		 */
		virtual void STDMETHODCALLTYPE OnVoiceError(void *pBufferContext, HRESULT Error)override;

		/**
		 * @fn
		 * @brief �J�n���ɃR�[�������
		 */
		void  SetOnVoiceProcessingPassStart(std::function<void(UINT32)> func);

		/**
		 * @fn
		 * @brief �I�����ɃR�[�������
		 */
		void  SetOnVoiceProcessingPassEnd(std::function<void()> func);

		/**
		 * @fn
		 * @brief �X�g���[���G���h�ɃR�[��
		 */
		void  SetOnStreamEnd(std::function<void()> func);

		/**
		�@* @fn
		�@* @brief �o�b�t�@�X�^�[�g�̎��ɌĂ΂��
		�@*/
		void  SetOnBufferStart(std::function<void(void*)> func);

		/**
		 * @fn
		 * @brief �o�b�t�@�̏I�[�ŌĂ΂��
		 */
		void  SetOnBufferEnd(std::function<void(void*)> func);

		/**
		 * @fn
		 * @brief ���[�v�G���h���ɌĂ΂��
		 */
		void  SetOnLoopEnd(std::function<void(void*)> func);

		/**
		 * @fn
		 * @brief �G���[���N�������Ƃ��ɌĂ΂��
		 */
		void  SetOnVoiceError(std::function<void(void*,HRESULT)> func);

	private:

	protected:

	};
}
