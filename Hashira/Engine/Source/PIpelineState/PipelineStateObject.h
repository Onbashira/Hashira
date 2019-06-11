#pragma once
#include"Engine/Source/Utility/D3D12Common.h"
#include <memory>
#include <string>

namespace Hashira {

	class RootSignature;
	class D3D12Device;
	class PipelineStateObject
	{
	public:

	protected:

	private:
		//!�p�C�v���C���X�e�[�g
		Microsoft::WRL::ComPtr<ID3D12PipelineState> _pipelineState;

		//!���[�g�V�O�l�`��
		std::weak_ptr<RootSignature>				_rootSignature;

		//�e���O�̓��ɂ����O
		std::string									_name;

	public:

		PipelineStateObject();

		virtual ~PipelineStateObject();

		/**
		* @fn
		* @brief �p�C�v���C���X�e�[�g�̍쐬
		* @param[in] name PSO�̖��O
		* @param[in] gpsDesc PSO�̃f�X�N���v�V����
		* @param[in] shader hlsl�ɋL�q�������[�g�V�O�l�`�����o�C���h����Ă���V�F�[�_�[
		* @return ���U���g
		*/
		virtual HRESULT								Initialize(std::string name, D3D12_GRAPHICS_PIPELINE_STATE_DESC & gpsDesc, ID3DBlob* shader);

		/**
		* @fn
		* @brief �p�C�v���C���X�e�[�g�̍쐬
		* @param[in] device �쐬�Ɏg���f�o�C�X�̎Q��
		* @param[in] name PSO�̖��O
		* @param[in] gpsDesc PSO�̃f�X�N���v�V����
		* @param[in] shader hlsl�ɋL�q�������[�g�V�O�l�`�����o�C���h����Ă���V�F�[�_�[
		* @return ���U���g
		*/
		virtual HRESULT								Initialize(std::shared_ptr<D3D12Device>& device, std::string name, D3D12_GRAPHICS_PIPELINE_STATE_DESC & gpsDesc, ID3DBlob* shader);

		/**
		* @fn
		* @brief �p�C�v���C���X�e�[�g�̍쐬
		* @param[in] name PSO�̖��O
		* @param[in] cpsDesc PSO�̃f�X�N���v�V����
		* @param[in] shader hlsl�ɋL�q�������[�g�V�O�l�`�����o�C���h����Ă���V�F�[�_�[
		* @return ���U���g
		*/
		virtual HRESULT								Initialize(std::string name, D3D12_COMPUTE_PIPELINE_STATE_DESC &  cpsDesc, ID3DBlob* shader);

		/**
		* @fn
		* @brief �p�C�v���C���X�e�[�g�̍쐬
		* @param[in] device �쐬�Ɏg���f�o�C�X�̎Q��
		* @param[in] name PSO�̖��O
		* @param[in] cpsDesc PSO�̃f�X�N���v�V����
		* @param[in] shader hlsl�ɋL�q�������[�g�V�O�l�`�����o�C���h����Ă���V�F�[�_�[
		* @return ���U���g
		*/
		virtual HRESULT								Initialize(std::shared_ptr<D3D12Device>& device,std::string name, D3D12_COMPUTE_PIPELINE_STATE_DESC &  cpsDesc, ID3DBlob* shader);

		/**
		* @fn
		* @brief �p�C�v���C���X�e�[�g�̍쐬
		* @param[in] name PSO�̖��O
		* @param[in] gpsDesc PSO�̃f�X�N���v�V����
		* @param[in] rootSignature ���[�g�V�O�l�`��
		* @return ���U���g
		*/
		virtual HRESULT								Initialize(std::string name, D3D12_GRAPHICS_PIPELINE_STATE_DESC & gpsDesc, std::weak_ptr<RootSignature> rootSignature);

		/**
		* @fn
		* @brief �p�C�v���C���X�e�[�g�̍쐬
		* @param[in] device �쐬�Ɏg���f�o�C�X�̎Q��
		* @param[in] name PSO�̖��O
		* @param[in] gpsDesc PSO�̃f�X�N���v�V����
		* @param[in] rootSignature ���[�g�V�O�l�`��
		* @return ���U���g
		*/
		virtual HRESULT								Initialize(std::shared_ptr<D3D12Device>& device, std::string name, D3D12_GRAPHICS_PIPELINE_STATE_DESC & gpsDesc, std::weak_ptr<RootSignature> rootSignature);

		/**
		* @fn
		* @brief �p�C�v���C���X�e�[�g�̍쐬
		* @param[in] name PSO�̖��O
		* @param[in] cpsDesc PSO�̃f�X�N���v�V����
		* @param[in] rootSignature ���[�g�V�O�l�`��
		* @return ���U���g
		*/
		virtual HRESULT								Initialize(std::string name, D3D12_COMPUTE_PIPELINE_STATE_DESC &  cpsDesc, std::weak_ptr<RootSignature> rootSignature);

		/**
		* @fn
		* @brief �p�C�v���C���X�e�[�g�̍쐬
		* @param[in] device �쐬�Ɏg���f�o�C�X�̎Q��
		* @param[in] name PSO�̖��O
		* @param[in] cpsDesc PSO�̃f�X�N���v�V����
		* @param[in] rootSignature ���[�g�V�O�l�`��
		* @return ���U���g
		*/
		virtual HRESULT								Initialize(std::shared_ptr<D3D12Device>& device,std::string name, D3D12_COMPUTE_PIPELINE_STATE_DESC &  cpsDesc, std::weak_ptr<RootSignature> rootSignature);

		/**
		* @fn
		* @brief ���O�̐ݒ�
		* @param[in] name PSO�̖��O
		*/
		void										SetName(std::string name);

		/**
		* @fn
		* @brief ���[�g�V�O�l�`���ւ̎�Q�Ƃ̃t�F�b�`
		* @return �o�C���h����Ă��郋�[�g�V�O�l�`���ւ̎�Q��
		*/
		std::weak_ptr<RootSignature>				GetRootSignature();

		/**
		* @fn
		* @brief PSO�̃t�F�b�`
		* @return PSO�̎Q��
		*/
		Microsoft::WRL::ComPtr<ID3D12PipelineState>& GetPSO();

		/**
		* @fn
		* @brief �f�[�^�j��
		*/
		virtual void Discard();

	private:

	};
}
