#include "stdafx.h"
#include "Material.h"

void Hashira::Material::SetBaseColor(const Vector3 & color)
{
	this->_baseColor = color;
}

void Hashira::Material::SetSpecular(const Vector3 & color, float power)
{
	this->_specular = color;
	this->_specularPower = power;
}

void Hashira::Material::SetAmbient(const Vector3 & ambient)
{
	this->_ambient = ambient;
}

void Hashira::Material::SetRoughness(float roughness)
{
	this->_roughness = roughness;
}

void Hashira::Material::SetMetalness(float metalness)
{
	this->_metalness = metalness;
}

Hashira::Vector3 & Hashira::Material::GetBaseColor()
{
	return _baseColor;
}

Hashira::Vector3 & Hashira::Material::GetAmbient()
{
	return _ambient;
}

float & Hashira::Material::GetRoughness()
{
	return _roughness;
}

float & Hashira::Material::GetMetalness()
{
	return _metalness;
}

std::vector<std::weak_ptr<Hashira::TextureObject>>& Hashira::Material::GetTextures()
{
	return _textures;
}
