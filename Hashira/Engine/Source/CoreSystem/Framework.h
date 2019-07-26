#pragma once
#include "Engine/Source/Utility/D3D12Common.h"
#include "Engine/Source/Device/D3D12Device.h"
#include "Engine/Source/Window/Window.h"
#include "Engine/Source/Utility/NonCopyable.h"
#include "Engine/Source/Factory/Factory.h"
#include "Engine/Source/CommandQueue/CommandQueue.h"
#include "Engine/Source/CommandList/CommandList.h"
#include "Engine/Source/Time/Timer.h"
#include "Engine/Source/Rendering/RenderingManager.h"
#include "Engine/Source/Input/InputManager.h"
#include "Engine/Source/Audio/AudioManager.h"
#include <string>
#include <vector>

namespace Hashira {

	class RenderingDevice;

	class Framework : NonCopyable
	{

	public:
	private:

		//�I�C���X�^���X
		inline static Framework* _instance = nullptr;

		//!�E�C���h�E��
		unsigned int _windowWidth;

		//�I�E�C���h�E����
		unsigned int _windowHeight;

		//�I�o�b�N�o�b�t�@�̐�
		unsigned int _backBufferNum;

		//�IWARP�f�o�C�X���g�p���邩�ǂ���
		bool _useWarpDevice;

		//�IAPP�N���X�l�[��
		std::wstring _appClassName;

		//!�f�o�C�X
		std::shared_ptr<D3D12Device> _device;

		//�t�@�N�g��
		Factory _factory;

		//�E�C���h�E
		Window		_window;

		//!�����_�����O�}�l�[�W��
		RenderingManager _renderingManager;

		//!���͊Ǘ��}�l�[�W��
		InputManager _inputManager;

		//�I�T�E���h�Ǘ��}�l�[�W��
		AudioManager _audioManager;

		//!�喃
		Timer	_timer;

	private:

	public:

		~Framework();

		/**
		* @fn
		* @brief �C���X�^���X�̃Z�b�g�A�b�v
		*/
		static void SetUp() {

			if (_instance == nullptr) {
				_instance = new Framework();
			}
		};

		/**
		* @fn
		* @brief �C���X�^���X�폜
		*/
		static void Destory() {

			if (_instance != nullptr) {
				delete _instance;
			}
		};

		/**
		* @fn
		* @brief �C���X�^���X�폜
		*/
		static Framework& GetInstance() {

			if (_instance != nullptr) {
				return *_instance;
			}
			else {
				//�����G���[
				assert(0);
				std::exit(EXIT_FAILURE);
			}

		};


		/**
		* @fn
		* @brief �C�j�V�����C�Y
		* @return ������S_OK
		*/
		HRESULT Initialize();

		/**
		* @fn
		* @brief �I�[����
		*/
		void Terminate();

		/**
		* @fn
		* @brief �f�o�C�X�t�F�b�`
		* @return �f�o�C�X
		*/
		std::shared_ptr<Hashira::D3D12Device>& GetDevice();

		/**
		* @fn
		* @brief �t�@�N�g���t�F�b�`
		* @return �t�@�N�g��
		*/
		Hashira::Factory& GetFactory();

		/**
		* @fn
		* @brief �E�C���h�E�t�F�b�`
		* @return �E�C���h�E
		*/
		Hashira::Window& GetWindow();

		/**
		* @fn
		* @brief �`��}�l�[�W���t�F�b�`
		* @return �`��}�l�[�W��
		*/
		Hashira::RenderingManager& GetRenderingManagre();

		/**
		* @fn
		* @brief �^�C�}�[�t�F�b�`
		* @return �^�C�}�[
		*/
		Hashira::Timer& Time();

		/**
		* @fn
		* @brief �o�b�N�T�[�t�F�X���t���b�v����
		*/
		void FlipScreen();

		/**
		* @fn
		* @brief �X�N���[�����N���A����
		* @param[in] list�@�R�}���h���X�g�̎�Q��
		*/
		void ClearScreen(std::weak_ptr<CommandList> list);

