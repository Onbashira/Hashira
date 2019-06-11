#include "Plane.h"
#include "Engine/Source/Primitive/DefaultComponent/PrimitiveInputComponent.h"
#include "Engine/Source/Primitive/DefaultComponent/PrimitivePhysicsComponent.h"
#include "Engine/Source/Primitive/DefaultComponent/PrimitiveRenderer.h"
#include "Engine/Source/Component/ComponentsHeader.h"
#include "Engine/Source/Mesh/MeshBuffer.h"
#include "Engine/Source/Mesh/MeshHeap.h"
#include "Engine/Source/Mesh/ModelMesh.h"
#include "Engine/Source/DescriptorHeap/GameHeap.h"


Hashira::Plane::Plane(std::shared_ptr<GameHeap>& heap) :
	PrimitiveObject(new PrimitiveRenderer(), new PrimitiveInputComponent(), new PrimitivePhysicsComponent(), heap)
{
	InitalizeTransformBuffer(sizeof(Transform));
	UpdateTransformBuffer();
}

Hashira::Plane::~Plane()
{
}

void Hashira::Plane::Update()
{
}

void Hashira::Plane::Draw(std::shared_ptr<CommandList>& cmdList)
{

	this->_graphicsComponent->Draw(cmdList, this, nullptr, _modelMesh.get());

}

void Hashira::Plane::Initialize()
{
	CreateMesh();
	CreateIndices();
	CreateDescriptors();
}

void Hashira::Plane::CreateMesh()
{

	unsigned int surfaceCount = 6;
	unsigned int planeVertex = 4;
	std::vector<PrimitiveVertex> vertices(planeVertex);
	std::vector<unsigned int > indices;

	PrimitiveVertex plane[4];
	for (unsigned int i = 0; i < planeVertex; ++i) {
		vertices[i].pos = Vector3((2.0f*static_cast<float>((i) % 2) - 1.0f) / 2.0f, -(2.0f * static_cast<float>((i) % 4 / 2) - 1.0f) / 2.0f, -1.0f / 2.0f);
		vertices[i].normal = Vector3::back;
		vertices[i].texcoord = { static_cast<float>(i % 2), static_cast<float>(i / 2) };
	}
	unsigned int planeList[] = { 0,1,2,1,3,2 };
	for (unsigned int listIndex = 0; listIndex < surfaceCount; listIndex++) {
		indices.push_back(planeList[listIndex]);
	}

	//GPUResourceinitialize

	{

		//VBInitialize
		this->_modelMesh->AddVertexBuffer(sizeof(PrimitiveVertex), vertices.size(), vertices.data());
		this->_modelMesh->InitializeIndexBuffer(sizeof(unsigned int), indices.size(), indices.data());
	}
}

void Hashira::Plane::CreateIndices()
{
}

void Hashira::Plane::CreateDescriptors()
{
	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
	//デフォルトカメラデスクリプタの取得(register0
	this->_modelMesh->AddDescriptor(_gameHeap->GetDescriptorHandle(GameHeap::HeapType::CPU, 0));
	cbvDesc.BufferLocation = this->_transformBuffer.GetResource()->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = Util::ConstantBufferAlign(sizeof(Transform));
	//register1
	this->_modelMesh->AddDescriptor(_gameHeap->CreateCBView(cbvDesc));
}