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
		
		//!テクスチャリソースへの参照
		std::shared_ptr<ShaderResource> _textureResource;
		
		//!デスクリプション
		TextureObjectDesc _desc;
		
	public:

		TextureObject();

		/**
		* @fn
		* @brief テクスチャオブジェクトの作成
		* @param[in] sr シェーダリソース
		* @param[in] desc デスクリプション
		* @return リザルト
		*/
		TextureObject( std::shared_ptr<ShaderResource> sr,const TextureObjectDesc& desc);

		~TextureObject();

		/**
		* @fn
		* @brief デスクリプションのフェッチ
		* @return デスクリプション
		*/
		const TextureObjectDesc& GetDesc()const;

		/**
		* @fn
		* @brief デスクリプションのフェッチ
		* @return デスクリプション
		*/
		void SetDesc(TextureObjectDesc&& desc);

		/**
		* @fn
		* @brief シェーダーリソースバッファへの弱参照の取得
		* @return シェーダーリソースバッファへの弱参照
		*/
		std::weak_ptr<ShaderResource> GetShaderResource();
		
	private:
	
	};

}