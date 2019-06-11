#include "MMDModelManager.h"
#include "MMDModel.h"


K3D::MMDModelManager::MMDModelManager()
{
}


K3D::MMDModelManager::~MMDModelManager()
{
	Discard();
}


void K3D::MMDModelManager::SetMMDModel(std::string modelPath, std::shared_ptr<MMDModel> model)
{
	_pool.Set(modelPath, model);
}

std::weak_ptr<K3D::MMDModel> K3D::MMDModelManager::GetMMDModel(std::string modelPath)
{
	return _pool.Get(modelPath);
}

void K3D::MMDModelManager::EraseMMDModel(std::string modelPath)
{
	_pool.Erase(modelPath);
}

void K3D::MMDModelManager::Discard()
{
	_pool.DiscardMap();
}