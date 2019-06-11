#pragma once
#include <memory>


namespace K3D{
	struct  MotionData;
	class VMDLoader
	{
	private:

	public:

	private:

	public:

		std::shared_ptr<K3D::MotionData> LoadMotion(std::string path);

		VMDLoader();

		~VMDLoader();
	};
}