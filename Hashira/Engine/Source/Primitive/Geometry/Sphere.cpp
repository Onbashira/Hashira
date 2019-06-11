#include "Sphere.h"
#include "Engine/Source/Primitive/DefaultComponent/PrimitiveInputComponent.h"
#include "Engine/Source/Primitive/DefaultComponent/PrimitivePhysicsComponent.h"
#include "Engine/Source/Primitive/DefaultComponent/PrimitiveRenderer.h"
#include "Engine/Source/Component/ComponentsHeader.h"
#include "Engine/Source/Mesh/MeshBuffer.h"
#include "Engine/Source/Mesh/MeshHeap.h"
#include "Engine/Source/Mesh/ModelMesh.h"
#include "Engine/Source/DescriptorHeap/GameHeap.h"

Hashira::Sphere::Sphere(std::shared_ptr<GameHeap>& heap) :
	PrimitiveObject(new PrimitiveRenderer(), new PrimitiveInputComponent(), new PrimitivePhysicsComponent(), heap)
{
	InitalizeTransformBuffer(sizeof(Transform));
	UpdateTransformBuffer();
}

Hashira::Sphere::~Sphere()
{
}

void Hashira::Sphere::Update()
{
}

void Hashira::Sphere::Draw(std::shared_ptr<CommandList>& cmdList)
{
	this->_graphicsComponent->Draw(cmdList, this, nullptr, _modelMesh.get());

}

void Hashira::Sphere::Initialize()
{
	CreateMesh();
	this->CreateDescriptors();
}

void Hashira::Sphere::CreateMesh()
{
	const float radius = 1.0f;
	const int div = 32;
	std::vector<PrimitiveVertex> vertices;
	PrimitiveVertex vert;
	vert.pos = { 0.f,radius, 0.f, 1.f };
	vert.normal = { 0.f, 1.0f, 0.0f, 1.0f };
	const Vector3 center = { 0.0f,0.0f,0.0f };
	Vector3 vertPos = { vert.pos.x, vert.pos.y,vert.pos.z };
	vertices.reserve(div * (div + 1));
	
	const float unitYDiv = DirectX::XM_PI / div;
	const float unitUDiv = 1.0f / div;
	const float unitVDiv = 1.0f / div;
	const float unitRadios = DirectX::XM_PI * 2.0f / div;

	const unsigned int circleVertNum = div + 1;

	//y方向分割
	for (unsigned int yDiv = 0; yDiv < circleVertNum; ++yDiv)
	{
		//円の分割
		for (unsigned int circleDiv = 0; circleDiv < circleVertNum; ++circleDiv)
		{
			float rRatio = sin(unitRadios * yDiv * 0.5f);

			float pi = unitRadios * circleDiv;
			vert.pos = Vector3(radius * sin(pi) * rRatio,
				radius * cos(unitYDiv * yDiv),
				radius * cos(pi) * rRatio);
			Vector3 v = Vector3::Normalize(vert.pos - center);
			vert.normal = Vector3( v.x, v.y, v.z);
			vert.texcoord = { unitUDiv * circleDiv, unitVDiv * yDiv };
			vertices.push_back(vert);
		}
	}

	this->_modelMesh->AddVertexBuffer(sizeof(PrimitiveVertex), vertices.size(), vertices.data());

}

void Hashira::Sphere::CreateIndices()
{

	const int div = 32;
	unsigned int indexNo = 0;
	std::vector<unsigned int>indices(div * 2 + (div - 2)*2);
	const unsigned int circleVertNum = div + 1;
	//一番上だけ
	for (unsigned int circleDiv = 0; circleDiv < div; ++circleDiv)
	{
		indices.push_back(circleDiv + 0);
		indices.push_back(circleDiv + (0 + 1) * circleVertNum);
		indices.push_back(circleDiv + (0 + 1) * circleVertNum + 1);
	}

	//一番上と一番下以外
	for (unsigned int yDiv = 1; yDiv < div - 1; ++yDiv)
	{
		for (unsigned int circleDiv = 0; circleDiv < circleVertNum; ++circleDiv)
		{
			indices.push_back(circleDiv + yDiv * circleVertNum);
			indices.push_back(circleDiv + (yDiv + 1) * circleVertNum);
			indices.push_back(circleDiv + (yDiv + 1) * circleVertNum + 1);

			indices.push_back(circleDiv + yDiv * circleVertNum);
			indices.push_back(circleDiv + (yDiv + 1) * circleVertNum + 1);
			indices.push_back(circleDiv + yDiv * circleVertNum + 1);
		}
	}

	//一番下だけ
	for (unsigned int circleDiv = 0; circleDiv < div; ++circleDiv)
	{
		indices.push_back(circleDiv + (div - 1) * circleVertNum);
		indices.push_back(circleDiv + (div + 0) * circleVertNum + 1);
		indices.push_back(circleDiv + (div - 1) * circleVertNum + 1);
	}

	//GPUResourceinitialize

	{
		//VBInitialize
		this->_modelMesh->InitializeIndexBuffer(sizeof(unsigned int), indices.size(), indices.data());

	}
}

void Hashira::Sphere::CreateDescriptors()
{
	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
	//デフォルトカメラデスクリプタの取得(register0
	this->_modelMesh->AddDescriptor(_gameHeap->GetDescriptorHandle(GameHeap::HeapType::CPU, 0));
	cbvDesc.BufferLocation = this->_transformBuffer.GetResource()->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = Util::ConstantBufferAlign(sizeof(Transform));
	//register1
	this->_modelMesh->AddDescriptor(_gameHeap->CreateCBView(cbvDesc));
}
