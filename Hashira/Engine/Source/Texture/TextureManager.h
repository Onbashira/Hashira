#pragma once

#include"Engine/Source/Utility/D3D12Common.h"
#include "Engine/Source/Utility/ManagerComponent.h"
#include <map>
#include <memory>


namespace Hashira {

	class DescriptorHeap;
	class TextureLoader;
	class ShaderResource;
	class TextureObject;
	class D3D12Device;
	class CommandQueue;
	class CommandList;
	class RenderContext;
	class RenderingDevice;

	class TextureManager
	{

	public:

		friend class TextureLoader;

	private:

		//ヌルテクスチャの名前
		const std::string _nullTextureName = "NULL_TEXTURE";

		//!黒テクスチャへの名前
		const std::string _nullTextureBlackName = "NULL_BLACK_TEXTURE";

		//!白テクスチャへの名前
		const std::string _nullTextureWhiteName = "NULL_WHITE_TEXTURE";

		//!情報キャリア
		UnorderedManagerComponent<TextureObject> _textureResourceMap;

		//!レンダーコンテキスト（キュー共有）を保持
		std::shared_ptr<RenderContext> _renderContext;

	public:

		~TextureManager();

		static TextureManager& GetInstance() {
			static TextureManager instance;
			return instance;
		};

		void SetRenderContext(std::shared_ptr<RenderContext>& renderContext);

		std::shared_ptr<TextureObject> GetTexture(String filename);

		std::shared_ptr<TextureObject> LoadTexture(String filename);

		std::shared_ptr<TextureObject> DuplicateTexture(String srcFilename, String dstFilename);

		std::shared_ptr<TextureObject> CreateColorTexture(String name, const Vector4 color);

		std::weak_ptr<TextureObject> GetNullTexture();

		std::weak_ptr<TextureObject> GetBlackTexture();

		std::weak_ptr<TextureObject> GetWhiteTexture();

		void Discard();

	private:

		TextureManager();

		TextureManager & operator=(const TextureManager& value) { return *this; };

		TextureManager & operator=(TextureManager&& value) { return *this; };

		TextureManager(const TextureManager& other) {};

		TextureManager(TextureManager&& other) {};

		void CreateBlackTexture();

		void CreateWhiteTexture();

	};
}