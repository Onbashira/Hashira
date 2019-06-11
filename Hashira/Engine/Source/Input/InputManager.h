#pragma once
#include "Engine/Source/Math/Math.h"
#include "GamePadManager.h"
#include <Windows.h>

namespace Hashira {

	enum VIRTUAL_KEY_STATE {
		VKS_LBUTTON = 0x01,	//�}�E�X���{�^��
		VKS_RBUTTON = 0x02,	//�}�E�X�E�{�^��
		VKS_CANCEL = 0x03,	//Ctrl - Break
		VKS_MBUTTON = 0x04,	//�}�E�X�����{�^��
		VKS_BACK = 0x08,	//Backspace
		VKS_TAB = 0x09,		//Tab
		VKS_CLEAR = 0x0C,	//NumLock ���O������Ԃ̃e���L�[5
		VKS_RETURN = 0x0D,	//Enter
		VKS_SHIFT = 0x10,	//Shift
		VKS_CONTROL = 0x11,	//Ctrl
		VKS_MENU = 0x12,	//Alt
		VKS_PAUSE = 0x13,	//Pause
		VKS_CAPITAL = 0x14,	//Caps Lock
		VKS_KANA = 0x15,
		VKS_HANGEUL,
		VKS_HANGUL,
		VLS_JUNJA = 0x17,
		VKS_FINAL = 0x18,
		VKS_HANJA = 0x19,
		VKS_KANJI,
		VKS_ESCAPE = 0x1B,	 //Esc
		VKS_CONVERT = 0x1C,
		VKS_NONCONVERT = 0x1D,
		VKS_ACCEPT = 0x1E,
		VKS_MODECHANGE = 0x1F,
		VKS_SPACE = 0x20,	//Spacebar
		VKS_PRIOR = 0x21,	//Page Up
		VKS_NEXT = 0x22,	//Page Down
		VKS_END = 0x23,		 //End
		VKS_HOME = 0x24,	//Home
		VKS_LEFT = 0x25,	//�������L�[
		VKS_UP = 0x26,		//������L�[
		VKS_RIGHT = 0x27,	// �E�����L�[
		VKS_DOWN = 0x28,	//�������L�[
		VKS_SELECT = 0x29,
		VKS_PRINT = 0x2A,
		VKS_EXECUTE = 0x2B,
		VKS_SNAPSHOT = 0x2C,// PringScreen
		VKS_INSERT = 0x2D,	// Insert
		VKS_DELETE = 0x2E,	// Delete
		VKS_HELP = 0x2F,
		VKS_0 = 0x30,		//--S	0x30�`0x39	//���C���L�[�{�[�h 0�`9
		VKS_1,
		VKS_2,
		VKS_3,
		VKS_4,
		VKS_5,
		VKS_6,
		VKS_7,
		VKS_8,
		VKS_9,
		VKS_A = 0x41,//--S	0x41�`0x5A	//�����L�[ A ���� Z
		VKS_B,
		VKS_C,
		VKS_D,
		VKS_E,
		VKS_F,
		VKS_G,
		VKS_H,
		VKS_I,
		VKS_J,
		VKS_K,
		VKS_L,
		VKS_M,
		VKS_N,
		VKS_O,
		VKS_P,
		VKS_Q,
		VKS_R,
		VKS_S,
		VKS_T,
		VKS_U,
		VKS_V,
		VKS_W,
		VKS_X,
		VKS_Y,
		VKS_Z,
		VKS_LWIN = 0x5B,	//�� Windows �L�[
		VKS_RWIN = 0x5C,	//�E Windows �L�[
		VKS_APPS = 0x5D,	//�A�v���P�[�V�����L�[
		VKS_NUMPAD0 = 0x60, //NumLock ���I���ɂȂ��Ă����Ԃł�S�ꂼ��e���L�[�� 0 �` 9 �܂�
		VKS_NUMPAD1 = 0x61,
		VKS_NUMPAD2 = 0x62,
		VKS_NUMPAD3 = 0x63,
		VKS_NUMPAD4 = 0x64,
		VKS_NUMPAD5 = 0x65,
		VKS_NUMPAD6 = 0x66,
		VKS_NUMPAD7 = 0x67,
		VKS_NUMPAD8 = 0x68,
		VKS_NUMPAD9 = 0x69,
		VKS_MULTIPLY = 0x6A,// �e���L�[ *
		VKS_ADD = 0x6B,		//�e���L�[ +
		VKS_SEPARATOR = 0x6C,
		VKS_SUBTRACT = 0x6D,// �e���L�[ -
		VKS_DECIMAL = 0x6E,	// �e���L�[ .
		VKS_DIVIDE = 0x6F,	//�e���L�[ /
		VKS_F1 = 0x70,		//���ꂼ�� F1 ���� F24 �܂ł̃t�@���N�V�����L�[
		VKS_F2 = 0x71,
		VKS_F3 = 0x72,
		VKS_F4 = 0x73,
		VKS_F5 = 0x74,
		VKS_F6 = 0x75,
		VKS_F7 = 0x76,
		VKS_F8 = 0x77,
		VKS_F9 = 0x78,
		VKS_F10 = 0x79,
		VKS_F11 = 0x7A,
		VKS_F12 = 0x7B,
		VKS_F13 = 0x7C,
		VKS_F14 = 0x7D,
		VKS_F15 = 0x7E,
		VKS_F16 = 0x7F,
		VKS_F17 = 0x80,
		VKS_F18 = 0x81,
		VKS_F19 = 0x82,
		VKS_F20 = 0x83,
		VKS_F21 = 0x84,
		VKS_F22 = 0x85,
		VKS_F23 = 0x86,
		VKS_F24 = 0x87,
		VKS_NUMLOCK = 0x90,	//Num Lock
		VKS_SCROLL = 0x91,	// Scroll
		VKS_LSHIFT = 0xA0,	// Windows NT : ��Shift�L�[
		VKS_RSHIFT = 0xA1,	//Windows NT : �EShift�L�[
		VKS_LCONTROL = 0xA2,//Windows NT : ��Ctrl�L�[
		VKS_RCONTROL = 0xA3,// Windows NT : �ECtrl�L�[
		VKS_LMENU = 0xA4,	//Windows NT : ��Alt�L�[
		VKS_RMENU = 0xA5,	// Windows NT : �EAlt�L�[
		VKS_PROCESSKEY = 0xE5,// WINVER �� 0x400 �Ɠ���������ȏ�ł���Β�`�����
		VKS_ATTN = 0xF6,
		VKS_CRSEL = 0xF7,
		VKS_EXSEL = 0xF8,
		VKS_EREOF = 0xF9,
		VKS_PLAY = 0xFA,
		VKS_ZOOM = 0xFB,
		VKS_NONAME = 0xFC,
		VKS_PA1 = 0xFD,
		VKS_OME_CLEAR
	};

