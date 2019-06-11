#include "MeshHeap.h"
#include "Engine/Source/CommandList/CommandList.h"
#include "Engine/Source/Texture/TextureObject.h"

Hashira::MeshHeap::MeshHeap()  :
	_textureStartPoint(0),
	_materialStartPoint(0)
{
	_descriptors.clear();
	_descriptors.shrink_to_fit();
}


Hashira::MeshHeap::~MeshHeap()
{
	Discard();
}


void Hashira::MeshHeap::SetTransformDescStartIndex(unsigned int startIndex)
{
	this->_transformStartPoint = startIndex;

}

void Hashira::MeshHeap::SetMaterialDescStartIndex(unsigned int startIndex)
{
	this->_materialStartPoint = startIndex;

}

void Hashira::MeshHeap::SetTextureDescStartIndex(unsigned int startIndex)
{
	this->_textureStartPoint = startIndex;

}

unsigned int Hashira::MeshHeap::GetTransformDescStartIndex()
{
	return _transformStartPoint;
}

unsigned int Hashira::MeshHeap::GetTextureDescStartIndex()
{
	return _textureStartPoint;
}

unsigned int Hashira::MeshHeap::GetMaterialDescStartIndex()
{
	return _materialStartPoint;
}

Hashira::ConstantBuffer & Hashira::MeshHeap::GetMaterialBufffer()
{
	return this->_materialBuffer;
}

std::vector<std::weak_ptr<Hashira::TextureObject>>& Hashira::MeshHeap::GetTexturesRef()
{
	return this->_textureResource;
}

std::weak_ptr<Hashira::TextureObject> Hashira::MeshHeap::GetTextureRef(unsigned int textureIndex)
{
	if (_textureResource.size() == 0 || _textureResource.size() <= textureIndex) {
		return std::weak_ptr<Hashira::TextureObject>();
	}
	return _textureResource[textureIndex];
}

Hashira::MeshHeap & Hashira::MeshHeap::AddTextureRef(std::weak_ptr<Hashira::TextureObject> textureRef)
{
	this->_textureResource.push_back(textureRef);
	return *this;
}

std::vector<std::weak_ptr<Hashira::Descriptor>>& Hashira::MeshHeap::GetDescriptors()
{
	return  _descriptors;
}

void Hashira::MeshHeap::AddDescriptor(std::weak_ptr<Descriptor>& descriptor)
{
	_descriptors.push_back(descriptor);
}

void Hashira::MeshHeap::Discard()
{
	_descriptors.clear();
	_descriptors.shrink_to_fit();
	_materialBuffer.Discard();
	_textureResource.clear();
	_textureResource.shrink_to_fit();
}
