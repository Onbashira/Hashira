#include <memory>
#include "MotionAssetPool.h"
#include "VMDLoader.h"
#include "MotionManager.h"


K3D::MotionManager::~MotionManager()
{
	Discard();
}


std::weak_ptr<K3D::MotionData> K3D::MotionManager::LoadMotion(std::string motion)
{
	if (!GetInstance()._motionPool.IsLoadedData(motion)) {
		GetInstance()._motionPool.RegistMotionData(motion, GetInstance()._motionLoader.LoadMotion(motion));
		return GetInstance()._motionPool.GetMotionData(motion);
	}
	else {
		return GetInstance()._motionPool.GetMotionData(motion);
	}
}

std::weak_ptr<K3D::MotionData> K3D::MotionManager::GetMotionData(std::string motion)
{
	return GetInstance()._motionPool.GetMotionData(motion);
}

K3D::MotionManager & K3D::MotionManager::GetInstance()
{
	static MotionManager instance;
	return instance;
}

void K3D::MotionManager::Discard()
{
	GetInstance()._motionPool.DiscardMotion();
}

