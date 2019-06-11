#pragma once
#include "Engine/Source/Math/Math.h"
#include <Windows.h>
#include <functional>
#include <Xinput.h>
#pragma comment(lib,"xinput.lib")

namespace Hashira {
	enum class GAME_PAD {

		DPAD_UP = 0x0001,
		DPAD_DOWN = 0x0002,
		DPAD_LEFT = 0x0004,
		DPAD_RIGHT = 0x0008,
		START = 0x0010,
		BACK = 0x0020,
		LEFT_THUMB = 0x0040,
		RIGHT_THUMB = 0x0080,
		LEFT_SHOULDER = 0x0100,
		RIGHT_SHOULDER = 0x0200,
		A = 0x1000,
		B = 0x2000,
		X = 0x4000,
		Y = 0x8000,
		RT = 0x0400,
		LT = 0x0800,
		R_STICK = 0x1001,
		L_STICK = 0x1002,
	};

	class GamePad
	{

	public:

	private:

		//!ゲームパッドID
		int					_gamePadID;

		//!右スティックのデッドライン
		unsigned short		_righrStickDeadline;

		//!左スティックのデッドライン
		unsigned short		_leftStickDeadline;

		//!右スティックの修正されたX値
		short				_rightStickFixValueX;

		//!右スティックの修正されたY値
		short				_rightStickFixValueY;

		//!左スティックの修正されたX値
		short				_leftStickFixValueX;

		//!左スティックの修正されたY値
		short				_leftStickFixValueY;

		//!右トリガーのデッドライン
		unsigned char		_rightTriggerDeadline;

		//!左トリガーのデッドライン
		unsigned char		_leftTriggerDeadline;

		//!右トリガーの修正された値
		unsigned char		_rightTriggerFixValue;

		//!左トリガーの修正された値
		unsigned char		_leftTriggerFixValue;

		//!右バイブレーターの振動時間
		unsigned int		_rightVibrationTime;

		//!左バイブレーターの振動時間
		unsigned int		_leftVibrationTime;

		//!右バイブレーターの挙動オブジェクト
		std::function<void()> _rightVibrationController;

		//!左バイブレーターの挙動オブジェクト
		std::function<void()> _leftVibrationController;

		//!バイブレーション情報
		XINPUT_VIBRATION	_vibration;

		//!現在のステート
		XINPUT_STATE		_nowState;

		//!前フレームのステート
		XINPUT_STATE		_oldState;

	public:	
		
		GamePad();

		~GamePad();

		GamePad(const GamePad& other);

		GamePad(GamePad&& other);

		GamePad& operator=(const  GamePad& value);

		GamePad& operator=(GamePad&& value);

		/**
		* @fn
		* @brief 更新
		*/
		void Update();

		/**
		* @fn
		* @brief コントローラのIDを設定する
		* @param[in] id ココントローラーのID
		*/
		void SetControllerID(int id);

		/**
		* @fn
		* @brief コントローラーを初期化する
		* @param[in] id コントローラーへのID
		*/
		void SetUp(int id);

		/**
		* @fn
		* @brief 押下情報をロギングする
		*/
		void ButtonDebug();

		/**
		* @fn
		* @brief アナログスティックの操作情報をロギングする
		*/
		void AnalogDebug();

		/**
		* @fn
		* @brief キーコードのボタンが押下された瞬間かどうか
		* @param[in] keycode キーコード
		* @return 真でtrue
		*/
		bool IsTriggerDownButton(GAME_PAD keycode);

		/**
		* @fn
		* @brief キーコードのボタンが話された瞬間かどうか
		* @param[in] keycode キーコード
		* @return 真でtrue
		*/
		bool IsTriggerUpButton(GAME_PAD keycode);

		/**
		* @fn
		* @brief キーコードのボタンが押下されているかどうか
		* @param[in] keycode キーコード
		* @return 真でtrue
		*/
		bool IsDownButton(GAME_PAD keycode);

		/**
		* @fn
		* @brief デッドラインの設定
		* @param[in] deadline キーコード
		*/
		void SetRightStickDeadline(unsigned short deadline);

		/**
		* @fn
		* @brief デッドラインの設定
		* @param[in] deadline キーコード
		*/
		void SetLeftStickDeadline(unsigned short deadline);

		/**
		* @fn
		* @brief デッドラインの設定
		* @param[in] deadline キーコード
		*/
		void SetRightTriggerDeadline(unsigned char deadline);

		/**
		* @fn
		* @brief デッドラインの設定
		* @param[in] deadline キーコード
		*/
		void SetLeftTriggerDeadline(unsigned char deadline);

		/**
		* @fn
		* @brief 振動の設定
		* @param[in] frequency 周波数
		* @param[in] frame 持続フレーム
		*/
		void VibrationRightMotor(unsigned short frequency = 0, unsigned int frame = 0);

		/**
		* @fn
		* @brief 振動の設定
		* @param[in] frequency 周波数
		* @param[in] frame 持続フレーム
		*/
		void VibrationLeftMotor(unsigned short frequency = 0, unsigned int frame = 0);
		
		/**
		* @fn
		* @brief -1 ~ 1 にクランプされたスティックの倒し情報
		* @return 操作していない状態を（0,0）とした位相差
		*/
		Vector2 GetSaturatedRightStick();
		
		/**
		* @fn
		* @brief -1 ~ 1 にクランプされたスティックの倒し情報
		* @return 操作していない状態を（0,0）とした位相差
		*/
		Vector2 GetSaturatedLeftStick();
		
		/**
		* @fn
		* @brief -1 ~ 1 にクランプされたトリガーの押し込み情報
		* @return 操作していない状態を（0,0）とした位相差
		*/
		float  GetSaturatedRT();
		
		/**
		* @fn
		* @brief -1 ~ 1 にクランプされたトリガーの押し込み情報
		* @return 操作していない状態を（0,0）とした位相差
		*/
		float  GetSaturatedLT();

	private:

		/**
		* @fn
		* @brief 右バイブレータを振動させる
		*/
		void VibrationRight();

		/**
		* @fn
		* @brief 右バイブレータを停止させる
		*/
		void VibrationStopRight();

		/**
		* @fn
		* @brief 右バイブレータ振動させる
		*/
		void VibrationLeft();

		/**
		* @fn
		* @brief 右バイブレータを停止させる
		*/
		void VibrationStopLeft();

		/**
		* @fn
		* @brief enumの入力値からキーコードを取得
		* @param[in] button ボタンのEnumClass
		* @return キーコード
		*/
		unsigned short GetPadKeyCode(GAME_PAD button);

		/**
		* @fn
		* @brief enumの入力値からボタン名を取得
		* @param[in] button ボタンのEnumClass
		* @return ボタン名
		*/
		std::string GetPadKeyName(GAME_PAD button);
	};
}