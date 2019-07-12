#pragma once
#include "Engine/Source/Math/Math.h"
#include "Engine/Source/Component/GameObject/GameObject.h"
#include "Engine/Source/Buffer/UploadBuffer.h"
#include "Engine/Source/Buffer/DepthStencilBuffer.h"

namespace Hashira {

	class CommandList;
	
	//!�J�����̃��[�h
	enum class CAMERA_MODE {
		Perspective,
		Orthogonal
	};

	//!�V�F�[�_�[�Ƀo�C���h����J�����̏��
	struct CameraInfo
	{
		Matrix view;
		Matrix projection;
		Matrix invView;
		Matrix invViewProj;
		float  windowWidth;
		float  windowHeight;
	};

	class Camera :
		public GameObject
	{

	public:

	private:
		//�I���[�h
		CAMERA_MODE _mode;

		//!Field�@Of�@View
		float _fov;

		//!�A�X��
		float _aspectRatio;
		//!�j�A�N���b�v
		float _near;
		
		//!�t�@�[�N���b�v
		float _far;

		//!�E�C���h�E��
		float _windowWidth;
		
		//!�E�C���h�E����
		float _windowHeight;

		//!�r���[�|�[�g
		D3D12_VIEWPORT	_viewport;

		//!�؂蔲����`
		D3D12_RECT _scissorRect;

		//!�ˉe�s��
		Matrix _projection;
		//!�J�����C���t�H���[�V�����o�b�t�@
		UploadBuffer<CameraInfo > _cameraMatrixBuffer;
		
		//!�f�v�X�X�e���V���o�b�t�@
		DepthStencil _depthStencillRersource;

		//!�J�����̏��
		CameraInfo _info;

	public:

		Camera();

		~Camera();

		/**
		* @fn
		* @brief �J�����̃��[�h���擾
		* @return ���[�h
		*/
		CAMERA_MODE		GetMode();

		/**
		* @fn
		* @brief �ˉe�s��̎擾
		* @return �ˉe�s��
		*/
		const Matrix&	GetProjection();

		/**
		* @fn
		* @brief �ˉe�s��̎擾
		* @return �ˉe�s��
		*/
		const Matrix	GetViewProjection();

		/**
		* @fn
		* @brief �J�����̏��̎擾
		* @return �J�����̏��
		*/
		CameraInfo		GetCameraInfo();

		/**
		* @fn
		* @brief �J�����o�b�t�@�̎擾
		* @return �J�����o�b�t�@�ւ̎Q��
		*/
		Hashira::UploadBuffer<Hashira::CameraInfo>&  GetCameraBuffer();

		/**
		* @fn
		* @brief �f�v�X�X�e���V���o�b�t�@�̎擾
		* @return �f�v�X�X�e���V���o�b�t�@�ւ̎Q��
		*/
		DepthStencil&	GetDepthStencil();

		/**
		* @fn
		* @brief FOV�̎擾
		* @return FOV
		*/
		float GetFov();

		/**
		* @fn
		* @brief �A�X�y�N�g��̎擾
		* @return �A�X�y�N�g��
		*/
		float GetAspectratio;

		/**
		* @fn
		* @brief �j�A�̎擾
		* @return �j�A
		*/
		float GetNearClip();

		/**
		* @fn
		* @brief �t�@�[�̎擾
		* @return �t�@�[
		*/
		float GetFarClip();

		/**
		* @fn
		* @brief �r���[�|�[�g�̎擾
		* @return �r���[�|�[�g�ւ̎Q��
		*/
		const D3D12_VIEWPORT& GetViewport()const;

		/**
		* @fn
		* @brief �؂蔲����`�̎擾
		* @return �؂蔲����`�ւ̎Q��
		*/
		const D3D12_RECT& GetScissorRect()const;

		/**
		* @fn
		* @brief �o�b�t�@�y�я��̍X�V
		*/
		void Update()override;

		/**
		* @fn
		* @brief �_�~�[�`��
		*/
		void Draw(std::shared_ptr<CommandList>& cmdList)override {};

		/**
		* @fn
		* @brief ���͂ɂ��ړ�
		* @oaram[in] input�@����
		*/
		void DebugMove(InputManager& input);

		/**
		* @fn
		* @brief ���͂ɂ���]
		* @oaram[in] input�@����
		*/
		void DebugRotate(InputManager& input);

		/**
		* @fn
		* @brief ���[�h�̕ύX
		* @oaram[in] mode�@���[�h
		*/
		void ChangeCameraMode(CAMERA_MODE mode);

		/**
		* @fn
		* @brief �J�����̏�����
		* @oaram[in] type�@���[�h
		* @oaram[in] width�@�J�����̉�ʕ�
		* @oaram[in] height�@�J�����̉�ʍ���
		* @oaram[in] nearClip�@�j�A
		* @oaram[in] farClip�@�t�@�[
		* @oaram[in] position�@�ʒu
		* @oaram[in] target�@�^�[�Q�b�g�ʒu
		* @oaram[in] upWard�@�A�b�v�x�N�g��
		*/
		void InitializeCamera(CAMERA_MODE type, const float width, const float height, const float nearClip, const float farClip, const Vector3& position, const Vector3& target, const Vector3& upWard);

		/**
		* @fn
		* @brief FOV�x�[�X�ŃJ�����̏�����
		* @oaram[in] fov�@FOV
		* @oaram[in] width�@�J�����̉�ʕ�
		* @oaram[in] height�@�J�����̉�ʍ���
		* @oaram[in] nearClip�@�j�A
		* @oaram[in] farClip�@�t�@�[
		* @oaram[in] position�@�ʒu
		* @oaram[in] target�@�^�[�Q�b�g�ʒu
		* @oaram[in] upWard�@�A�b�v�x�N�g��
		*/
		void InitializeCameraFOV(const float fov, const float width, const float height, const float nearClip, const float farClip, const Vector3& position, const Vector3& target, const Vector3& upWard);

		/**
		* @fn
		* @brief �f�v�X�X�e���V���o�b�t�@�̏�����
		* @oaram[in] depthFormat�@�t�H�[�}�b�g
		* @oaram[in] windowWidth�@�J�����̉�ʕ�
		* @oaram[in] windowHeight�@�J�����̉�ʍ���
		* @return ���U���g S_OK�Ő���
		*/
		HRESULT InitializeCameraDepthStencill(std::shared_ptr<D3D12Device>& device,DXGI_FORMAT depthFormat, unsigned int windowWidth, unsigned int windowHeight);

		/**
		* @fn
		* @brief �R�}���h���X�g�ɃJ���������Z�b�g
		* @oaram[in] list�@�Z�b�g��
		* @oaram[in] parameterIndex�@�p�����[�^�̃C���f�b�N�X
		*/
		void SetCameraParameter(std::weak_ptr<CommandList> list, unsigned int parameterIndex = 0);

		/**
		* @fn
		* @brief �j��
		*/
		void Discard();

	private:

		/**
		* @fn
		* @brief �e�o�b�t�@�̍쐬
		* @return ���U���g S_OK�Ő���
		*/
		HRESULT CreateBuffer();

		/**
		* @fn
		* @brief ���ˉe�J�����̏�����
		* @oaram[in] fov�@FOV
		* @oaram[in] width�@�J�����̉�ʕ�
		* @oaram[in] height�@�J�����̉�ʍ���
		* @oaram[in] nearClip�@�j�A
		* @oaram[in] farClip�@�t�@�[
		* @oaram[in] position�@�ʒu
		* @oaram[in] target�@�^�[�Q�b�g�ʒu
		* @oaram[in] upWard�@�A�b�v�x�N�g��
		* @return ���U���g S_OK�Ő���
		*/

		HRESULT InitializeOrthogonal(const float width, const float height, const float nearClip, const float farClip, const Vector3& position, const Vector3& target, const Vector3& upWard);
		
		/**
		* @fn
		* @brief �ˉe�J�����̏�����
		* @oaram[in] fov�@FOV
		* @oaram[in] width�@�J�����̉�ʕ�
		* @oaram[in] height�@�J�����̉�ʍ���
		* @oaram[in] nearClip�@�j�A
		* @oaram[in] farClip�@�t�@�[
		* @oaram[in] position�@�ʒu
		* @oaram[in] target�@�^�[�Q�b�g�ʒu
		* @oaram[in] upWard�@�A�b�v�x�N�g��
		* @return ���U���g S_OK�Ő���
		*/
		HRESULT initializePerspective(const float width, const float height, const float nearClip, const float farClip, const Vector3& position, const Vector3& target, const Vector3& upWard);

	};

}