#pragma once
#include"Engine/Source/Utility/D3D12Common.h"
#include"Engine/Source/Utility//NonCopyable.h"
#include"Engine/Source/CommandList/CommandListManager.h"
#include"Engine/Source/ShaderObject/ShaderObjectManager.h"
#include <vector>



namespace Hashira {
	class CommandQueue;
	class DepthStencil;
	class Fence;
	class PipelineStateObject;


	class GraphicsContextManager : private NonCopyable
	{
	private:

		//!�R�}���h���X�g�}�l�[�W��
		CommandListManager			_commandListLibrary;

		//!�V�F�[�_�[�I�u�W�F�N�g�}�l�[�W��
		ShaderObjectManager			_shaderObjectLibrary;

		////!�L���[�}�l�[�W��
		//CommandQueueManager         _queueManager;

	public:
	private:

		GraphicsContextManager();

	public:

		~GraphicsContextManager();

		/**
		* @fn
		* @brief �R�}���h���X�g�}�l�[�W�����̃R�}���h���X�g�����ׂĕ���
		*/
		void CloseAllCommandLists();

		/**
		* @fn
		* @brief �R�}���h���X�g������A���̃R�}���h���X�g���������A����
		* @param[in] commandListName �R�}���h���X�g���O
		*/
		void CloseCommandList(std::string commandListName);

		/**
		* @fn
		* @brief �R�}���h���X�g�}�l�[�W�����̃R�}���h���X�g�����ׂă��Z�b�g����
		*/
		void ResetAllCommandList();

		/**
		* @fn
		* @brief �R�}���h���X�g������A���̃R�}���h���X�g���������A����
		* @param[in] commandListName �R�}���h���X�g���O
		*/
		void ResetCommandList(std::string commandListName,std::shared_ptr<RenderContext>& renderContext);

		/**
		* @fn
		* @brief �񓯊��ŃR�}���h���X�g�����s����
		* @param[in] queue �R�}���h�L���[�ւ̃|�C���^
		* @param[in] commandListName �R�}���h���X�g���O
		* @param[in] fence �t�F���X
		* @return ������S_OK
		*/
		HRESULT ExecutionCommandListAsync(CommandQueue* queue, std::vector<std::string> commandListNames, Fence* fence = nullptr);

		/**
		* @fn
		* @brief ���ׂẴR�}���h���X�g�����s����
		* @param[in] queue �R�}���h�L���[�ւ̃|�C���^
		* @param[in] fence �t�F���X
		* @return ������S_OK
		*/
		HRESULT ExecutionAllCommandLists(CommandQueue* queue, Fence* fence = nullptr);

		/**
		* @fn
		* @brief �R�}���h���X�g���쐬����
		* @param[in] commandListName �R�}���h���X�g�̖��O
		* @param[in] device �쐬�p�f�o�C�X
		* @param[in] renderContext �R���e�L�X�g
		* @param[in] nodeMask �m�[�h�}�X�N
		* @param[in] listType ���X�g�^�C�v
		* @return ������S_OK
		*/
		HRESULT CreateCommandList(std::string commandListName, std::shared_ptr<RenderingDevice>& device, std::shared_ptr<RenderContext>& renderContext, unsigned int nodeMask, D3D12_COMMAND_LIST_TYPE listType);

		/**
		* @fn
		* @brief �p�C�v���C���X�e�[�g���쐬
		* @param[in] psoName �p�C�v���C���X�e�[�g�̖��O
		* @param[in] gps �O���t�B�N�X�p�C�v���C���X�e�[�g�̃f�X�N���v�V����
		* @param[in] rootSignature ���[�g�V�O�l�`��
		* @return ������S_OK
		*/
		HRESULT CreatePSO(std::string psoName, D3D12_GRAPHICS_PIPELINE_STATE_DESC& gps, ID3DBlob* rootSignature = nullptr);

		/**
		* @fn
		* @brief �p�C�v���C���X�e�[�g���쐬
		* @param[in] device �f�o�C�X
		* @param[in] psoName �p�C�v���C���X�e�[�g�̖��O
		* @param[in] gps �O���t�B�N�X�p�C�v���C���X�e�[�g�̃f�X�N���v�V����
		* @param[in] rootSignature ���[�g�V�O�l�`��
		* @return ������S_OK
		*/
		HRESULT CreatePSO(std::shared_ptr<D3D12Device>& device,std::string psoName, D3D12_GRAPHICS_PIPELINE_STATE_DESC& gps, ID3DBlob * rootSignature = nullptr);

		/**
		* @fn
		* @brief �p�C�v���C���X�e�[�g���쐬
		* @param[in] psoName �p�C�v���C���X�e�[�g�̖��O
		* @param[in] cps �R���s���[�g�p�C�v���C���X�e�[�g�̃f�X�N���v�V����
		* @param[in] rootSignature ���[�g�V�O�l�`��
		* @return ������S_OK
		*/
		HRESULT CreatePSO(std::string psoName, D3D12_COMPUTE_PIPELINE_STATE_DESC& gps, ID3DBlob* rootSignature = nullptr);

