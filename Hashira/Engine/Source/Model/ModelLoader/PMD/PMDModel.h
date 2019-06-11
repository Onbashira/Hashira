#pragma once
#include "Engine/Source/Math/Math.h"
#include <vector>
#include <map>
#include <string>


namespace K3D {

	class GraphicsCommandList;

#pragma pack(push,1)
	//�w�b�_�[�t�@�C��
	struct PMDHeader {
		char signature[3];
		float version;
		char name[20];
		char coment[0x100];
	};
#pragma pack(pop)

#pragma pack(push,1)
	//���_���
	struct PMDVertex {
		Vector3 pos;
		Vector3 normal;
		Vector2 texCoord;
		unsigned short boneIndex[2];
		unsigned char boneWeight;
		unsigned char edgeFlag;
	};
#pragma pack(pop)

#pragma pack(push,1)
	//�}�e���A�����
	struct PMDMaterial {
		Vector3 diffuse;
		float alpha;
		float power;
		Vector3 specular;
		Vector3 emissive;
		unsigned char toonIndex;
		unsigned char visualFlag;
		unsigned int faceVertex;
		char textureName[20];
	};
#pragma pack(pop)

#pragma pack(push,1)
	//�{�[�����

	struct PMDBone {
		char boneName[20];
		unsigned short parentIndex;
		unsigned short tailIndex;
		char boneType;
		unsigned short IKBoneIndex;
		Vector3 boneHeadPosition;
	};
#pragma pack(pop)

	struct Bone {
		Vector3 head;
		Vector3 tail;
	};

	struct BoneNode {
		std::string				name;
		unsigned short					parentIndex;				//�e�ւ̃C���f�N�T
		unsigned short					ownIndex;
		std::vector<BoneNode>	children;					//�q�ւ̃A�N�Z�X�|�C���^
	};

	struct BoneTree {

		UINT							boneNum;
		std::map<std::string, int>		boneMap;
		std::map<std::string, BoneNode*>nodeAccessor;
		BoneNode						root;
		std::vector<Matrix>	nodeMatrix;
	};

	//IK���
#pragma pack(push,1)
	struct PMDIKData {
		unsigned short boneIndex;
		unsigned short targetboneIndex;
		unsigned char chainLength;
		unsigned short recursiveCount;
		float controlWeight;
		std::vector<unsigned short> childBone;
	};
#pragma pack(pop)


	//���[�ӏ��
#pragma pack(push,1)
	struct PMDMorph {
	};
#pragma pack(pop)


	class PMDModel
	{
		//-------------------------------------------
		friend class PMDLoader;
		//-------------------------------------------

	private:
	public:
		std::string									modelPath;
		PMDHeader									header;
		std::vector<PMDVertex>						vertices;
		std::vector<unsigned short>							verticesIndex;
		std::vector<PMDMaterial>					materials;
		std::vector<PMDBone>						bonesInfo;
		std::vector<PMDIKData>						ikData;
		std::vector<Bone>							bones;
	private:
	public:
		PMDModel();
		virtual~PMDModel();
	};
}