#include "ModelConverter.h"

#include "Engine/Source/CommandList/CommandList.h"
#include <memory>
#include <vector>
#include <map>
#include <memory>


constexpr unsigned int ToonMapTextureNum = 11;
constexpr float MODEL_INIT_ROTATION = 0.0f;
//�Œ�l
const std::string ToonMapPath[ToonMapTextureNum] =
{
	"Data/toon/toon0.bmp",
	"Data/toon/toon01.bmp",
	"Data/toon/toon02.bmp",
	"Data/toon/toon03.bmp",
	"Data/toon/toon04.bmp",
	"Data/toon/toon05.bmp",
	"Data/toon/toon06.bmp",
	"Data/toon/toon07.bmp",
	"Data/toon/toon08.bmp",
	"Data/toon/toon09.bmp",
	"Data/toon/toon10.bmp",

};
const std::string NullTexturePath = "NULL_TEXTURE";

void CreatePMDBoneTree(K3D::MMDBoneTree* skelton, std::vector<K3D::PMDBone>& bonesInfo, K3D::MMDBoneNode * node);

void CreatePMXBoneTree(K3D::MMDBoneTree* skelton, std::vector<K3D::PMXBone>& bonesInfo, K3D::MMDBoneNode * node, std::vector<K3D::MMDIKData>& ikBoneIndex);

K3D::ModelConverter::ModelConverter()
{
}


K3D::ModelConverter::~ModelConverter()
{
}