		/**
		* @fn
		* @brief �E�C���h�E�̖��O��ݒ肷��
		* @param[in] name �E�C���h�E�̖��O
		*/
		void SetWindowName(std::wstring name);

		/**
		* @fn
		* @brief �E�C���h�E�̃T�C�Y��ݒ肷��
		* @param[in] width �E�C���h�E�̕�
		* @param[in] height �E�C���h�E�̍���
		*/
		void SetWindowSize(unsigned int width, unsigned int  height);

		/**
		* @fn
		* @brief �E�C���h�E�̃T�C�Y��ݒ肷��
		* @param[inout] width �E�C���h�E�̕�
		* @param[inout] height �E�C���h�E�̍���
		*/
		void GetWindowSize(unsigned int& width ,unsigned int& height);

		/**
		* @fn
		* @brief �o�b�N�o�b�t�@�̐���ݒ肷��
		* @param[in] backBufferNum �o�b�N�o�b�t�@�̐�
		*/
		void SetBackBufferNum(unsigned int backBufferNum);

		/**
		* @fn
		* @brief �v���J�n
		*/
		void TimerStart();

		/**
		* @fn
		* @brief �v����~
		*/
		void TimerStop();

		/**
		* @fn
		* @brief �Ăяo�����_�ł̍������R�~�b�g
		*/
		void Tick();

		/**
		* @fn
		* @brief �O�t���[���Ƃ̍������擾
		*/
		float DeltaTime();

		/**
		* @fn
		* @brief ���̓}�l�[�W���t�F�b�`
		* @return ���̓}�l�[�W��
		*/
		InputManager& Input();

		/**
		* @fn
		* @brief �I�[�f�B�I�}�l�[�W���t�F�b�`
		* @return �I�[�f�B�I�}�l�[�W��
		*/
		AudioManager& AudioManager();

		/**
		* @fn
		* @brief ���b�Z�[�W���[�v
		*/
		int LoopMessage();

	private:

		Framework();

		Framework(const Framework&)noexcept {};

		Framework(Framework&&)noexcept {};

		Framework& operator =(const Framework&)noexcept { return *this; };

		Framework& operator =(Framework&&)noexcept { return *this; };

		/**
		* @fn
		* @brief �E�C���h�E�̏�����
		* @return�@���U���g�@S_OK�Ő���
		*/
		HRESULT InitWindow();

		/**
		* @fn
		* @brief D3D12�V�X�e���̏�����
		* @return�@���U���g�@S_OK�Ő���
		*/
		HRESULT InitD3D12();

		/**
		* @fn
		* @brief �f�o�C�X�̏�����
		* @return�@���U���g�@S_OK�Ő���
		*/
		HRESULT InitDevice();

		/**
		* @fn
		* @brief �t�@�N�g���̏�����
		* @return�@���U���g�@S_OK�Ő���
		*/
		HRESULT InitFactory();

		/**
		* @fn
		* @brief �����_�����O�}�l�[�W���̏�����
		* @return�@���U���g�@S_OK�Ő���
		*/
		HRESULT InitRenderingManager();

		/**
		* @fn
		* @brief ���̓}�l�[�W���̏�����
		* @return�@���U���g�@S_OK�Ő���
		*/
		HRESULT InitInputManager();

		/**
		* @fn
		* @brief �I�[�f�B�I�}�l�[�W���̏�����
		* @return�@���U���g�@S_OK�Ő���
		*/
		HRESULT InitAudioManager();
		
		/**
		* @fn
		* @brief �e�N�X�`���}�l�[�W���̏�����
		* @return�@���U���g�@S_OK�Ő���
		*/
		HRESULT InitTextureManager();
		/**
		* @fn
		* @brief �E�C���h�E�̏������I������
		*/
		void TermWindow();

		/**
		* @fn
		* @brief D3D12�V�X�e�����I��������
		*/
		void TermD3D();
	};
}

