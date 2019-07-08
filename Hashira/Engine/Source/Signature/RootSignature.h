#pragma once
#include "Engine/Source/Utility/D3D12Common.h"
#include <string>

namespace Hashira {

	class D3D12Device;

	struct ShaderVisibility
	{
		enum Type
		{
			Vertex = 0x01 << 0,
			Pixel = 0x01 <<1,
			Geometry = 0x01 <<2,
			Domain = 0x01 << 3,
			Hull = 0x01 <<4,
			Compute = 0x01 << 5,

			All = Vertex | Pixel | Geometry | Domain | Hull | Compute

		};
	};

	struct RootParameterType
	{
		enum Type
		{
			ConstantBuffer,
			ShaderResource,
			UnorderedAccess,
			Sampler,
			Max
		};
	};

	struct RootParameter
	{
		RootParameterType::Type type;
		Uint32 shaderVisibility;
		Uint32 registerIndex;

		RootParameter(RootParameterType::Type type = RootParameterType::ConstantBuffer, Uint32 shaderVisibility = ShaderVisibility::All
			, Uint32 registerIndex = 0):
			type(type),
			shaderVisibility(shaderVisibility),
			registerIndex(registerIndex)
		{
		}

	};

	struct RootSignatureDesc
	{
		Uint32 numParameters = 0;
		const RootParameter*  pParameters = nullptr;
		D3D12_ROOT_SIGNATURE_FLAGS flags = D3D12_ROOT_SIGNATURE_FLAGS::D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	};

	class RootSignature
	{
	public:

		struct InputIndex {
			struct VS {
				Uint8 CbvIndex;
				Uint8 SrvIndex;
				Uint8 SamplerIndex;
			}vs;
			struct PS {
				Uint8 CbvIndex;
				Uint8 SrvIndex;
				Uint8 SamplerIndex;
				Uint8 UavIndex;
			}ps;
			struct GS {
				Uint8 CbvIndex;
				Uint8 SrvIndex;
				Uint8 SamplerIndex;
			}gs;
			struct HS {
				Uint8 CbvIndex;
				Uint8 SrvIndex;
				Uint8 SamplerIndex;
			}hs;
			struct DS {
				Uint8 CbvIndex;
				Uint8 SrvIndex;
				Uint8 SamplerIndex;
			}ds;
			struct CS {
				Uint8 CbvIndex;
				Uint8 SrvIndex;
				Uint8 SamplerIndex;
				Uint8 UavIndex;
			}cs;

		};

	private:

		//!���[�g�V�O�l�`��
		Microsoft::WRL::ComPtr<ID3D12RootSignature> _rootSignature;

		InputIndex _inputIndex;

		//!���O
		std::string	_name;

	public:

		RootSignature();

		~RootSignature();

		/**
		* @fn
		* @brief ���[�g�V�O�l�`���̍쐬
		* @param[in] shader �o�C���h���ꂽ�V�F�[�_�[
		* @return ���U���g
		*/
		HRESULT InitializeFromShader(ID3DBlob* shader);

		/**
		* @fn
		* @brief ���[�g�V�O�l�`���̍쐬
		* @param[in] desc �f�X�N���v�V����
		* @return ���U���g
		*/
		HRESULT InitializeFromDesc(D3D12_ROOT_SIGNATURE_DESC* desc);

		/**
		* @fn
		* @brief ���[�g�V�O�l�`���̍쐬
		* @param[in] device �f�o�C�X
		* @param[in] desc �f�X�N���v�V����
		* @return ���U���g
		*/
		HRESULT InitializeFromDesc(std::shared_ptr<D3D12Device>& device,RootSignatureDesc& desc);

		/**
		* @fn
		* @brief ���[�g�V�O�l�`���̃t�F�b�`
		* @return ���[�g�V�O�l�`���ւ̎Q��
		*/
		Microsoft::WRL::ComPtr<ID3D12RootSignature>& GetSignature();

		const Hashira::RootSignature::InputIndex& GetInputIndex()
		{
			return _inputIndex;
		}

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