std::shared_ptr<K3D::MMDModelResourceData> K3D::ModelConverter::ConvertPMDModel(std::shared_ptr<PMDModel> model)
{
	std::shared_ptr<K3D::MMDModelResourceData> convertModel = std::make_shared<MMDModelResourceData>();
	convertModel->_modelPath = model->modelPath;

	Matrix rotMat = Matrix::CreateRotationY(DegToRad(MODEL_INIT_ROTATION));

	//���_�R���o�[�g
	{
		convertModel->_vertexes.resize(model->vertices.size());
		convertModel->_deformationData.resize(model->vertices.size());

		for (size_t i = 0; i < convertModel->_vertexes.size(); i++) {
			convertModel->_vertexes[i].pos = Vector3::Transform(model->vertices[i].pos, rotMat);
			convertModel->_vertexes[i].normal = Vector3::Transform(model->vertices[i].normal, rotMat);
			convertModel->_vertexes[i].texCoord = std::move(Vector2(model->vertices[i].texCoord.x, model->vertices[i].texCoord.y));
			convertModel->_vertexes[i].deformType = MMDWeightDeformType::BDEF2;
			convertModel->_vertexes[i].deformation.bdef2.boneIndex01 = model->vertices[i].boneIndex[0];
			convertModel->_vertexes[i].deformation.bdef2.boneIndex02 = model->vertices[i].boneIndex[1];
			convertModel->_vertexes[i].deformation.bdef2.boneWeight01 = model->vertices[i].boneWeight;
			convertModel->_vertexes[i].deformation.bdef2.boneWeight02 = 1.0f - model->vertices[i].boneWeight;

		}

		////GPU�n���p�̕ό`�f�[�^�̍\�z
		//{
		//	convertModel->_deformation.Create(convertModel->_vertexes.size(), convertModel->_vertexes.size() * sizeof(MMDWeightDeform), &convertModel->_deformationData[0]);
		//}
	}
	//	�C���f�b�N�X���X�g�R���o�[�g
	{
		convertModel->_indexList.list.resize(model->verticesIndex.size());
		for (size_t i = 0; i < convertModel->_indexList.list.size(); i++) {
			convertModel->_indexList.list[i] = model->verticesIndex[i];
		}
	}
	//�}�e���A���R���o�[�g
	{
		//�^���I�ȃe�N�X�`���e�[�u��
		std::map<std::string, int> textureTable;
		convertModel->_materials.resize(model->materials.size());
		for (size_t i = 0; i < convertModel->_materials.size(); i++) {
			convertModel->_materials[i].ambient = Vector3(0.4f, 0.4f, 0.4f);
			convertModel->_materials[i].diffuse = Vector4(Vector3(model->materials[i].diffuse), 1.0);
			convertModel->_materials[i].specular = Vector4(Vector3(model->materials[i].specular), model->materials[i].power);
			convertModel->_materials[i].emissive = model->materials[i].emissive;
			//�ʏ�e�N�X�`��
			if (textureTable.find(model->materials[i].textureName) == textureTable.end()) {
				//�e�N�X�`�����Ȃ������Ȃ��
				convertModel->_materials[i].textureTableIndex = static_cast<unsigned int>(textureTable.size());
				textureTable[model->materials[i].textureName] = static_cast<unsigned int>(textureTable.size());

			}
			else {
				//��������Ȃ��
				convertModel->_materials[i].textureTableIndex = textureTable[model->materials[i].textureName];
			}

			//�X�t�B�A�e�N�X�`�����[�h
			//(����͐����킹�̂��߂ɓK���ȕ����������
			//���[�_�[�̂ق��Ɏ�����������������Z�b�g���Ƀ��[�h�����NULL�e�N�X�`�������[�h����B�j
			if (textureTable.find(NullTexturePath) == textureTable.end()) {
				//�e�N�X�`�����Ȃ������Ȃ��
				convertModel->_materials[i].sphereIndex = static_cast<unsigned int>(textureTable.size());
				textureTable[NullTexturePath] = static_cast<unsigned int>(textureTable.size());

			}
			else {
				//��������Ȃ��
				convertModel->_materials[i].sphereIndex = textureTable[NullTexturePath];
			}
			//Sphere�}�b�v�͂Ȃ��c�Ƃ������Ή����Ȃ��c���Ƃ��Ȃ�
			convertModel->_materials[i].sphereIndex = -1;
			convertModel->_materials[i].sphereBlendType = static_cast<int>(MMDSphereBlendType::NONE);
			//�g�D�[���e�N�X�`��
			if (textureTable.find(ToonMapPath[model->materials[i].toonIndex]) == textureTable.end()) {
				//�e�N�X�`�����Ȃ������Ȃ�� 
				convertModel->_materials[i].toonIndex = static_cast<unsigned int>(textureTable.size());
				textureTable[ToonMapPath[model->materials[i].toonIndex]] = static_cast<unsigned int>(textureTable.size());

			}
			else {
				//��������Ȃ��
				convertModel->_materials[i].toonIndex = textureTable[ToonMapPath[model->materials[i].toonIndex]];
			}
			convertModel->_materials[i].surfaceCount = model->materials[i].faceVertex;

		}
		//�e�N�X�`���̎擾
		{
			std::vector<std::string> paths;
			paths.reserve(model->materials.size());

			for (std::map<std::string, int>::iterator itr = textureTable.begin(); itr != textureTable.end(); itr++) {

				paths.push_back(Util::GetRelativeTexturePath(model->modelPath, itr->first));

			}
			convertModel->_texturePaths = std::move(paths);
			TextureLoader::GetInstance().LoadModelTexture(D3D12System::GetInstance().GetCommandList("CommandList"), &D3D12System::GetInstance().GetMasterCommandQueue(), model->modelPath, convertModel->_texturePaths);

		}
	}

	//�X�P���g���̃c���[��W�J
	{
		//���[�g�{�[���̒�`�itransform��Pos�ƈ�v
		{
			convertModel->_boneTree = std::make_shared<MMDBoneTree>();
			convertModel->_boneTree->boneNum = static_cast<unsigned int>(model->bonesInfo.size());
			convertModel->_boneTree->rootBone.pos = model->bonesInfo.at(0).boneHeadPosition;
			convertModel->_boneTree->rootBone.parentIndex = model->bonesInfo.at(0).parentIndex;
			convertModel->_boneTree->rootBone.rotation = Quaternion::CreateIdentity();
			convertModel->_boneTree->rootBone.index = 0; // ���ׂĂ̐e�i���[�g�I�u�W�F�N�g�j

			convertModel->_boneTree->boneAccessor[model->bonesInfo.at(0).boneName] = &convertModel->_boneTree->rootBone;
			convertModel->_boneTree->boneNameAccessor[0] = model->bonesInfo.at(0).boneName;
			convertModel->_boneTree->bonesMatrix.resize(model->bones.size());
			std::fill(convertModel->_boneTree->bonesMatrix.begin(), convertModel->_boneTree->bonesMatrix.end(), Matrix::CreateIdentity());


		}
		CreatePMDBoneTree(convertModel->_boneTree.get(), model->bonesInfo, &convertModel->_boneTree->rootBone);
	}
	//�X�P���g����IK���R���o�[�g
	{
		convertModel->_boneTree->modelIKdata.resize(model->ikData.size());
		for (size_t i = 0; convertModel->_boneTree->modelIKdata.size(); i++) {
			convertModel->_boneTree->modelIKdata[i].targetBoneIndex = model->ikData[i].targetboneIndex;
			convertModel->_boneTree->modelIKdata[i].boneIndex = model->ikData[i].boneIndex;
			convertModel->_boneTree->modelIKdata[i].loopCount = model->ikData[i].recursiveCount;
			convertModel->_boneTree->modelIKdata[i].limitRadian = model->ikData[i].controlWeight;

			convertModel->_boneTree->modelIKdata[i].ikLinks.resize(model->ikData[i].childBone.size());
			//IK�Ƀ����N���Ă���{�[�����̔������
			for (size_t j = 0; j < convertModel->_boneTree->modelIKdata[i].ikLinks.size(); i++) {
				convertModel->_boneTree->modelIKdata[i].ikLinks[j].boneIndex = model->ikData[i].childBone[j];
				//PMD�ɂ͂��̍��ڂ͂Ȃ��̂ō����Ă���B�ꉞ�Ȃ��ł�����
				//convertModel->_modelIKdata[i].ikLinks[j].hasLimit = �H;
				convertModel->_boneTree->modelIKdata[i].ikLinks[j].hasLimit = false;
				//�����ŉ����l����l���L�����ǂ����𔻒f���Ȃ���΂����Ȃ��̂����APMD�ɂ͂��̃f�[�^���Ȃ��̂��c������
				//�������^�Ȃ�Ή����l����l�����݂��Ă���Ƃ��āA�ݒ���s��
				if (convertModel->_boneTree->modelIKdata[i].ikLinks[j].hasLimit) {
					convertModel->_boneTree->modelIKdata[i].ikLinks[j].limits.maximumAngle = Vector3::zero;
					convertModel->_boneTree->modelIKdata[i].ikLinks[j].limits.minimamAngle = Vector3::zero;
				}
				else {
					convertModel->_boneTree->modelIKdata[i].ikLinks[j].limits.maximumAngle = Vector3::zero;
					convertModel->_boneTree->modelIKdata[i].ikLinks[j].limits.minimamAngle = Vector3::zero;
				}
			}

		}
	}
	return convertModel;
};

