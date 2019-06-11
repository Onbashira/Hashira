#pragma once
#include "Engine/Source/Async/Fence.h"
#include <string>
namespace Hashira {

	class D3D12Device;
	class CommandList;

	class CommandQueue
	{


	public:
	private:

		//!3D�L���[
		Microsoft::WRL::ComPtr<ID3D12CommandQueue>	_3DQueue;

		//!�R�s�[�L���[
		Microsoft::WRL::ComPtr<ID3D12CommandQueue>	_copyQueue;

		//!�R���s���[�g�L���[
		Microsoft::WRL::ComPtr<ID3D12CommandQueue>	_computeQueue;
				
		//!�L���[�̖��O
		std::string _name;

	public:

		CommandQueue();

		~CommandQueue();

		/**
		* @fn
		* @brief �L���[�쐬
		* @param[in] desc �L���[�̃f�X�N���v�V����
		* @return ���U���g�@S_OK�Ő���
		*/
		HRESULT Initialize(D3D12_COMMAND_QUEUE_DESC& desc);
		
		/**
		* @fn
		* @brief �L���[�쐬
		* @param[in] device �f�o�C�X
		* @param[in] desc �L���[�̃f�X�N���v�V����
		* @return ���U���g�@S_OK�Ő���
		*/
		HRESULT Initialize(std::weak_ptr<D3D12Device> device,D3D12_COMMAND_QUEUE_DESC& desc);

		/**
		* @fn
		* @brief �L���[�̃t�F�b�`
		* @return �L���[�ւ̎Q��
		*/
		Microsoft::WRL::ComPtr<ID3D12CommandQueue>& GetQueue();

		/**
		* @fn
		* @brief COPY�L���[�̃t�F�b�`
		* @return �L���[�ւ̎Q��
		*/
		Microsoft::WRL::ComPtr<ID3D12CommandQueue>& GetCopyQueue();
		
		/**
		* @fn
		* @brief COMPUTE�L���[�̃t�F�b�`
		* @return �L���[�ւ̎Q��
		*/
		Microsoft::WRL::ComPtr<ID3D12CommandQueue>& GetComputeQueue();

		/**
		* @fn
		* @brief �^�C���X�^���v�̎擾
		* @return �^�C���X�^���v
		*/
		UINT64 GetTimestampFrequency();

		/**
		* @fn
		* @brief �L���[�̃f�X�N���v�V�����̎擾
		* @return �f�X�N���v�V����
		*/
		D3D12_COMMAND_QUEUE_DESC GetDesc();

		/**
		* @fn
		* @brief �L���[�̃f�X�N���v�V�����̎擾
		* @param[in] lists�@�R�}���h���X�g�x�N�^�ւ̎Q��
		* @return �f�X�N���v�V����
		*/
		void ExecuteCommandLists(std::vector<std::shared_ptr<CommandList>>& lists);

		/**
		* @fn
		* @brief �L���[�̃f�X�N���v�V�����̎擾
		* @param[in] lists�@�R�}���h���X�g�x�N�^�ւ̎Q��
		* @return �f�X�N���v�V����
		*/
		void ExecuteCopyCommands(std::vector<std::shared_ptr<CommandList>>& lists);

		/**
		* @fn
		* @brief �L���[�̃f�X�N���v�V�����̎擾
		* @param[in] lists�@�R�}���h���X�g�x�N�^�ւ̎Q��
		* @return �f�X�N���v�V����
		*/
		void ExecuteComputeCommands(std::vector<std::shared_ptr<CommandList>>& lists);

		/**
		* @fn
		* @brief �L���[�̖��O�̐ݒ�
		* @param[in] name ���O
		*/
		void SetName(std::string name);

		/**
		* @fn
		* @brief �j��
		*/
		void Discard();

	private:

		void Execute(std::vector<std::shared_ptr<CommandList>>& lists, Microsoft::WRL::ComPtr<ID3D12CommandQueue>& queue);

	};
}

