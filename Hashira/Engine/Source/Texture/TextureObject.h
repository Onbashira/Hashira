#pragma once
#include <d3d12.h>
#include "Engine/Source/ThirdParty/DirectXTex/include/DirectXTex.h"

namespace Hashira {

	class ShaderResource;
	class TextureLoader;
	class D3D12Device;

	struct TextureObjectDesc {

		float gamma;
		std::string fileName;
		D3D12_SUBRESOURCE_DATA subResource;
		DirectX::TexMetadata metaData;
	};

	class TextureObject
	{
	public:
		friend class TextureLoader;
		friend class TextureManager;
	private:
		
		//!�e�N�X�`�����\�[�X�ւ̎Q��
		std::shared_ptr<ShaderResource> _textureResource;
		
		//!�f�X�N���v�V����
		TextureObjectDesc _desc;
		
	public:

		TextureObject();

		/**
		* @fn
		* @brief �e�N�X�`���I�u�W�F�N�g�̍쐬
		* @param[in] sr �V�F�[�_���\�[�X
		* @param[in] desc �f�X�N���v�V����
		* @return ���U���g
		*/
		TextureObject( std::shared_ptr<ShaderResource> sr,const TextureObjectDesc& desc);

		~TextureObject();

		/**
		* @fn
		* @brief �f�X�N���v�V�����̃t�F�b�`
		* @return �f�X�N���v�V����
		*/
		const TextureObjectDesc& GetDesc()const;

		/**
		* @fn
		* @brief �f�X�N���v�V�����̃t�F�b�`
		* @return �f�X�N���v�V����
		*/
		void SetDesc(TextureObjectDesc&& desc);

		/**
		* @fn
		* @brief �V�F�[�_�[���\�[�X�o�b�t�@�ւ̎�Q�Ƃ̎擾
		* @return �V�F�[�_�[���\�[�X�o�b�t�@�ւ̎�Q��
		*/
		std::weak_ptr<ShaderResource> GetShaderResource();
		
	private:
	
	};

}