std::shared_ptr<K3D::MMDModelResourceData> K3D::ModelConverter::ConvertPMXModel(std::shared_ptr<PMXModel> model)
{
	std::shared_ptr<MMDModelResourceData> convertModel = std::make_shared<MMDModelResourceData>();
	convertModel->_modelPath = model->modelPath;

	//���_�R���o�[�g
	{
		Matrix rotMat = Matrix::CreateRotationY(DegToRad(MODEL_INIT_ROTATION));

		convertModel->_vertexes.resize(model->vertcies.size());
		convertModel->_deformationData.resize(model->vertcies.size());

		for (size_t i = 0; i < convertModel->_vertexes.size(); i++) {
			convertModel->_vertexes[i].pos = Vector3::Transform(model->vertcies[i].position, rotMat);
			convertModel->_vertexes[i].normal = Vector3::Transform(model->vertcies[i].normal, rotMat);;
			convertModel->_vertexes[i].texCoord = model->vertcies[i].uv;
			convertModel->_vertexes[i].deformType = static_cast<MMDWeightDeformType>(model->vertcies[i].weightDeformType);
			convertModel->_vertexes[i].deformation.sdef = model->vertcies[i].weightDeform.sdef;

			//convertModel->_deformationData[i].deformType = static_cast<int>(model->vertcies[i].weightDeformType);
			//convertModel->_deformationData[i].boneIndex01 = model->vertcies[i].weightDeform.qdef.boneIndex01;
			//convertModel->_deformationData[i].boneIndex02 = model->vertcies[i].weightDeform.qdef.boneIndex02;
			//convertModel->_deformationData[i].boneIndex03 = model->vertcies[i].weightDeform.qdef.boneIndex03;
			//convertModel->_deformationData[i].boneIndex04 = model->vertcies[i].weightDeform.qdef.boneIndex04;
			//convertModel->_deformationData[i].boneWeight01 = model->vertcies[i].weightDeform.qdef.boneWeight01;
			//convertModel->_deformationData[i].boneWeight02 = model->vertcies[i].weightDeform.qdef.boneWeight02;
			//convertModel->_deformationData[i].boneWeight03 = model->vertcies[i].weightDeform.qdef.boneWeight03;
			//convertModel->_deformationData[i].boneWeight04 = model->vertcies[i].weightDeform.qdef.boneWeight04;
			//convertModel->_deformationData[i].c = model->vertcies[i].weightDeform.sdef.c;
			//convertModel->_deformationData[i].r0 = model->vertcies[i].weightDeform.sdef.r0;
			//convertModel->_deformationData[i].r1 = model->vertcies[i].weightDeform.sdef.r1;

		}

		////GPU�n���p�̕ό`�f�[�^�̍\�z
		//{
		//	convertModel->_deformation.Create(convertModel->_vertexes.size(), convertModel->_vertexes.size() * sizeof(MMDWeightDeform), &convertModel->_deformationData[0]);
		//}
	}
	//	�C���f�b�N�X���X�g�R���o�[�g
	{
		convertModel->_indexList.list.resize(model->surfaces.surfaceIndex.size());
		for (size_t i = 0; i < convertModel->_indexList.list.size(); i++) {
			convertModel->_indexList.list[i] = model->surfaces.surfaceIndex[i];
		}
	}
	//�}�e���A���R���o�[�g
	{
		//�e�N�X�`���p�X�i�e�N�X�`���e�[�u���j
		//��{�e�N�X�`������у��[�U�[�g�D�[���e�N�X�`���A���[�U�[�X�t�B�A�}�b�v�̖��O���擾
		std::vector<std::string> textureTable;
		textureTable.resize(model->paths.path.size());
		for (size_t i = 0; i < model->paths.path.size(); i++) {
			textureTable[i] = Util::GetRelativeTexturePath(model->modelPath, Util::WStringToString(model->paths.path[i]));
		}
		//�O�̂��߂̃k���e�N�X�`���ւ̃p�X����������
		textureTable.push_back(NullTexturePath);
		//�k���e�N�X�`���ւ̃C���f�b�N�X�������Ă���
		int nullTextureIndex = static_cast<int>(textureTable.size() - 1);
		convertModel->_materials.resize(model->materials.size());
		for (size_t i = 0; i < convertModel->_materials.size(); i++) {
			convertModel->_materials[i].ambient = model->materials[i].ambientColor;
			convertModel->_materials[i].diffuse = model->materials[i].diffuseColor;
			convertModel->_materials[i].specular = Vector4(Vector3(model->materials[i].specularColor), model->materials[i].specularStrength);
			convertModel->_materials[i].emissive = Vector3::zero;
			//�g�D�[���e�N�X�`��
			//�ʃg�D�[���t���O���^�Ȃ��
			if (model->materials[i].toonReference) {
				convertModel->_materials[i].toonIndex = model->materials[i].toonValue.individualToon;
			}
			else {
				convertModel->_materials[i].toonIndex = model->materials[i].toonValue.individualToon;
				//���̎��_�ŋ��L�g�D�[���}�b�v���g�����Ƃ͊m�肵���Ȃ��B�s���l�`�F�b�N���s���B
				//�����s���l���������Ă����Ȃ��NullTexture����
				if (convertModel->_materials[i].toonIndex != -1) {
					if (std::find(textureTable.begin(), textureTable.end(), ToonMapPath[convertModel->_materials[i].toonIndex]) == textureTable.end()) {
						textureTable.push_back(ToonMapPath[convertModel->_materials[i].toonIndex]);
					}
				}
				else {
					textureTable.push_back(NullTexturePath);
					convertModel->_materials[i].toonIndex = nullTextureIndex;
				}
			}
			//PMX�̏ꍇ�A�ʏ�e�N�X�`���C���f�b�N�X�͂��łɍ���Ă���B�g�D�[���}�b�v�C���f�b�N�X�����l�ł���B
			//���ꂼ��e�N�X�`���n�͕s���l�i�|�P�j��������Ă���ꍇ�̏������K�v�B
			//�����Ȃ������Ȃ�΁ANull�e�N�X�`�������[�h����悤�ɂ���B

			//�ʏ�e�N�X�`��
			//�s���ȃe�N�X�`���C���f�b�N�X����Ȃ�������
			if (model->materials[i].textureIndex != -1) {
				convertModel->_materials[i].textureTableIndex = model->materials[i].textureIndex;
			}
			else {
				//�C���f�b�N�X�̒�����NULL�e�N�X�`�����e�N�X�`���p�X�ւԂ�����
				convertModel->_materials[i].textureTableIndex = nullTextureIndex;
			}
			//Sphere�}�b�v��PMX���ƑΉ�
			//�X�t�B�A�e�N�X�`�����[�h
			if (model->materials[i].environmentTextureIndex != -1) {
				convertModel->_materials[i].sphereIndex = model->materials[i].environmentTextureIndex;
				convertModel->_materials[i].sphereBlendType = static_cast<int>(model->materials[i].environmentBlendFlags);

			}
			else {
				convertModel->_materials[i].sphereIndex = nullTextureIndex;
				convertModel->_materials[i].sphereBlendType = static_cast<int>(MMDSphereBlendType::NONE);

			}

			//�T�[�t�F�X�J�E���g�̃Z�b�g
			convertModel->_materials[i].surfaceCount = model->materials[i].surfaceCount;
		}

		//�e�N�X�`���p�X�̎擾����у��\�[�X�o�b�t�@�̊m��
		{
			convertModel->_texturePaths = std::move(textureTable);
			TextureLoader::GetInstance().LoadModelTexture(D3D12System::GetInstance().GetCommandList("CommandList"), &D3D12System::GetInstance().GetMasterCommandQueue(), model->modelPath, convertModel->_texturePaths);

		}
	}

	//�X�P���g���̃c���[��W�J �����IK�f�[�^�𐶐�
	{
		std::vector<MMDIKData> ikDataMap;
		//���[�g�{�[���̒�`�itransform��Pos�ƈ�v
		{
			convertModel->_boneTree = std::make_shared<MMDBoneTree>();
			convertModel->_boneTree->boneNum = static_cast<unsigned int>(model->bones.size());

			convertModel->_boneTree->rootBone.pos = model->bones.at(0).bonePosition;
			convertModel->_boneTree->rootBone.parentIndex = model->bones.at(0).parentBoneIndex;
			convertModel->_boneTree->rootBone.rotation = Quaternion::CreateIdentity();
			convertModel->_boneTree->rootBone.index = 0; // ���ׂĂ̐e�i���[�g�I�u�W�F�N�g�j
			convertModel->_boneTree->boneAccessor[Util::WStringToString(model->bones.at(0).boneNameLocal)] = &convertModel->_boneTree->rootBone;
			convertModel->_boneTree->boneNameAccessor[0] = Util::WStringToString(model->bones.at(0).boneNameLocal);

			convertModel->_boneTree->bonesMatrix.resize(model->bones.size());
			std::fill(convertModel->_boneTree->bonesMatrix.begin(), convertModel->_boneTree->bonesMatrix.end(), Matrix::CreateIdentity());
		}
		CreatePMXBoneTree(convertModel->_boneTree.get(), model->bones, &(convertModel->_boneTree->rootBone), ikDataMap);

		//�X�P���g����IK���R���o�[�g
		{
			convertModel->_boneTree->modelIKdata.reserve(ikDataMap.size());
			for (std::vector<MMDIKData>::iterator itr = ikDataMap.begin(); itr != ikDataMap.end(); itr++) {
				convertModel->_boneTree->modelIKdata.push_back(*itr);
			}
		}
	}

	///���Ή�
	////���[�t
	////����
	////�W���C���g
	///

	return convertModel;
}

