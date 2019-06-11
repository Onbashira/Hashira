#pragma once
#include <d3d12.h>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace Hashira {

	class PipelineStateObject;
	class RootSignature;
	class D3D12Device;

	class ShaderObjectManager
	{
	private:
		
		//!PSO�L�����A
		std::map <std::string, std::shared_ptr<Hashira::PipelineStateObject>> _psolibrary;
		
		//!RS�L�����A
		std::map <std::string, std::shared_ptr<Hashira::RootSignature>> _rootSignaturelibrary;
	
	public:
	private:
	public:

		ShaderObjectManager();
		
		~ShaderObjectManager();

		/**
		* @fn
		* @brief �p�C�v���C���X�e�[�g���쐬
		* @param[in] psoName �p�C�v���C���X�e�[�g�̖��O
		* @param[in] gps �O���t�B�N�X�p�C�v���C���X�e�[�g�̃f�X�N���v�V����
		* @param[in] rootSignature ���[�g�V�O�l�`��
		* @return ������S_OK
		*/
		HRESULT CreatePSO(std::string psoName, D3D12_GRAPHICS_PIPELINE_STATE_DESC& gps, ID3DBlob* signature = nullptr);
		
		/**
		* @fn
		* @brief �p�C�v���C���X�e�[�g���쐬
		* @param[in] device �f�o�C�X
		* @param[in] psoName �p�C�v���C���X�e�[�g�̖��O
		* @param[in] gps �O���t�B�N�X�p�C�v���C���X�e�[�g�̃f�X�N���v�V����
		* @param[in] rootSignature ���[�g�V�O�l�`��
		* @return ������S_OK
		*/
		HRESULT CreatePSO(std::shared_ptr<D3D12Device>& device ,std::string psoName, D3D12_GRAPHICS_PIPELINE_STATE_DESC& gps, ID3DBlob* signature = nullptr);

		/**
		* @fn
		* @brief �p�C�v���C���X�e�[�g���쐬
		* @param[in] psoName �p�C�v���C���X�e�[�g�̖��O
		* @param[in] cps �R���s���[�g�p�C�v���C���X�e�[�g�̃f�X�N���v�V����
		* @param[in] rootSignature ���[�g�V�O�l�`��
		* @return ������S_OK
		*/
		HRESULT CreatePSO(std::string psoName, D3D12_COMPUTE_PIPELINE_STATE_DESC& cps, ID3DBlob* signature = nullptr);
		
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
		HRESULT CreateRootSignature(std::string rsName,D3D12_ROOT_SIGNATURE_DESC * desc);

		/**
		* @fn
		* @brief �p�C�v���C���X�e�[�g�̃Z�b�g
		* @param[in] psoName �p�C�v���C���X�e�[�g��
		* @param[in] pso �p�C�v���C���X�e�[�g�̎Q��
		*/
		void SetPSO(std::string psoName, std::shared_ptr<Hashira::PipelineStateObject> pso);
		
		/**
		* @fn
		* @brief ���[�g�V�O�l�`���̃Z�b�g
		* @param[in] rsName ���[�g�V�O�l�`����
		* @param[in] rootSignature ���[�g�V�O�l�`���̎Q��
		*/
		void SetRootSignature(std::string rsName, std::shared_ptr<RootSignature> rootSignature);

		/**
		* @fn
		* @brief �p�C�v���C���X�X�e�[�g�̃Z�b�g
		* @param[in] psoName �p�C�v���C���X�X�e�[�g��
		* @return �p�C�v���C���X�X�e�[�g�Q��
		*/
		std::weak_ptr<Hashira::PipelineStateObject> GetPSO(std::string psoName);
		
		/**
		* @fn
		* @brief ���[�g�V�O�l�`���̃Z�b�g
		* @param[in] rsName ���[�g�V�O�l�`����
		* @return ���[�g�V�O�l�`���Q��
		*/
		std::weak_ptr<Hashira::RootSignature> GetRootSignature(std::string rsName);

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

		/**
		* @fn
		* @brief ���ׂẴL�����A�[���ێ����Ă���f�[�^�̔j��
		*/
		void Discard();

	};
}