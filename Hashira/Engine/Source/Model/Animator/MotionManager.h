#pragma once
#include <memory>
#include "MotionAssetPool.h"
#include "VMDLoader.h"

namespace K3D {

	class MotionManager
	{
	private:
		MotionAssetPool _motionPool;
		VMDLoader		_motionLoader;
	public:
	private:
		MotionManager() {};

		MotionManager(const K3D::MotionManager&) {};

		MotionManager(K3D::MotionManager&&) {};

		K3D::MotionManager& operator= (const K3D::MotionManager& other) { return *this; };

		K3D::MotionManager& operator= (K3D::MotionManager&& other) { return *this; };

	public:
		~MotionManager();

		static std::weak_ptr<K3D::MotionData> LoadMotion(std::string motion);

		static std::weak_ptr<K3D::MotionData> GetMotionData(std::string motion);

		static MotionManager& GetInstance();

		static void Discard();

	};

}