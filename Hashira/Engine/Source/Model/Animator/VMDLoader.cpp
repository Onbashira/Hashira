#include "Engine/Source/Math/Math.h"
#include "VMD.h"
#include "VMDLoader.h"
#include <map>
#include <vector>
#include <algorithm>

K3D::VMDLoader::VMDLoader()
{
}


K3D::VMDLoader::~VMDLoader()
{
}


std::shared_ptr<K3D::MotionData> K3D::VMDLoader::LoadMotion(std::string path)
{
	FILE* fp = nullptr;
	fopen_s(&fp ,path.c_str(), "rb");

	if (fp == nullptr) {
		return nullptr;
	}
	std::shared_ptr<MotionData> motion = std::make_shared<MotionData>();

	VMDHeader header;
	fread(&header, sizeof(VMDHeader), 1, fp);
	unsigned long count = {};
	fread(&count, sizeof(count), 1, fp);

	motion->vmdMotion.resize(count);
	//“Ç‚Ýž‚Ýˆ—
	for (auto& motion : motion->vmdMotion) {
		fread(&motion, sizeof(VMDMotion), 1, fp);
	}
	fclose(fp);

	for (auto& data : motion->vmdMotion) {

		//DirectX::XMVECTOR quaternion = DirectX::XMLoadFloat4(&data.rotation);
		Quaternion quaternion = Quaternion(data.rotation.x, data.rotation.y, data.rotation.z, data.rotation.w);

		MotionKeyFrameData  motionData(data.frameNo, quaternion, data.location);

		motionData.interpolationA = Vector4(data.InterPolation[3] , data.InterPolation[5],data.InterPolation[11],data.InterPolation[15]);
		motionData.interpolationB = Vector4(data.InterPolation[3], data.InterPolation[5], data.InterPolation[11], data.InterPolation[15]);

		motion->keyFrame[data.boneName].push_back(motionData);

	}
	for (auto& keyframe : motion->keyFrame) {
		std::sort(keyframe.second.begin(), keyframe.second.end(), [](MotionKeyFrameData& lhs, MotionKeyFrameData& rhs) {return rhs.frameNo > lhs.frameNo; });
	}

	return motion;
}
