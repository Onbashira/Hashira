#include "stdafx.h"
#include "TextureObject.h"
#include "Engine/Source/Texture/TextureLoader.h"

Hashira::TextureObject::TextureObject()
{
}

Hashira::TextureObject::TextureObject(std::shared_ptr<ShaderResource> sr, const TextureObjectDesc & desc):
	_textureResource(sr),_desc(desc)
{
}

Hashira::TextureObject::~TextureObject()
{
}

const Hashira::TextureObjectDesc & Hashira::TextureObject::GetDesc() const
{
	return _desc;
}

void Hashira::TextureObject::SetDesc(TextureObjectDesc && desc)
{
	this->_desc = desc;
}

std::weak_ptr<Hashira::ShaderResource> Hashira::TextureObject::GetShaderResource()
{
	return this->_textureResource;
}
