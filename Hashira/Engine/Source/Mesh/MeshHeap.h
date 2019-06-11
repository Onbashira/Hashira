#pragma once
#include "Engine/Source/DescriptorHeap/Descriptor.h"
#include "Engine/Source/Resource/ConstantBuffer.h"
#include <memory>
#include <vector>

namespace Hashira {

	class ShaderResource;
	class TextureObject;
	class CommandList;
	class ModelMesh;

	//Meshをレンダリングするためのクラス。主にシェーダーに投げるためのヒープを管理する
	class MeshHeap
	{
		friend class ModelMesh;
	public:
	protected:
	private:


		//!変形情報デスクリプタの開始位置 (maybe 0)
		unsigned int _transformStartPoint;
		//!マテリアルデスクリプタの開始位置 (maybe 0+1 ~ x)
		unsigned int _materialStartPoint;
		//!テクスチャデスクリプタの開始位置 (maybe x+1 ~ y)
		unsigned int _textureStartPoint;
		//!マテリアル情報のバッファ 　　
		Hashira::ConstantBuffer _materialBuffer;
		//!テクスチャへの参照
		std::vector<std::weak_ptr<Hashira::TextureObject>> _textureResource;
		//!デスクリプタ (変形情報 + マテリアル数 + テクスチャ数)
		std::vector<std::weak_ptr<Hashira::Descriptor>> _descriptors;
		
		//基本レイアウト
		//1カメラ　2トランスフォーム、3~ｘマテリアル (x+1)~y テクスチャ

	public:

		MeshHeap();

		virtual ~MeshHeap();

		void SetTransformDescStartIndex(unsigned int startIndex = 0);

		void SetMaterialDescStartIndex(unsigned int startIndex = 1);

		void SetTextureDescStartIndex(unsigned int startIndex = 2);

		unsigned int GetTransformDescStartIndex();

		unsigned int GetTextureDescStartIndex();

		unsigned int GetMaterialDescStartIndex();

		ConstantBuffer& GetMaterialBufffer();

		std::vector<std::weak_ptr<Hashira::TextureObject>>& GetTexturesRef();

		std::weak_ptr<Hashira::TextureObject> GetTextureRef(unsigned int textureIndex);

		MeshHeap& AddTextureRef(std::weak_ptr<Hashira::TextureObject> textureRef);

		std::vector <std::weak_ptr<Descriptor>>& GetDescriptors();

		void AddDescriptor(std::weak_ptr<Descriptor>& descriptor);

		void Discard();

	protected:

	private:

	};
}

