#pragma once
#include"Engine/Source/Utility/D3D12Common.h"
#include <string>

namespace Hashira {

	class D3D12Device;

	class DescriptorHeap
	{

	public:

	private:

		//�ICPU�A�h���X�J�n�n�_
		D3D12_CPU_DESCRIPTOR_HANDLE						_cpuHandle;

		//�ICPU�A�h���X�J�n�n�_
		D3D12_GPU_DESCRIPTOR_HANDLE						_gpuHandle;

		//!�^�C�v
		UINT											_type;

		//�I�C���N�������g�T�C�Y
		UINT											_incrementSize;

		//�I�q�[�v�f�X�N���v�V����
		D3D12_DESCRIPTOR_HEAP_DESC						_heapDesc;

		//!�q�[�v
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>	_heap;

		//�I�q�[�v��
		std::string										_heapName;

	public:
		DescriptorHeap();

		~DescriptorHeap();

		/**
		* @fn
		* @brief CPU�n���h���̃t�F�b�`
		* @param[in] index �r���[�C���f�b�N�X
		* @return CPU�n���h��
		*/
		D3D12_CPU_DESCRIPTOR_HANDLE						GetCPUHandle(UINT indexCount)const;

		/**
		* @fn
		* @brief GPU�n���h���̃t�F�b�`
		* @param[in] index �r���[�C���f�b�N�X
		* @return GPU�n���h��
		*/
		D3D12_GPU_DESCRIPTOR_HANDLE						GetGPUHandle(UINT indexCount)const;

		/**
		* @fn
		* @brief �C���N�������g�T�C�Y�̃t�F�b�`
		* @return �q�[�v�C���N�������g�T�C�Y
		*/
		UINT											GetIncrementSize()const;

		/**
		* @fn
		* @brief �q�[�v�̃t�F�b�`
		* @return �q�[�v�̎Q��
		*/
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>&	GetHeap();

		/**
		* @fn
		* @brief �q�[�v�|�C���^�̃t�F�b�`
		* @return �q�[�v�̃|�C���^
		*/
		ID3D12DescriptorHeap* 							GetPtr()const;

		/**
		* @fn
		* @brief ���̃t�F�b�`
		* @return ���
		*/
		const D3D12_DESCRIPTOR_HEAP_DESC&				GetDesc()const;

		/**
		* @fn
		* @brief �q�[�v�|�C���^�̃t�F�b�`
		* @return �q�[�v�̃|�C���^
		*/
		ID3D12DescriptorHeap*							operator ->();

		/**
		* @fn
		* @brief �q�[�v�̍쐬
		* @param[in] pDesc �f�X�N���v�V�����ւ̃|�C���^
		* @return ���U���g�@S_OK�Ő���
		*/
		HRESULT											Initialize(D3D12_DESCRIPTOR_HEAP_DESC* pDesc);

		/**
		* @fn
		* @brief �q�[�v�̍쐬
		* @param[in] device �쐬�Ɏg�p����f�o�C�X
		* @param[in] pDesc �f�X�N���v�V�����ւ̃|�C���^
		* @return ���U���g�@S_OK�Ő���
		*/
		HRESULT											Initialize(std::shared_ptr<D3D12Device>& device,D3D12_DESCRIPTOR_HEAP_DESC* pDesc);

		/**
		* @fn
		* @brief �q�[�v�̍쐬
		* @param[in] type �q�[�v�̃^�C�v
		* @param[in] numDescriptors �f�X�N���v�^�̐�
		* @param[in] nodeMask �m�[�h�}�X�N
		* @return ���U���g�@S_OK�Ő���
		*/
		HRESULT											Initialize(D3D12_DESCRIPTOR_HEAP_TYPE type, unsigned int numDescriptors, unsigned int nodeMask = 0);

		/**
		* @fn
		* @brief �q�[�v�̍쐬
		* @param[in] device �쐬�Ɏg�p����f�o�C�X
		* @param[in] type �q�[�v�̃^�C�v
		* @param[in] numDescriptors �f�X�N���v�^�̐�
		* @param[in] nodeMask �m�[�h�}�X�N
		* @return ���U���g�@S_OK�Ő���
		*/
		HRESULT											Initialize(std::shared_ptr<D3D12Device>& device, D3D12_DESCRIPTOR_HEAP_TYPE type, unsigned int numDescriptors, unsigned int nodeMask = 0);

		/**
		* @fn
		* @brief �q�[�v�̖��O�̐ݒ�
		* @param[in] name ���O
		* @return ���U���g�@S_OK�Ő���
		*/
		void											SetName(std::string name);

		/**
		* @fn
		* @brief �q�[�v�̔j��
		*/
		void											Discard();

	private:
	};

}
