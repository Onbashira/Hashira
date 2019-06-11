#pragma once
namespace Hashira {
	class Timer
	{
	public:

	private:

		//!一秒毎のカウント
		double _secondsPerCount;

		//!デルタタイム
		double _deltaTime;

		//!ベースタイム
		__int64 _baseTime;

		//!一時停止タイム
		__int64 _pausedTime;

		//!停止タイム
		__int64 _stopTime;

		//!前の時間
		__int64 _prevTime;

		//!現在の時間
		__int64 _currentTime;

		//!停止しているか
		bool _stopped;

	public:

		Timer();

		~Timer();

		/**
		* @fn
		* @brief 前フレームとの差分時間の取得
		* @return デルタタイム
		*/
		float DeltaTime()const;

		/**
		* @fn
		* @brief トータル時間の取得
		* @return トータル時間
		*/
		float TotalTime()const;

		/**
		* @fn
		* @brief タイマーリセット
		*/
		void Reset();

		/**
		* @fn
		* @brief タイマースタート
		*/
		void Start();

		/**
		* @fn
		* @brief タイマーストップ
		*/
		void Stop();

		/**
		* @fn
		* @brief 計測
		*/
		void Tick();

	private:

	};
}

