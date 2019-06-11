#pragma once
#include "Engine/Source/Math/Math.h"
#include <vector>

namespace K3D {

	class ModelConverter;

	enum class PMXGlobalTypes {
		TEXT_ENCORDING_TYPE,
		ADDITIONAL_UV_COUNT,
		VERTEX_INDEX_SIZE,
		TEXTURE_INDEX_SIZE,
		MATERIAL_INDEX_SIZE,
		BONE_INDEX_SIZE,
		MORPH_INDEX_SIZE,
		RIGIDBODY_IDNEX_SIZE,
		PMX_GLOBAL_TYPES_MAX
	};

	struct PMXHeader {
		unsigned char signature[4];
		float version;
		unsigned char globalCount;
		std::vector<unsigned char> globals;

		std::wstring modelName;
		std::wstring modelNameEnglish;
		std::wstring comment;
		std::wstring commentEnglish;

	};

	struct BDEF1 {
		int boneIndex;
		BDEF1()  : boneIndex(0) {};
		~BDEF1() {};
	};

	struct BDEF2 {
		int boneIndex01;
		int boneIndex02;
		float boneWeight01;
		float boneWeight02;
		BDEF2() : boneIndex01(0), boneIndex02(0), boneWeight01(0.0f), boneWeight02(0.0f) {

		}
		~BDEF2() {

		};
	};

	struct BDEF4 {
		int boneIndex01;
		int boneIndex02;
		int boneIndex03;
		int boneIndex04;
		float boneWeight01;
		float boneWeight02;
		float boneWeight03;
		float boneWeight04;
		BDEF4() : boneIndex01(0), boneIndex02(0), boneIndex03(0), boneIndex04(0),
			boneWeight01(0.0f), boneWeight02(0.0f), boneWeight03(0.0f), boneWeight04(0.0f) {

		}
		~BDEF4() {

		};
	};

	struct SDEF {
		int boneIndex01;
		int boneIndex02;
		float boneWeight01;
		float boneWeight02;
		Vector3 c;
		Vector3 r0;
		Vector3 r1;
		SDEF() : boneIndex01(0), boneIndex02(0),
			boneWeight01(0), boneWeight02(0), c(), r0(), r1() {

		}
		~SDEF() {

		};
	};

	struct QDEF {
		int boneIndex01;
		int boneIndex02;
		int boneIndex03;
		int boneIndex04;
		float boneWeight01;
		float boneWeight02;
		float boneWeight03;
		float boneWeight04;

		QDEF()  : boneIndex01(0), boneIndex02(0), boneIndex03(0), boneIndex04(0),
			boneWeight01(0.0f), boneWeight02(0.0f), boneWeight03(0.0f), boneWeight04(0.0f) {

		}
		~QDEF() {

		};
	};

	union WeightDeform
	{
		BDEF1	bdef1;
		BDEF2	bdef2;
		BDEF4	bdef4;
		SDEF	sdef;
		QDEF	qdef;

		WeightDeform()  : sdef() {};
		WeightDeform(const WeightDeform& other) {
			this->sdef = other.sdef;
		};
		WeightDeform& operator=(const WeightDeform& value) {
			sdef = value.sdef;
			return *this;
		};
		~WeightDeform() {};
	};

	struct PMXVertex {
		Vector3 position;
		Vector3 normal;
		Vector2 uv;
		std::vector<Vector4> additionalUV; //Can be 0 ;
		unsigned char weightDeformType;
		WeightDeform weightDeform;
		float edgeScale;
		PMXVertex() {};	
		PMXVertex(const PMXVertex &) {};
		~PMXVertex() {};
	};


	struct PMXSurface {
		//[surface 0 -> A, surface 1 -> B, surface 2 -> C] = triangle [vertex A, vertex B, vertex C]
		std::vector<unsigned int> surfaceIndex;
	};
	struct PMXTexturePath {
		std::vector<std::wstring> path;
	};

	union ToonValue {
		unsigned char	unionToon;
		int		individualToon;
		ToonValue()  : individualToon(0) {};
		ToonValue(const ToonValue& other) {
			individualToon = other.individualToon;
		};
		~ToonValue() {};

	};

	struct PMXMaterial {
		std::wstring				nameLocal;
		std::wstring				nameUniversal;
		Vector4						diffuseColor;
		Vector3						specularColor;
		float						specularStrength;
		Vector3						ambientColor;
		unsigned char						drawingFlags;
		Vector4						edgeColor;
		float						edgwScale;
		int							textureIndex;
		int							environmentTextureIndex;
		unsigned char						environmentBlendFlags;
		unsigned char						toonReference;

		ToonValue					toonValue;
		std::vector<std::wstring>	metaData;
		int							surfaceCount;
	};

	struct InheritBone {
		int parentIndex;
		float parentInfluence;

	};
	struct BoneFixedAxis {
		Vector3 axisDirection;

	};
	struct BoneLocalCoordinate {
		Vector3 right;
		Vector3 up;

	};

	struct BoneExternalParent {
		int index;
	};

	struct IKAngleLimit {

		Vector3 minimamAngle;
		Vector3 maximumAngle;
		IKAngleLimit() : minimamAngle(), maximumAngle() {};
		~IKAngleLimit() {};
	};

	struct IKLinks {
		int boneIndex;
		unsigned char hasLimit;
		IKAngleLimit limits;
		IKLinks()  : boneIndex(0), hasLimit(0), limits() {};
		~IKLinks() {};
	};

	struct BoneIK {
		int targetIndex;
		int loopCount;
		float limitRadian;
		int linkCount;
		std::vector<IKLinks> ikLinks;
		BoneIK() : targetIndex(0), loopCount(0), limitRadian(0.0f), linkCount(0) {};
	};