//�ϊ������I���W�i�����f���f�[�^��������f���f�[�^���쐬
std::shared_ptr<K3D::MMDModel> K3D::ModelConverter::ExtructMMDModel(std::weak_ptr<PipelineStateObject> pso, std::weak_ptr<RootSignature> rootSignature, std::weak_ptr<CommandList> list, std::shared_ptr<MMDModelResourceData> modelResource)
{
	std::shared_ptr<K3D::MMDModel> extructedModel(std::make_shared<K3D::MMDModel>());
	std::vector<MMDRendererMaterial> materials;
	extructedModel->_bundleList.SetName("BundleList");
	//�}�e���A�����o
	materials.resize(modelResource->_materials.size());
	for (UINT i = 0; i < materials.size(); i++) {
		materials[i].diffuse = modelResource->_materials[i].diffuse;
		materials[i].ambient = modelResource->_materials[i].ambient;
		materials[i].specular = modelResource->_materials[i].specular;
		materials[i].emissive = modelResource->_materials[i].emissive;
		materials[i].sphereBlendType = modelResource->_materials[i].sphereBlendType;
	}
	//���_���o
	extructedModel->_vertexes.reserve(modelResource->_vertexes.size());
	for (UINT i = 0; i < extructedModel->_vertexes.size(); i++) {
		extructedModel->_vertexes[i].pos = modelResource->_vertexes[i].pos;
		extructedModel->_vertexes[i].normal = modelResource->_vertexes[i].normal;
		extructedModel->_vertexes[i].texCoord = modelResource->_vertexes[i].texCoord;
	}
	extructedModel->_resourceData = modelResource;
	{
		//�I���W�i���f�[�^����C���f�b�N�X�o�b�t�@�A���_�o�b�t�@���쐬�A����Ƀ��\�[�X�ɑ΂��Ė��O��t����B�i�f�o�b�O���Ɋm�F���₷�����邽�߁j�G
		extructedModel->GetMeshBuffer().GetIBO().Create(modelResource->_indexList.list.size() * sizeof(UINT), sizeof(UINT), &modelResource->_indexList.list[0]);
		extructedModel->GetMeshBuffer().GetIBO().SetName("MMDModelIndexData");

		//extructedModel->GetDefaultVBO()->Create(extructedModel->GetDefaultVertexData().size() * sizeof(Vertex3D), sizeof(Vertex3D), &extructedModel->_vertexes[0]);
		extructedModel->GetMeshBuffer().InitializeVBO(extructedModel->_vertexes.size() * sizeof(Vertex3D), sizeof(Vertex3D), &extructedModel->_vertexes[0]);
		extructedModel->GetMeshBuffer().GetVBO().SetName("MMDModelVertexData");
	}
	//�}�e���A���o�b�t�@���쐬�B256Byte�A���C���Ń}�e���A���������̈���m��
	extructedModel->GetMeshHeap().GetMaterialBufffer().Create(materials.size() * Util::Alignment256Bytes(sizeof(MMDRendererMaterial)));
	extructedModel->GetMeshHeap().GetMaterialBufffer().SetName("ModelMaterialBuffer");

	//�q�[�v�̍쐬�E�e���̃X�^�[�g�|�C���g�̐ݒ�
	{
		unsigned int heapSize = static_cast<unsigned int>(1 + modelResource->_materials.size() + modelResource->_texturePaths.size()); //�J�����������������f���ɕK�v�ȏ��̐������Z�B�i�ό`���A�}�e���A�����A�e�N�X�`�����j
		extructedModel->GetMeshHeap().GetHeap().Create(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, heapSize);
		extructedModel->GetMeshHeap().GetHeap().SetName("ModelHeap");

		//extructedModel->_transformStartPoint = 0;
		extructedModel->GetMeshHeap().SetTransformDescStartIndex(0);
		extructedModel->GetMeshHeap().SetMaterialDescStartIndex(extructedModel->GetMeshHeap().GetTransformDescStartIndex() + 1);
		extructedModel->GetMeshHeap().SetTextureDescStartIndex(extructedModel->GetMeshHeap().GetMaterialDescStartIndex() + static_cast<unsigned int>(modelResource->_materials.size()));

		//extructedModel->_textureStartPoint = extructedModel->_materialStartPoint + static_cast<unsigned int>(modelResource->_materials.size());

	}
	//�ό`���̍\�z����т���ɑ΂��Ă̌������`
	{

		extructedModel->InitalizeTransformBuffer(Util::Alignment256Bytes(sizeof(Transform)));
		extructedModel->_transformBuffer.SetName("MMDModelTransformBuffer");

		D3D12_CONSTANT_BUFFER_VIEW_DESC view{};

		view.BufferLocation = extructedModel->_transformBuffer.GetResource()->GetGPUVirtualAddress();
		view.SizeInBytes = static_cast<unsigned int>(Util::Alignment256Bytes(sizeof(Transform)));

		extructedModel->_transformBuffer.CreateView(view, extructedModel->GetMeshHeap().GetHeap().GetCPUHandle(extructedModel->GetMeshHeap().GetTransformDescStartIndex()));

		//�ʒu���̏����ݒ�
		{
			extructedModel->SetRotation(Quaternion::CreateIdentity());
			extructedModel->SetPos(Vector3::zero);
			extructedModel->SetScale(Vector3::one);
			extructedModel->TransformUpdate();
			extructedModel->Update();
		}
	}
	//�}�e���A���̏������݂ƃf�X�N���v�^�쐬
	{
		auto gpuPtr = extructedModel->GetMeshHeap().GetMaterialBufffer().GetResource()->GetGPUVirtualAddress();
		D3D12_CONSTANT_BUFFER_VIEW_DESC view{};
		UINT writeOffset = 0;
		for (UINT i = 0; i < materials.size(); i++) {

			//�m�ۂ������\�[�X�̗̈�ɏ�������
			extructedModel->GetMeshHeap().GetMaterialBufffer().Update(&materials[i], sizeof(MMDRendererMaterial), writeOffset);

			//�f�X�N���v�^������AGPU�Ƀ~���[�����O���ꂽ�̈���X���C�h����
			view.BufferLocation = gpuPtr;
			view.SizeInBytes = static_cast<unsigned int>(Util::Alignment256Bytes(sizeof(MMDRendererMaterial)));

			extructedModel->GetMeshHeap().GetMaterialBufffer().CreateView(view, extructedModel->GetMeshHeap().GetHeap().GetCPUHandle(extructedModel->GetMeshHeap().GetMaterialDescStartIndex() + i));

			gpuPtr += static_cast<unsigned int>(Util::Alignment256Bytes(sizeof(MMDRendererMaterial)));
			writeOffset += static_cast<unsigned int>(Util::Alignment256Bytes(sizeof(MMDRendererMaterial)));
		}
	}
	//�e�N�X�`�����\�[�X�̃r���[�̍쐬
	{
		TextureLoader::GetInstance().LoadModelTexture(D3D12System::GetInstance().GetCommandList("CommandList"), &D3D12System::GetInstance().GetMasterCommandQueue(), extructedModel->GetMeshHeap().GetHeap(), extructedModel->GetMeshHeap().GetTextureDescStartIndex(), modelResource->_modelPath, modelResource->_texturePaths);
	}

	//���[�_�[�ɃZ�b�g���ꂽ�R�}���h���X�g�����f���ɃZ�b�g
	extructedModel->_commandList = list;

	//���f���Ƀp�C�v���C���ƃ��[�g�V�O�l�`�����Z�b�g
	extructedModel->SetPipelineState(pso);
	extructedModel->SetRootSignature(rootSignature);
	extructedModel->RegisterToBundle();
	return extructedModel;
}

