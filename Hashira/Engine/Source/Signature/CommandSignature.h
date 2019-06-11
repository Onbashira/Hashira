#pragma once
#include <string>
#include "Engine/Source/Utility/D3D12Common.h"

namespace Hashira {

	class RootSignature;
	class D3D12Device;

	class CommandSignature
	{
	public:

	private:

		//!�R�}���h�V�O�l�`��
		Microsoft::WRL::ComPtr<ID3D12CommandSignature>	 _commandSignature;

		//!���O
		std::string										 _name;

	public:

		CommandSignature();

		~CommandSignature();

		/**
		* @fn
		* @brief �R�}���h�V�O�l�`���̍쐬
		* @param[in] desc �p�C�v���C���X�X�e�[�g��
		* @param[in] rs �o�C���h��̃��[�g�V�O�l�`��
		* @return ���U���g
		*/
		HRESULT	Initialize(const D3D12_COMMAND_SIGNATURE_DESC& desc, std::weak_ptr<Hashira::RootSignature> rs);

		/**
		* @fn
		* @brief �R�}���h�V�O�l�`���̍쐬
		* @param[in] device �쐬�Ɏg�p����f�o�C�X
		* @param[in] desc �p�C�v���C���X�X�e�[�g��
		* @param[in] rs �o�C���h��̃��[�g�V�O�l�`��
		* @return ���U���g
		*/
		HRESULT	Initialize(std::shared_ptr<D3D12Device>& device,const D3D12_COMMAND_SIGNATURE_DESC& desc, std::weak_ptr<Hashira::RootSignature> rs);

		/**
		* @fn
		* @brief �R�}���h�V�O�l�`���̍쐬
		* @param[in] desc �p�C�v���C���X�X�e�[�g��
		* @return ���U���g
		*/
		HRESULT	Initialize(const D3D12_COMMAND_SIGNATURE_DESC& desc);

		/**
		* @fn
		* @brief �R�}���h�V�O�l�`���̍쐬
		* @param[in] device �쐬�Ɏg�p����f�o�C�X
		* @param[in] desc �p�C�v���C���X�X�e�[�g��
		* @return ���U���g
		*/
		HRESULT	Initialize(std::shared_ptr<D3D12Device>& device,const D3D12_COMMAND_SIGNATURE_DESC& desc);

		/**
		* @fn
		* @brief �R�}���h�V�O�l�`���̃t�F�b�`
		* @return �R�}���h�V�O�l�`��
		*/
		Microsoft::WRL::ComPtr<ID3D12CommandSignature>& GetSignature();

		/**
		* @fn
		* @brief �R�}���h�V�O�l�`���̃t�F�b�`
		* @param[in] name ���O
		*/
		void	SetName(std::string name);

		/**
		* @fn
		* @brief �j��
		*/
		void	Discard();

	private:
	};
}
