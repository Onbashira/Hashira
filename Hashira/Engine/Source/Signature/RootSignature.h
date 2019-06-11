#pragma once
#include "Engine/Source/Utility/D3D12Common.h"
#include <string>

namespace Hashira {

	class RootSignature
	{
	public:

	private:

		//!���[�g�V�O�l�`��
		Microsoft::WRL::ComPtr<ID3D12RootSignature> _rootSignature;

		//!���O
		std::string									_name;

	public:

		RootSignature();

		~RootSignature();

		/**
		* @fn
		* @brief ���[�g�V�O�l�`���̍쐬
		* @param[in] shader �o�C���h���ꂽ�V�F�[�_�[
		* @return ���U���g
		*/
		HRESULT CreateFromShader(ID3DBlob* shader);

		/**
		* @fn
		* @brief ���[�g�V�O�l�`���̍쐬
		* @param[in] desc �f�X�N���v�V����
		* @return ���U���g
		*/
		HRESULT CreateFromDesc(D3D12_ROOT_SIGNATURE_DESC* desc);

		/**
		* @fn
		* @brief ���[�g�V�O�l�`���̃t�F�b�`
		* @return ���[�g�V�O�l�`���ւ̎Q��
		*/
		Microsoft::WRL::ComPtr<ID3D12RootSignature>& GetSignature();

		/**
		* @fn
		* @brief ���O�̐ݒ�
		* @param[in] name ���O
		*/
		void SetName(std::string name);

		/**
		* @fn
		* @brief�@�j��
		*/
		void Discard();

	private:

	};
}