#pragma once
#include "Engine/Source/DescriptorHeap/DescriptorHeap.h"
#include <vector>
#include <string>
#include "Engine/ThirdParty/DirectXTex/include/DirectXTex.h"

namespace Hashira {

	class ShaderResource;
	class CommandList;
	class CommandQueue;
	class D3D12Device;
	class TextureObject;
	class RenderingDevice;
	class RenderContext;
	
	class TextureLoader
	{
	
		friend class TextureManager;

	public:

	private:

	public:

		static TextureLoader& GetInstance() {
			static TextureLoader instance;
			return instance;
		};
			~TextureLoader();

		/**
		* @fn
		* @brief モデルのテクスチャのロード
		* @param[in] commandList リストの参照
		* @param[in] renderContext レンダリング管理オブジェクトの参照
		* @param[in] filePath ファイルパス
		*/
		std::shared_ptr<TextureObject> LoadTextureResource(std::shared_ptr<RenderContext>& renderContex, std::string filePath);

	private:

		TextureLoader();

		/**
		* @fn
		* @brief UpdateSubresourceを用いたバッファへの書き込み
		* @param[in] commandList リストの参照
		* @param[in] commandQueue キューの参照
		* @param[in] resource シェーダーリソースへの弱参照
		* @param[out] subResource サブリソース
		* @param[in] path ファイルパス
		* @return リザルト
		*/
		HRESULT UpdateSubResource(std::shared_ptr<CommandList> list, std::shared_ptr<RenderContext>& renderContex, std::weak_ptr<ShaderResource> resource, D3D12_SUBRESOURCE_DATA& subResource, std::string path);


		/**
		* @fn
		* @brief WriteToSubResourceを用いたバッファへの書き込み
		* @param[in] commandList リストの参照
		* @param[in] commandQueue キューの参照
		* @param[in] resource シェーダーリソースへの弱参照
		* @param[out] subResource サブリソース
		* @param[in] paths ファイルパス
		* @return リザルト
		*/
		HRESULT WriteToSubResource(std::shared_ptr<CommandList> list, std::shared_ptr<RenderContext>& renderContex, std::weak_ptr<ShaderResource> resource, D3D12_SUBRESOURCE_DATA& subResource, std::string path);

		/**
		* @fn
		* @brief Fileの読み込み
		* @param[out] metaData メタデータへの参照
		* @param[out] scratchImage スクラッチデータへの参照
		* @param[in] path ファイルパス
		* @return リザルト
		*/
		HRESULT LoadFile(DirectX::TexMetadata& metaData, DirectX::ScratchImage& scratchImage, std::string& path);

		/**
		* @fn
		* @brief WICFileの読み込み
		* @param[out] metaData メタデータへの参照
		* @param[out] scratchImage スクラッチデータへの参照
		* @param[in] path ファイルパス
		* @return リザルト
		*/
		HRESULT LoadWICFile(DirectX::TexMetadata& metaData, DirectX::ScratchImage& scratchImage, std::string& path);

		/**
		* @fn
		* @brief DDSFileの読み込み
		* @param[out] metaData メタデータへの参照
		* @param[out] scratchImage スクラッチデータへの参照
		* @param[in] path ファイルパス
		* @return リザルト
		*/
		HRESULT LoadDDSFile(DirectX::TexMetadata& metaData, DirectX::ScratchImage& scratchImage, std::string& path);

		/**
		* @fn
		* @brief TGAFileの読み込み
		* @param[out] metaData メタデータへの参照
		* @param[out] scratchImage スクラッチデータへの参照
		* @param[in] path ファイルパス
		* @return リザルト
		*/
		HRESULT LoadTGAFile(DirectX::TexMetadata& metaData, DirectX::ScratchImage& scratchImage, std::string& path);

		/**
		* @fn
		* @brief フォーマットからガンマ補正がかかっているかどうかの検討をつける
		* @param[in] format format
		* @return リザルト 真でガンマ値あり
		*/
		bool IsUseGamma(DXGI_FORMAT format);

	};

}