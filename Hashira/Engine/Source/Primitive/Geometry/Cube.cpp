#include "Cube.h"
#include "Engine/Source/Primitive/DefaultComponent/PrimitiveInputComponent.h"
#include "Engine/Source/Primitive/DefaultComponent/PrimitivePhysicsComponent.h"
#include "Engine/Source/Primitive/DefaultComponent/PrimitiveRenderer.h"
#include "Engine/Source/Component/ComponentsHeader.h"
#include "Engine/Source/Mesh/MeshBuffer.h"
#include "Engine/Source/Mesh/MeshHeap.h"
#include "Engine/Source/Mesh/ModelMesh.h"
Hashira::Cube::Cube(std::shared_ptr<GameHeap>& heap) :
	PrimitiveObject(new PrimitiveRenderer(), new PrimitiveInputComponent(), new PrimitivePhysicsComponent(), heap)
{
	InitalizeTransformBuffer(sizeof(Transform));
	UpdateTransformBuffer();
}


Hashira::Cube::~Cube()
{
}

void Hashira::Cube::Update()
{
	GameObject::UpdateTransformBuffer();
}

void Hashira::Cube::Draw(std::shared_ptr<CommandList>& cmdList)
{
	this->_graphicsComponent->Draw(cmdList, this, nullptr, _modelMesh.get());

}

void Hashira::Cube::Initialize()
{
	CreateMesh();
	CreateIndices();
	this->CreateDescriptors();

}

void Hashira::Cube::CreateMesh()
{


	unsigned int sufaceCount = 6;
	unsigned int planeVertex = 4;
	std::vector<PrimitiveVertex> vertices;
	std::vector<unsigned int > indices;

	PrimitiveVertex plane[4];
	for (unsigned int i = 0; i < planeVertex; ++i) {
		plane[i].pos = Vector3((2.0f*static_cast<float>((i) % 2) - 1.0f) / 2.0f, -(2.0f * static_cast<float>((i) % 4 / 2) - 1.0f) / 2.0f, -1.0f / 2.0f);
		plane[i].normal = Vector3::back;
		plane[i].texcoord = { static_cast<float>(i % 2), static_cast<float>(i / 2) };
	}

	unsigned int planeList[] = { 0,1,2,1,3,2 };

	//四方
	for (unsigned int i = 0; i < sufaceCount - 2; ++i) {
		PrimitiveVertex p;
		Quaternion q = Quaternion::CreateFromAxisAngles(Vector3::up, F_PIDIV2 * i);
		for (unsigned int v = 0; v < planeVertex; v++) {
			p.normal = Vector3::Rotate(plane[v].normal, q);
			p.pos = Vector3::Rotate(plane[v].pos, q);

			p.texcoord = plane[v].texcoord;
			vertices.push_back(p);
		}
		for (unsigned int listIndex = 0; listIndex < sufaceCount; listIndex++) {
			indices.push_back(planeList[listIndex] + (4 * i));
		}
	}
	//底面上面
	for (unsigned int i = sufaceCount - 2; i < sufaceCount; ++i) {
		PrimitiveVertex p;
		float rad = F_PIDIV2 + F_PI * (i - (sufaceCount - 2));
		Quaternion q = Quaternion::CreateFromAxisAngles(Vector3::right, rad);
		for (unsigned int v = 0; v < planeVertex; v++) {
			p.normal = Vector3::Rotate(plane[v].normal, q);
			p.pos = Vector3::Rotate(plane[v].pos, q);
			p.texcoord = plane[v].texcoord;
			vertices.push_back(p);

		}
		for (unsigned int listIndex = 0; listIndex < sufaceCount; listIndex++) {
			indices.push_back(planeList[listIndex] + (4 * i));
		}
	}

	//GPUResourceinitialize

	{
		//VBInitialize
		this->_modelMesh->AddVertexBuffer(sizeof(PrimitiveVertex), vertices.size(), vertices.data());
		this->_modelMesh->InitializeIndexBuffer(sizeof(unsigned int) , indices.size(), indices.data());

	}
}

void Hashira::Cube::CreateIndices()
{
}
void Hashira::Cube::CreateDescriptors()
{
	//D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
	////デフォルトカメラデスクリプタの取得(register0
	//this->_modelMesh->AddDescriptor(_gameHeap->GetDescriptorHandle(GameHeap::HeapType::CPU, 0));
	//cbvDesc.BufferLocation = this->_transformBuffer.GetResource()->GetGPUVirtualAddress();
	//cbvDesc.SizeInBytes = Util::ConstantBufferAlign(sizeof(Transform));
	////register1
	//this->_modelMesh->AddDescriptor(_gameHeap->CreateCBView(cbvDesc));

}


