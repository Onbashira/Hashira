#include <memory>
#include "MMDModelResourceDataManager.h"
#include "MMDModelResourceData.h"


K3D::MMDModelResourceDataManager::MMDModelResourceDataManager()
{
}


K3D::MMDModelResourceDataManager::~MMDModelResourceDataManager()
{
	Discard();
}


std::weak_ptr<K3D::MMDModelResourceData> K3D::MMDModelResourceDataManager::GetConvertedModelData(std::string path)
{
	return _pool.Get(path);
}

void K3D::MMDModelResourceDataManager::SetConvertedModelData(std::string key, std::shared_ptr<MMDModelResourceData> modelData)
{
	_pool.Set(key, modelData);
}

void K3D::MMDModelResourceDataManager::EraseConvertedModelData(std::string key)
{
	_pool.Erase(key);
}

bool K3D::MMDModelResourceDataManager::IsLoadedModel(std::string path)
{
	return _pool.IsLooded(path);
}

void K3D::MMDModelResourceDataManager::Discard()
{
	_pool.DiscardMap();
}




