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

		//!�Q�[���p�b�hID
		int					_gamePadID;

		//!�E�X�e�B�b�N�̃f�b�h���C��
		unsigned short		_righrStickDeadline;

		//!���X�e�B�b�N�̃f�b�h���C��
		unsigned short		_leftStickDeadline;

		//!�E�X�e�B�b�N�̏C�����ꂽX�l
		short				_rightStickFixValueX;

		//!�E�X�e�B�b�N�̏C�����ꂽY�l
		short				_rightStickFixValueY;

		//!���X�e�B�b�N�̏C�����ꂽX�l
		short				_leftStickFixValueX;

		//!���X�e�B�b�N�̏C�����ꂽY�l
		short				_leftStickFixValueY;

		//!�E�g���K�[�̃f�b�h���C��
		unsigned char		_rightTriggerDeadline;

		//!���g���K�[�̃f�b�h���C��
		unsigned char		_leftTriggerDeadline;

		//!�E�g���K�[�̏C�����ꂽ�l
		unsigned char		_rightTriggerFixValue;

		//!���g���K�[�̏C�����ꂽ�l
		unsigned char		_leftTriggerFixValue;

		//!�E�o�C�u���[�^�[�̐U������
		unsigned int		_rightVibrationTime;

		//!���o�C�u���[�^�[�̐U������
		unsigned int		_leftVibrationTime;

		//!�E�o�C�u���[�^�[�̋����I�u�W�F�N�g
		std::function<void()> _rightVibrationController;

		//!���o�C�u���[�^�[�̋����I�u�W�F�N�g
		std::function<void()> _leftVibrationController;

		//!�o�C�u���[�V�������
		XINPUT_VIBRATION	_vibration;

		//!���݂̃X�e�[�g
		XINPUT_STATE		_nowState;

		//!�O�t���[���̃X�e�[�g
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
		* @brief �X�V
		*/
		void Update();

		/**
		* @fn
		* @brief �R���g���[����ID��ݒ肷��
		* @param[in] id �R�R���g���[���[��ID
		*/
		void SetControllerID(int id);

		/**
		* @fn
		* @brief �R���g���[���[������������
		* @param[in] id �R���g���[���[�ւ�ID
		*/
		void SetUp(int id);

		/**
		* @fn
		* @brief �����������M���O����
		*/
		void ButtonDebug();

		/**
		* @fn
		* @brief �A�i���O�X�e�B�b�N�̑���������M���O����
		*/
		void AnalogDebug();

		/**
		* @fn
		* @brief �L�[�R�[�h�̃{�^�����������ꂽ�u�Ԃ��ǂ���
		* @param[in] keycode �L�[�R�[�h
		* @return �^��true
		*/
		bool IsTriggerDownButton(GAME_PAD keycode);

		/**
		* @fn
		* @brief �L�[�R�[�h�̃{�^�����b���ꂽ�u�Ԃ��ǂ���
		* @param[in] keycode �L�[�R�[�h
		* @return �^��true
		*/
		bool IsTriggerUpButton(GAME_PAD keycode);

		/**
		* @fn
		* @brief �L�[�R�[�h�̃{�^������������Ă��邩�ǂ���
		* @param[in] keycode �L�[�R�[�h
		* @return �^��true
		*/
		bool IsDownButton(GAME_PAD keycode);

		/**
		* @fn
		* @brief �f�b�h���C���̐ݒ�
		* @param[in] deadline �L�[�R�[�h
		*/
		void SetRightStickDeadline(unsigned short deadline);

		/**
		* @fn
		* @brief �f�b�h���C���̐ݒ�
		* @param[in] deadline �L�[�R�[�h
		*/
		void SetLeftStickDeadline(unsigned short deadline);

		/**
		* @fn
		* @brief �f�b�h���C���̐ݒ�
		* @param[in] deadline �L�[�R�[�h
		*/
		void SetRightTriggerDeadline(unsigned char deadline);

		/**
		* @fn
		* @brief �f�b�h���C���̐ݒ�
		* @param[in] deadline �L�[�R�[�h
		*/
		void SetLeftTriggerDeadline(unsigned char deadline);

		/**
		* @fn
		* @brief �U���̐ݒ�
		* @param[in] frequency ���g��
		* @param[in] frame �����t���[��
		*/
		void VibrationRightMotor(unsigned short frequency = 0, unsigned int frame = 0);

		/**
		* @fn
		* @brief �U���̐ݒ�
		* @param[in] frequency ���g��
		* @param[in] frame �����t���[��
		*/
		void VibrationLeftMotor(unsigned short frequency = 0, unsigned int frame = 0);
		
		/**
		* @fn
		* @brief -1 ~ 1 �ɃN�����v���ꂽ�X�e�B�b�N�̓|�����
		* @return ���삵�Ă��Ȃ���Ԃ��i0,0�j�Ƃ����ʑ���
		*/
		Vector2 GetSaturatedRightStick();
		
		/**
		* @fn
		* @brief -1 ~ 1 �ɃN�����v���ꂽ�X�e�B�b�N�̓|�����
		* @return ���삵�Ă��Ȃ���Ԃ��i0,0�j�Ƃ����ʑ���
		*/
		Vector2 GetSaturatedLeftStick();
		
		/**
		* @fn
		* @brief -1 ~ 1 �ɃN�����v���ꂽ�g���K�[�̉������ݏ��
		* @return ���삵�Ă��Ȃ���Ԃ��i0,0�j�Ƃ����ʑ���
		*/
		float  GetSaturatedRT();
		
		/**
		* @fn
		* @brief -1 ~ 1 �ɃN�����v���ꂽ�g���K�[�̉������ݏ��
		* @return ���삵�Ă��Ȃ���Ԃ��i0,0�j�Ƃ����ʑ���
		*/
		float  GetSaturatedLT();

	private:

		/**
		* @fn
		* @brief �E�o�C�u���[�^��U��������
		*/
		void VibrationRight();

		/**
		* @fn
		* @brief �E�o�C�u���[�^���~������
		*/
		void VibrationStopRight();

		/**
		* @fn
		* @brief �E�o�C�u���[�^�U��������
		*/
		void VibrationLeft();

		/**
		* @fn
		* @brief �E�o�C�u���[�^���~������
		*/
		void VibrationStopLeft();

		/**
		* @fn
		* @brief enum�̓��͒l����L�[�R�[�h���擾
		* @param[in] button �{�^����EnumClass
		* @return �L�[�R�[�h
		*/
		unsigned short GetPadKeyCode(GAME_PAD button);

		/**
		* @fn
		* @brief enum�̓��͒l����{�^�������擾
		* @param[in] button �{�^����EnumClass
		* @return �{�^����
		*/
		std::string GetPadKeyName(GAME_PAD button);
	};
}