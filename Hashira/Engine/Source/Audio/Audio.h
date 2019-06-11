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
		//キューの最大値
		inline static unsigned int AUDIO_BUFFER_QUEUE_MAX = 2;
		//サウンドごとのデータ
		XAUDIO2_BUFFER _audioBuffer;
		//コミット用ソースボイス
		IXAudio2SourceVoice* _sourceVoice;
		//生波形データへの弱参照
		std::weak_ptr<AudioWaveSource> _rawData;
		//コールバック
		AudioCallBack _callBack;
		//音の状態
		XAUDIO2_VOICE_STATE _voiceState;
		//波形データのフォーマット
		WAVEFORMATEXTENSIBLE _format;
		//生データのどの位置から一秒間のサンプリングを行っているかを知らせる要素番号
		unsigned int _seekPoint;
		//曲の最大ながさ
		unsigned int _audioLength;
		//曲のループヘッド
		unsigned int _loopHead;
		//曲のループテール
		unsigned int _loopTail;
		//ループするかしないか。
		bool _isLoop;
		//破棄されたか
		bool _isDiscarded;

	protected:

	public:

		Audio();

		virtual ~Audio();

		/**
		 * @fn
		 * @brief 再生
		 */
		virtual void Play();

		/**
		* @fn
		* @brief ループの頭から再生
		*/
		virtual void PlayFromBeginning();

		/**
		 * @fn
		 * @brief 一時停止
		 * @param[in] pause trueでポーズ
		 */
		virtual void Pause();

		/**
		 * @fn
		 * @brief 停止
		 */
		virtual void Stop();

		/**
		 * @fn
		 * @brief ループ再生を許可
		 */
		void LoopEnable();

		/**
		 * @fn
		 * @brief ループ再生を不許可
		 */
		void LoopDisable();

		/**
		* @fn
		* @brief ループポイントを設定する
		* @param[in] headPointTime ヘッド地点の時間（秒単位）
		* @param[in] tailPointTime テール地点の時間（秒単位）
		*/
		void SetLoopPoints(float headPointTime, float tailPointTime);

		/**
		* @fn
		* @brief ループポイントを設定する
		* @param[in] headPointTime ループヘッド地点の時間（秒単位）
		*/
		void SetLoopHeadPoint(float headPointTime);

		/**
		* @fn
		* @brief ループポイントを設定する
		* @param[in] headPointTime ループテール地点の時間（秒単位）
		*/
		void SetLoopTailPoint(float tailPointTime);

		/**
		 * @fn
		 * @brief 状態の更新
		 */
		void UpdateState();

		/**
		* @fn
		* @brief ボリューム設定
		* @param[in] volume 1.0でゲイン値0（元の音量）
		*/
		virtual void SetVolume(float volume);

		/**
		 * @fn
		 * @brief 再生中かどうか
		 * @return 再生中で真、停止中で偽
		 */
		virtual bool CheckPlaying();

		/**
		 * @fn
		 * @brief 各参照データの所有権の破棄と楽曲停止
		 */
		virtual void Discard();

		/**
		 * @fn
		 * @brief バッファーのサブミット
		 */
		void SubmitBuffer();

		/**
		 * @fn
		 * @brief 一括サブミットをバッファエンド時に呼ばれるコールバックにセット
		 */
		void BulkSubmit();

		/**
		 * @fn
		 * @brief ストリーミングサブミットをバッファエンド時に呼ばれるコールバックにセット
		 */
		void StreamSubmit();

		/**
		* @fn
		* @brief 一括サブミット
		*/
		void DirectBulkSubmit();

		/**
		 * @fn
		 * @brief ストリーミングサブミット
		 */
		void DirectStreamSubmit();

	private:

		/**
		* @fn
		* @brief オーディオバッファに波形データをサブミットする
		* @param[in] audioBytePerSec サブミットするオーディオのバイト数
		* @param[in] bufferPoint 波形開始地点
		*/
		void SubmitAudioBuffer(UINT32 audioBytePerSec, unsigned int bufferPoint);

	};
}