	struct PMXBone {
		std::wstring				boneNameLocal;
		std::wstring				boneNameUniversal;
		Vector3						bonePosition;
		int							parentBoneIndex;
		int							deformationHierarchyLayer;

		//���{�[���t���O
		//0x0001  : �ڑ���(PMD�q�{�[���w��)�\�����@ -> 0 : ���W�I�t�Z�b�g�Ŏw�� 1 : �{�[���Ŏw��
		//0x0002 : ��]�\
		//0x0004 : �ړ��\
		//0x0008 : �\��
		//0x0010 : �����
		//0x0020 : IK
		//0x0080 : ���[�J���t�^ | �t�^�Ώ� 0 : ���[�U�[�ό`�l�^IK�����N�^���d�t�^ 1 : �e�̃��[�J���ό`��
		//0x0100 : ��]�t�^
		//0x0200 : �ړ��t�^
		//0x0400 : ���Œ�
		//0x0800 : ���[�J����
		//0x1000 : ������ό`
		//0x2000 : �O���e�ό`
		unsigned short				boneFlags;

		Vector3						tailPosition;
		int							connectBoneIndex;

		InheritBone					inheritBone;
		BoneFixedAxis				fixedAxis;
		BoneLocalCoordinate			boneAxis;
		BoneExternalParent			externalParent;
		BoneIK						ikData;
	};

	enum class MorphType
	{
		Group,
		Vertex,
		Bone,
		UV,
		UVadd1,
		UVadd2,
		UVadd3,
		UVadd4,
		Material,
		Flip,
		Impulse
	};

	struct GroupMorph
	{
		unsigned int index;
		float influence;
	};

	struct VertexMorph
	{
		unsigned int vertexIndex;
		Vector3 translation;
	};

	struct BoneMorph
	{
		unsigned int boneIndex;
		Vector3 translation;
		Vector4 rotation;
	};

	struct UVMorph
	{
		unsigned int vertexIndex;
		Vector4 uvOffset;
	};

	struct MaterialMorph
	{
		int materialIndex;//-1�͑S�ގ��ΏۊO
		unsigned char offsetcalType;//0:��Z 1:���Z
		Vector4 diffuse;
		Vector3 specular;
		float specularity;
		Vector3 ambient;
		Vector4 edgeColor;
		float edgeSize;
		Vector4 textureTint;
		Vector4 environmentTint;
		Vector4 toonTint;
	};

	struct Flip {
		int morphIndex;
		unsigned char localFlag;
		Vector3 velocity;
		Vector3 torque;
	};

	struct Impulus {
		int	rigidBodyIndex;
		unsigned char	localFlag;
		Vector3 velocity;
		Vector3 torque;
	};

	union MorphData
	{
		VertexMorph vertexMorph;
		UVMorph uvMorph;
		BoneMorph boneMorph;
		MaterialMorph materialMorph;
		GroupMorph groupMorph;
		MorphData()  {};
		~MorphData() {}
		MorphData(const MorphData &) {};;
	};

	struct PMXMorph {
		std::wstring nameLocal;
		std::wstring nameUniversal;
		unsigned char controllPanel;
		unsigned char morphType;
		unsigned int morphCount;
		std::vector<MorphData> morphs;

	};

	struct DisplayData
	{
		unsigned char type;
		unsigned int index;
	};

	struct PMXDiplay
	{
		std::wstring nameLocal;
		std::wstring nameUniversal;
		unsigned char flags; //0;normal 1:specialFrame
		unsigned int count;
		std::vector<DisplayData> datas;
	};

	struct PMXRigidBody
	{
		std::wstring nameLocal;
		std::wstring nameUniversal;

		unsigned int boneIndex;//��

		unsigned char groupID;
		unsigned short nonCollisionGroup;
		unsigned char shape;

		Vector3 size;
		Vector3 pos;
		Vector3 rotation;//���W�A��

		float mass;//����
		float moveAttenuation;//�ړ�����
		float rotationDamping;//��]����
		float boundPower;//������(�W��?
		float frictionPower;//���C��(�W��?

		unsigned char phisicsMode;//0:�{�[���Ǐ](static) 1:�������Z(dynamic) 2:�������Z + Bone�ʒu���킹
	};

	struct SixDof
	{
		unsigned int rigidAIndex;
		unsigned int rigidBIndex;

		Vector3 pos;
		Vector3 rotation;

		Vector3 posMinimum;
		Vector3 posMaximum;
		Vector3 rotationMinimum;
		Vector3 rotationMaximum;

		Vector3 positionSpring;
		Vector3 rotationSpring;
	};
	enum class JointType {
		Spring6Dof,
		SixDof,
		P2P,
		ConeTwist,
		Slider,
		Hinge
	};
	struct PMXJoint
	{
		std::wstring nameLocal;
		std::wstring nameUniversal;

		unsigned char type;//pmx2.0�ł�0�̂�
		SixDof spring;
	};

	//���f���̏�����񂪓��������̂ŁA������R���o�[�g���ă��f�������
	class PMXModel
	{
	private:
	public:
		std::string					modelPath;
		PMXHeader					header;
		std::vector<PMXVertex>		vertcies;
		PMXSurface					surfaces;
		PMXTexturePath				paths;
		std::vector<PMXMaterial>	materials;
		std::vector<PMXBone>		bones;
		std::vector<PMXMorph>		morphDatas;
		std::vector<PMXDiplay>		displayInfo;
		std::vector<PMXRigidBody>	rigidBodies;
		std::vector<PMXJoint>		joints;
	private:
	public:
		PMXModel();
		virtual ~PMXModel();
	};

}