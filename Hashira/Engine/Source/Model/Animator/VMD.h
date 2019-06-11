#pragma once
#include "Engine/Source/Math/Math.h"
#include <map>
#include <vector>

namespace K3D {
	class VMDLoader;

#pragma pack(push,1)
	struct VMDHeader {
		//VMDのヘッダ情報
		char vmdHeader[30];
		char VmdModelName[20];
	};
#pragma pack(pop)

#pragma pack(push,1)
	struct VMDMotion
	{
		//
		friend class  VMDLoader;
		//

		char boneName[15];
		unsigned int frameNo;
		Vector3 location; //position
		Quaternion rotation; //quaternion
		unsigned char InterPolation[64];		//Completion
	};
#pragma pack(pop)
	//モーション情報
	struct MotionKeyFrameData {
		unsigned int frameNo;
		Quaternion quaternion;
		Vector3	location;

		Vector4 interpolationA;
		Vector4 interpolationB;

		MotionKeyFrameData(unsigned int _frameNo, Quaternion _quaternion, Vector3 location) :
			frameNo(_frameNo), quaternion(_quaternion), location(location) {}
		MotionKeyFrameData() :frameNo(0), quaternion() {};
	};

	struct MotionData {
		//エイリアス
		unsigned int frame;													//現在のフレーム
		std::map<std::string, std::vector<MotionKeyFrameData>> keyFrame;	//ボーン名とモーションデータのペア配列
		std::vector<VMDMotion> vmdMotion;
	};

}