		/**
		* @fn
		* @brief �p�C�v���C���X�e�[�g���쐬
		* @param[in] device �f�o�C�X
		* @param[in] psoName �p�C�v���C���X�e�[�g�̖��O
		* @param[in] cps �R���s���[�g�p�C�v���C���X�e�[�g�̃f�X�N���v�V����
		* @param[in] rootSignature ���[�g�V�O�l�`��
		* @return ������S_OK
		*/
		HRESULT CreatePSO(std::shared_ptr<D3D12Device>& device, std::string psoName, D3D12_COMPUTE_PIPELINE_STATE_DESC& gps, ID3DBlob * rootSignature = nullptr);

		///**
		//* @fn
		//* @brief �R�}���h�L���[�쐬
		//* @param[in] queueName �L���[�̖��O
		//* @param[in] desc �L���[�̃f�X�N���v�V����
		//* @return ������S_OK
		//*/
		//HRESULT CreateCommandQueue(std::string queueName, D3D12_COMMAND_QUEUE_DESC& desc);

		/**
		* @fn
		* @brief ���[�g�V�O�l�`���쐬
		* @param[in] rsName ���[�g�V�O�l�`���̖��O
		* @param[in] signature ���[�g�V�O�l�`���̃f�[�^�|�C���^
		* @return ������S_OK
		*/
		HRESULT CreateRootSignature(std::string rsName, ID3DBlob* signature);

		/**
		* @fn
		* @brief ���[�g�V�O�l�`���쐬
		* @param[in] rsName ���[�g�V�O�l�`���̖��O
		* @param[in] signature ���[�g�V�O�l�`���̃f�X�N���v�V����
		* @return ������S_OK
		*/
		HRESULT CreateRootSignature(std::string rsName, D3D12_ROOT_SIGNATURE_DESC* signature);

		/**
		* @fn
		* @brief �R�}���h���X�g�̃Z�b�g
		* @param[in] commandListName �R�}���h���X�g��
		* @param[in] commandList �R�}���h���X�g�̎Q��
		*/
		void SetCommandList(std::string commandListName, std::shared_ptr<CommandList> commandList);

		/**
		* @fn
		* @brief �p�C�v���C���X�e�[�g�̃Z�b�g
		* @param[in] psoName �p�C�v���C���X�e�[�g��
		* @param[in] pso �p�C�v���C���X�e�[�g�̎Q��
		*/
		void SetPSO(std::string psoName, std::shared_ptr<PipelineStateObject> pso);

		/**
		* @fn
		* @brief ���[�g�V�O�l�`���̃Z�b�g
		* @param[in] rsName ���[�g�V�O�l�`����
		* @param[in] rootSignature ���[�g�V�O�l�`���̎Q��
		*/
		void SetRootSignature(std::string rsName, std::shared_ptr<RootSignature> rootSignature);

		/**
		* @fn
		* @brief �R�}���h���X�g�̃Z�b�g
		* @param[in] commandListName �R�}���h���X�g��
		* @return �R�}���h���X�g�ւ̎Q��
		*/
		std::shared_ptr<CommandList> GetCommandList(std::string commandListName);

		/**
		* @fn
		* @brief �p�C�v���C���X�X�e�[�g�̃Z�b�g
		* @param[in] psoName �p�C�v���C���X�X�e�[�g��
		* @return �p�C�v���C���X�X�e�[�g�Q��
		*/
		std::shared_ptr<PipelineStateObject> GetPSO(std::string psoName);

		///**
		//* @fn
		//* @brief �L���[�̃Z�b�g
		//* @param[in] queueName �L���[��
		//* @return �L���[�Q��
		//*/
		//std::shared_ptr<CommandQueue> GetCommandQueue(std::string queueName);

		/**
		* @fn
		* @brief ���[�g�V�O�l�`���̃Z�b�g
		* @param[in] rsName ���[�g�V�O�l�`����
		* @return ���[�g�V�O�l�`���Q��
		*/
		std::shared_ptr<RootSignature> GetRootSignature(std::string rsName);

		/**
		* @fn
		* @brief �R�}���h���X�g�̍폜
		* @param[in] commandListName ���[�g�V�O�l�`����
		*/
		void EraseCommandList(std::string commandListName);

		/**
		* @fn
		* @brief �p�C�v���C���X�X�e�[�g�̍폜
		* @param[in] psoName �p�C�v���C���X�X�e�[�g��
		*/
		void ErasePSO(std::string psoName);

		/**
		* @fn
		* @brief ���[�g�V�O�l�`���̍폜
		* @param[in] psoName ���[�g�V�O�l�`����
		*/
		void EraseRootSignature(std::string psoName);

		///**
		//* @fn
		//* @brief �L���[�̍폜
		//* @param[in] queueName �L���[��
		//*/
		//void EraseCommandQueue(std::string queueName);

		/**
		* @fn
		* @brief ���ׂẴL�����A�[���ێ����Ă���f�[�^�̔j��
		*/
		void Discard();

		static GraphicsContextManager& GetInstance();
	};
};