void CreatePMXBoneTree(K3D::MMDBoneTree* skelton, std::vector<K3D::PMXBone>& bonesInfo, K3D::MMDBoneNode * node, std::vector<K3D::MMDIKData>& ikDataMap)
{
	Matrix rotMat = Matrix::CreateRotationY(DegToRad(MODEL_INIT_ROTATION));

	for (auto info = bonesInfo.begin(); info != bonesInfo.end(); info++) {
		if (node->index == info->parentBoneIndex) {
			K3D::MMDBoneNode newNode;
			//�{�[���̈ʒu��180�x��]
			newNode.pos = Vector3::Transform(info->bonePosition, rotMat);
			newNode.parentIndex = node->index;
			newNode.rotation = Quaternion::CreateIdentity();
			newNode.index = static_cast<UINT>(std::distance(bonesInfo.begin(), info));

			newNode.boneOption.inheritBone = info->inheritBone;
			newNode.boneOption.externalParentBoneIndex = info->externalParent;
			newNode.boneOption.fixidAxis = info->fixedAxis;
			newNode.boneOption.localAxis = info->boneAxis;

			newNode.boneOptionFlags = info->boneFlags;

			node->childrenBone.push_back(newNode);
			skelton->boneNameAccessor[newNode.index] = K3D::Util::WStringToString(info->boneNameLocal);

			//�{�[����IK�����������Ă���Ȃ�
			if ((static_cast<int>(info->boneFlags) & 0x0020) == 0x0020) {
				K3D::MMDIKData ikData;
				ikData.boneIndex = newNode.index;
				ikData.limitRadian = info->ikData.limitRadian;
				ikData.loopCount = info->ikData.loopCount;
				ikData.targetBoneIndex = info->ikData.targetIndex;
				ikData.ikLinks.resize(info->ikData.ikLinks.size());
				for (size_t i = 0; i < ikData.ikLinks.size(); i++) {
					ikData.ikLinks[i].boneIndex = info->ikData.ikLinks[i].boneIndex;
					ikData.ikLinks[i].hasLimit = info->ikData.ikLinks[i].hasLimit;
					ikData.ikLinks[i].limits = info->ikData.ikLinks[i].limits;
				}
				ikDataMap.push_back(ikData);
			}
		}
	}

	for (unsigned int i = 0; i < node->childrenBone.size(); i++)
	{
		skelton->boneAccessor[skelton->boneNameAccessor[node->childrenBone[i].index]] = &node->childrenBone[i];

	}

	for (unsigned int i = 0; i < node->childrenBone.size(); i++)
	{
		//�ċA
		CreatePMXBoneTree(skelton, bonesInfo, &node->childrenBone[i], ikDataMap);
	}
}