	//�C���v�b�g�S�̂��Ǘ�
	class InputManager
	{
	public:

	private:
		//!�O�t���[���̃L�[�{�[�h�̓��͏��
		BYTE _preKey[256];

		//!���t���[���̃L�[�{�[�h�̓��͏��
		BYTE _key[256];

		//!�}�E�X���
		Vector2 _mousePos;
		
		//!�Q�[���p�b�h�}�l�[�W��
		GamePadManager _gamePadManager;

		//!�t�H�[�J�X���Ă���HWND
		HWND _forcusWindowHandle;

	public:

		InputManager();

		~InputManager();

		/**
		* @fn
		* @brief ���͑S�̂̍X�V
		*/
		void InputUpdate();

		/**
		* @fn
		* @brief �L�[�������ꂽ�u�Ԃ��ǂ���
		* @param[in] code ���z�L�[�R�[�h
		* @return �^�ŉ����ꂽ�u��
		*/
		bool IsTriggerDown(VIRTUAL_KEY_STATE code);

		/**
		* @fn
		* @brief �L�[�������ꂽ�u�Ԃ��ǂ���
		* @param[in] code ���z�L�[�R�[�h
		* @return �^�ŗ����ꂽ�u��
		*/
		bool IsTriggerUp(VIRTUAL_KEY_STATE code);

		/**
		* @fn
		* @brief �L�[��������Ă���u�Ԃ��ǂ���
		* @param[in] code ���z�L�[�R�[�h
		* @return �^�ŉ�����Ă�����
		*/
		bool IsDown(VIRTUAL_KEY_STATE code);

		/**
		* @fn
		* @brief �L�[���g�O����Ԃ��ǂ���
		* @param[in] code ���z�L�[�R�[�h
		* @return �^�Ńg�O�����
		*/
		bool IsToggle(VIRTUAL_KEY_STATE code);

		/**
		* @fn
		* @brief �}�E�X�̈ʒu�̎擾
		* @return �}�E�X�̃X�N���[���X�y�[�X�ł̈ʒu
		*/
		Vector2 GetMousePos();

		/**
		* @fn
		* @brief �j��
		*/
		void Discard();

		/**
		* @fn
		* @brief �t�H�[�J�X��̃Z�b�g
		* @param[in] forcusWindow �t�H�[�J�X��E�C���h�E�n���h��
		*/
		void SetFocusWindow(HWND forcusWindow);

		/**
		* @fn
		* @brief �R���g���[���[�̏�����
		* @param[in] cotrollerNum �R���g���[���[��
		*/
		void GamePadInitialize(int cotrollerNum = 4);

		/**
		* @fn
		* @brief �Q�[���p�b�h�}�l�[�W���̃t�F�b�`
		* @return �Q�[���p�b�h�}�l�[�W���̎Q��
		*/
		GamePadManager& GetGamePadManager();

		/**
		* @fn
		* @brief �Q�[���p�b�h�̃t�F�b�`
		* @param[in] cotrollerID �R���g���[��ID
		* @return �Q�[���p�b�h�̎Q��
		*/
		std::shared_ptr<GamePad> GetGamePad(int cotrollerID = 0);

	private:

	};
}

