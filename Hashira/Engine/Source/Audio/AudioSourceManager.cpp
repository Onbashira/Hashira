#include "AudioSourceManager.h"


void Hashira::AudioSourceManager::SetResource(std::string resourceName,std::shared_ptr<AudioWaveSource> source)
{
	this->_resourceMap.Set(resourceName,source);
}

std::weak_ptr<Hashira::AudioWaveSource> Hashira::AudioSourceManager::GetResource(std::string resourceName)
{
	return this->_resourceMap.Get(resourceName);
}

void Hashira::AudioSourceManager::EraseResource(std::string name)
{
	this->_resourceMap.Erase(name);
}

bool Hashira::AudioSourceManager::IsLoaded(std::string resourceName)
{
	return this->_resourceMap.IsLooded(resourceName);
}

void Hashira::AudioSourceManager::DiscardManager()
{
	this->_resourceMap.DiscardMap();
}