void CreatePMDBoneTree(K3D::MMDBoneTree* skelton, std::vector<K3D::PMDBone>& bonesInfo, K3D::MMDBoneNode * node)
{

	Matrix rotMat = Matrix::CreateRotationY(DegToRad(MODEL_INIT_ROTATION));

	for (auto info = bonesInfo.begin(); info != bonesInfo.end(); info++) {
		if (node->index == info->parentIndex) {
			K3D::MMDBoneNode newNode;
			//�{�[���̈ʒu��180�x��]
			newNode.pos = Vector3::Transform(info->boneHeadPosition, rotMat);
			newNode.parentIndex = node->index;
			newNode.rotation = Quaternion::CreateIdentity();
			newNode.index = static_cast<UINT>(std::distance(bonesInfo.begin(), info));

			newNode.boneOption = {};

			newNode.boneOptionFlags = 0x0002;
			node->childrenBone.push_back(std::move(newNode));
			skelton->boneAccessor[info->boneName] = &node->childrenBone.back();
		}
	}

	for (unsigned int i = 0; i < node->childrenBone.size(); i++)
	{
		skelton->boneAccessor[skelton->boneNameAccessor[node->childrenBone[i].index]] = &node->childrenBone[i];

	}

	for (auto& child : node->childrenBone)
	{
		//�ċArekure
		CreatePMDBoneTree(skelton, bonesInfo, &child);
	}
}
