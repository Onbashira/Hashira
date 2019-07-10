#include "stdafx.h"
#include "ModelMesh.h"
#include "Engine/Source/Mesh/MeshBuffer.h"
#include "Engine/Source/Mesh/MeshHeap.h"


Hashira::ModelMesh::ModelMesh() :
	_meshBuffer(new MeshBuffer()), _meshHeap(new MeshHeap)
{

}

Hashira::ModelMesh::~ModelMesh()
{
}

void Hashira::ModelMesh::InitializeIndexBuffer(size_t elementSize, size_t elementNum, void * pSrc)
{
	_meshBuffer->CreateIndexBuffer(elementSize, elementNum, pSrc);

}

void Hashira::ModelMesh::AddVertexBuffer(size_t elementSize, size_t elementNum, void * pSrc)
{
	_meshBuffer->AddVertexBuffer(elementSize, elementNum, pSrc);
}


const std::unique_ptr<Hashira::GeometryState>& Hashira::ModelMesh::GetGeometryState()
{
	return  _meshBuffer->GetGeometryState();
}

const std::unique_ptr<Hashira::IndexBuffer>& Hashira::ModelMesh::GetIndexBuffer()
{
	return  _meshBuffer->GetIndexBuffer();

}

std::vector<std::weak_ptr<Hashira::TextureObject>>& Hashira::ModelMesh::GetTexturesRef()
{
	return _meshHeap->GetTexturesRef();
}

std::weak_ptr<Hashira::TextureObject> Hashira::ModelMesh::GetTextureRef(unsigned int textureIndex)
{
	return _meshHeap->GetTextureRef(textureIndex);
}

Hashira::MeshHeap & Hashira::ModelMesh::AddTextureRef(std::weak_ptr<Hashira::TextureObject> textureRef)
{
	return _meshHeap->AddTextureRef(textureRef);
}

void Hashira::ModelMesh::Discard()
{
	_meshHeap->Discard();
	_meshBuffer->Discard();
}
