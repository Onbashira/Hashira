#include "Sprite.h"
#include "Engine/Source/Sprite/DefaultComponent/SpriteInputComponent.h"
#include "Engine/Source/Sprite/DefaultComponent/SpritePhysicsComponent.h"
#include "Engine/Source/Sprite/DefaultComponent/SpriteRenderer.h"
#include "Engine/Source/Component/ComponentsHeader.h"
#include "Engine/Source/Mesh/MeshBuffer.h"
#include "Engine/Source/Mesh/MeshHeap.h"
#include "Engine/Source/Mesh/ModelMesh.h"
#include "Engine/Source/DescriptorHeap/GameHeap.h"
#include "Engine/Source/Resource/Resource.h"
#include "Engine/Source/Resource/ConstantBuffer.h"

Hashira::Sprite::Sprite(std::shared_ptr<GameHeap>& heap) :
	PrimitiveObject(new SpriteRenderer(), new SpriteInputComponent(), new SpritePhysicsComponent(), heap)
{
	InitalizeTransformBuffer(sizeof(Transform));
	UpdateTransformBuffer();
}

Hashira::Sprite::~Sprite()
{
}

void Hashira::Sprite::Update()
{
}

void Hashira::Sprite::Draw(std::shared_ptr<CommandList>& cmdList)
{
	this->_graphicsComponent->Draw(cmdList, this, nullptr, _modelMesh.get());

}

void Hashira::Sprite::Initialize()
{
	CreateMesh();
	CreateIndices();

	//testResource Creation

	{
		_materialBuffer = std::make_shared<ConstantBuffer>();
		auto hr = _materialBuffer->Initialize(Util::Alignment256Bytes(sizeof(SpriteMaterial)));

		if (FAILED(hr))
		{
			Util::Comment(L"Error SpriteCreate");
		}
		Vector4 albedo(0.1f, 0.8f, 1.0f, 1.0f);
		_materialBuffer->Update(&albedo, sizeof(SpriteMaterial), 0);
	}


	this->CreateDescriptors();
}

void Hashira::Sprite::CreateMesh()
{
	unsigned int planeVertex = 4;
	struct SpriteVertex
	{
		Vector3 pos;
		Vector2 uv;
	} vert;


	std::vector<SpriteVertex> vertexes;

	SpriteVertex plane[4];
	for (unsigned int i = 0; i < planeVertex; ++i) {
		plane[i].pos = Vector3((2.0f*static_cast<float>((i) % 2) - 1.0f), -(2.0f * static_cast<float>((i) % 4 / 2) - 1.0f),0.0f);
		plane[i].uv = { static_cast<float>(i % 2), static_cast<float>(i / 2) };
		vertexes.push_back(plane[i]);
	}

	this->_modelMesh->AddVertexBuffer(sizeof(SpriteVertex) * vertexes.size(), sizeof(SpriteVertex), vertexes.data());

}

void Hashira::Sprite::CreateIndices()
{
	std::vector<unsigned int > indexList;
	const unsigned int count = 6;

	unsigned int planeList[] = { 0,1,2,1,3,2 };
	for (unsigned int listIndex = 0; listIndex < count; listIndex++) {
		indexList.push_back(planeList[listIndex]);
	}
	this->_modelMesh->GetIndexBuffer()->Initialize(static_cast<ULONG64>(indexList.size() * sizeof(unsigned int )),static_cast<size_t>(sizeof(unsigned int)) ,indexList.data());

}

void Hashira::Sprite::CreateDescriptors()
{
	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
	//マテリアル情報のCBVを作成してバインド
	cbvDesc.BufferLocation = this->_materialBuffer->GetResource()->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = Util::ConstantBufferAlign(sizeof(SpriteMaterial));
	//register1
	this->_modelMesh->AddDescriptor(_gameHeap->CreateCBView(cbvDesc));

}
