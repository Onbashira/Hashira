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
		
		//!開始時に呼ばれる関数オブジェクト
		std::function<void(UINT32)> _onVoiceProcessingPassStart;

		//!終了時に呼ばれる関数オブジェクト
		std::function<void()> _onVoiceProcessingPassEnd;

		//!ストリーム終了地点で呼ばれる関数オブジェクト
		std::function<void()> _onStreamEnd;

		//!バッファの頭で呼ばれる関数オブジェクト
		std::function<void(void*)> _onBufferStart;

		//!バッファ終端で呼ばれる関数オブジェクト
		std::function<void(void*)> _onBufferEnd;

		//!ループ時に呼ばれる関数オブジェクト
		std::function<void(void*)> _onLoopEnd;

		//!エラー発生時に呼ばれる関数オブジェクト
		std::function<void(void*, HRESULT)> _onVoiceError;

	public:

		AudioCallBack();

		virtual ~AudioCallBack();

		/**
		 * @fn
		 * @brief 開始時にコールされる関数
		 */
		virtual void STDMETHODCALLTYPE OnVoiceProcessingPassStart(UINT32 BytesRequired)override;

		/**
		 * @fn
		 * @brief 終了時にコールされる
		 */
		virtual void STDMETHODCALLTYPE OnVoiceProcessingPassEnd(void)override;
		
		/**
		 * @fn
		 * @brief ストリームエンドにコール
		 */
		virtual void STDMETHODCALLTYPE OnStreamEnd(void)override;
		
		/**
		 * @fn
		 * @brief バッファスタートの時に呼ばれる
		 */
		virtual void STDMETHODCALLTYPE OnBufferStart(void *pBufferContext)override;

		/**
		 * @fn
		 * @brief バッファの終端で呼ばれる
		 */
		virtual void STDMETHODCALLTYPE OnBufferEnd(void *pBufferContext)override;

		/**
		 * @fn
		 * @brief ループエンド時に呼ばれる
		 */
		virtual void STDMETHODCALLTYPE OnLoopEnd(void *pBufferContext)override;

		/**
		 * @fn
		 * @brief エラーを起こしたときに呼ばれる
		 */
		virtual void STDMETHODCALLTYPE OnVoiceError(void *pBufferContext, HRESULT Error)override;

		/**
		 * @fn
		 * @brief 開始時にコールされる
		 */
		void  SetOnVoiceProcessingPassStart(std::function<void(UINT32)> func);

		/**
		 * @fn
		 * @brief 終了時にコールされる
		 */
		void  SetOnVoiceProcessingPassEnd(std::function<void()> func);

		/**
		 * @fn
		 * @brief ストリームエンドにコール
		 */
		void  SetOnStreamEnd(std::function<void()> func);

		/**
		　* @fn
		　* @brief バッファスタートの時に呼ばれる
		　*/
		void  SetOnBufferStart(std::function<void(void*)> func);

		/**
		 * @fn
		 * @brief バッファの終端で呼ばれる
		 */
		void  SetOnBufferEnd(std::function<void(void*)> func);

		/**
		 * @fn
		 * @brief ループエンド時に呼ばれる
		 */
		void  SetOnLoopEnd(std::function<void(void*)> func);

		/**
		 * @fn
		 * @brief エラーを起こしたときに呼ばれる
		 */
		void  SetOnVoiceError(std::function<void(void*,HRESULT)> func);

	private:

	protected:

	};
}
