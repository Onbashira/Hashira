#pragma once
#include "Engine/Source/Model/ModelLoader/PMD/PMDModel.h"
#include "Engine/Source/Model/ModelLoader/PMX/PMXModel.h"
#include "Engine/Source/PipelineState/PipelineStateObject.h"
#include "Engine/Source/DescriptorHeap/DescriptorHeap.h"


namespace K3D {

	class ModelConverter;

	enum class MMDWeightDeformType {
		BDEF1 = 0,
		BDEF2,
		BDEF4,
		SDEF,
		QDEF
	};

	union MMDWeightDeform
	{
		BDEF1	bdef1;
		BDEF2	bdef2;
		BDEF4	bdef4;
		SDEF	sdef;
		QDEF	qdef;

		MMDWeightDeform()  : sdef() {};
		MMDWeightDeform(const WeightDeform& other) {
			this->sdef = other.sdef;
		};
		MMDWeightDeform(const MMDWeightDeform& other) {
			this->sdef = other.sdef;
		};
		MMDWeightDeform& operator=(const WeightDeform& value) {
			sdef = value.sdef;
			return *this;
		};
		MMDWeightDeform& operator=(const MMDWeightDeform& value) {
			sdef = value.sdef;
			return *this;
		};
		~MMDWeightDeform() {};
	};

	enum class MMDMaterialFlags {
		NO_CULL,
		GROUND_SHADOW,
		DRAW_SHADOW,
		RECEIVE_SHADOW,
		HAS_EDGE,
		VERTEX_COLOR,
		POINT_DRAWING,
		LINE_DRAWING
	};

	enum class MMDBoneFlags {
		INDEXED_TAIL_POSITION,
		ROTATABLE,
		TRANSLATABLE,
		IS_VISIBLE,
		ENABLE,
		IK,
		INHERIT_ROTATION,
		INHERIT_TRANSLATION,
		FIXED_AXIS,
		LOCAL_COORDINATE,
		PHISICS_AFTER_DEFORM,
		EXTERNAL_PARENT_DEFORM

	};

	enum class MMDSphereBlendType {
		NONE,
		MULT,
		ADD,
		SUB_TEXURE
	};


	struct MMDVertex {
		//�ʒu
		Vector3 pos;
		//���_�@��
		Vector3 normal;
		//UV���W
		Vector2 texCoord;
		//�ό`�^�C�v
		MMDWeightDeformType deformType;
		//�ό`�Q�Ə��
		MMDWeightDeform deformation;
		MMDVertex() :
			pos(), normal(), texCoord(){};
		~MMDVertex() {};
		MMDVertex(const MMDVertex& other) {
			*this = other;
		}
		MMDVertex& operator= (const MMDVertex& rhs) {
			pos = rhs.pos;
			normal = rhs.normal;
			texCoord = rhs.texCoord;
			deformType = rhs.deformType;
			deformation = rhs.deformation;
			return *this;
		}
	};

	//�����_�����O��⏕����}�e���A�����
	struct MMDMaterial {
		//	�g�U���ˌ� w = alpha
		Vector4 diffuse;
		//�@���ʔ��ˌ�	w = pow
		Vector4 specular;
		//����
		Vector3 ambient;
		//���Ȕ����F
		Vector3 emissive;
		//�X�t�B�A�}�b�v�̃u�����f�B���O�^�C�v
		int	sphereBlendType;
		//�e�N�X�`���e�[�u���̃C���f�b�N�X
		unsigned int textureTableIndex;
		//�X�t�B�A�}�b�v�̃C���f�b�N�X
		unsigned int	sphereIndex;
		//LUT�̎Q�ƃe�[�u���ԍ�
		unsigned int	toonIndex;
		//���̃}�e���A���`��܂ł̃C���f�b�N�X�I�t�Z�b�g
		unsigned int	surfaceCount;
	};

	//�����_�����O���ɃZ�b�g����C���f�b�N�X���
	struct MMDIndexList {
		std::vector<unsigned int> list;
	};

	//�����_�����O���ɕK�v�ȃe�N�X�`�����
	struct MMDTextureHeap {
		std::weak_ptr<DescriptorHeap> texturesHeap;
	};

	struct MMDBoneNode {
		//�e�ւ̃C���f�b�N�X
		unsigned int parentIndex;
		//���g�̃C���f�b�N�X
		unsigned int index;
		//�{�[���̈ʒu
		Vector3	pos;
		//�����|�[�Y����̉�]�ʁi�l�����j
		Quaternion	rotation;
		unsigned short	boneOptionFlags;
		struct MMDBoneOption {
			//�p���{�[��
			InheritBone inheritBone;
			//���[�J����
			BoneFixedAxis fixidAxis;
			//�Œ莲
			BoneLocalCoordinate localAxis;
			//�O���e�C���f�b�N�X
			BoneExternalParent	externalParentBoneIndex;
		}boneOption;
		//���̃{�[����e�Ƃ��ĎQ�Ƃ���{�[��
		std::vector<MMDBoneNode> childrenBone;
		void Discard() {
			for (auto& child : childrenBone) {
				child.Discard();
			}
			this->childrenBone.clear();
		};

		MMDBoneNode()  {
		};
		~MMDBoneNode() {
		};
	};

	struct MMDIKLinkData {
		unsigned int boneIndex;
		bool hasLimit;		//�����l����l�̔��f�t���O
		IKAngleLimit limits;
	};

	struct MMDIKData {
		unsigned int boneIndex;
		unsigned int targetBoneIndex; //IK�{�[�����ŏ��ɃR�l�N�e�B���O����{�[���C���f�b�N�X
		int loopCount;
		float limitRadian;
		std::vector<MMDIKLinkData> ikLinks;
		MMDIKData()  {};
		~MMDIKData() {
			ikLinks.clear();
		}
	};

	struct MMDBoneTree {
		unsigned int boneNum;
		std::map<std::string, MMDBoneNode*>					boneAccessor;
		std::map<unsigned int, std::string>					boneNameAccessor;
		std::vector<Matrix>									bonesMatrix;
		std::vector<MMDIKData>								modelIKdata;
		MMDBoneNode											rootBone;
		MMDBoneTree()  {

		};

		~MMDBoneTree() {
			bonesMatrix.clear();
			modelIKdata.clear();
			boneAccessor.clear();
		};
	};

	//MMD�œǂݍ��߂郂�f���f�[�^�iPMX�EPMD�j�̃I���W�i���f�[�^��PMX�֊񂹂����
	struct MMDModelResourceData
	{
		//���_���
		std::vector<MMDVertex>						_vertexes;
		std::vector<MMDWeightDeform>				_deformationData;
		//���_�C���f�b�N�X���
		MMDIndexList								_indexList;
		//�e�N�X�`�����
		MMDTextureHeap								_textureHeaps;
		//�}�e���A�����
		std::vector<MMDMaterial>					_materials;
		//�X�P���g��
		std::shared_ptr<MMDBoneTree>				_boneTree;
		//�e�N�X�`���p�X
		std::vector<std::string>					_texturePaths;
		//���f���p�X
		std::string									_modelPath;
		MMDModelResourceData();
		~MMDModelResourceData();
	};
}