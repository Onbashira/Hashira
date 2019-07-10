#pragma once
#include "Engine/Source/Command/State/GeometryState.h"

namespace Hashira {
	class MeshHeap;
	class MeshBuffer;
	class TextureObject;
	struct Descriptor;
	class IndexBuffer;

	class ModelMesh
	{

	public:

	private:
		//基本メッシュ情報を保持したもの
		std::unique_ptr<MeshBuffer> _meshBuffer;
		//デスクリプタヒープやマテリアルバッファ、テクスチャへの参照をもったもの (マテリアルバッファは分離すべきか？　あまり意識してなかったけど分離したほうが都合がいいかもしれない)
		std::unique_ptr<MeshHeap> _meshHeap;
	public:	
		
		ModelMesh();

		~ModelMesh();

		void InitializeIndexBuffer(size_t elementSize, size_t elementNum, void * pSrc);

		void AddVertexBuffer(size_t elementSize, size_t elementNum, void * pSrc);

		const std::unique_ptr<Hashira::GeometryState>& GetGeometryState();

		const std::unique_ptr < Hashira::IndexBuffer > & GetIndexBuffer();

		std::vector<std::weak_ptr<Hashira::TextureObject>>& GetTexturesRef();

		std::weak_ptr<Hashira::TextureObject> GetTextureRef(unsigned int textureIndex);

		MeshHeap& AddTextureRef(std::weak_ptr<Hashira::TextureObject> textureRef);

		void Discard();

	};
}