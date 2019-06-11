#include "PMDModel.h"
#include "PMDLoader.h"
#include "Engine/Source/Texture/TextureLoader.h"
#include "Engine/Source/Texture/TextureManager.h"

K3D::PMDLoader::PMDLoader()
{
}

K3D::PMDLoader::~PMDLoader()
{
}


void K3D::PMDLoader::LoadVertex(FILE * fp, PMDModel * model)
{
	unsigned int vertexCount = 0;
	fread(&vertexCount, sizeof(vertexCount), 1, fp);

	model->vertices.resize(vertexCount);

	for (auto& vertex : model->vertices) {
		fread(&vertex, sizeof(PMDVertex), 1, fp);
	}
}

void K3D::PMDLoader::LoadIndexList(FILE * fp, PMDModel * model)
{
	unsigned int indexCount = 0;
	fread(&indexCount, sizeof(indexCount), 1, fp);

	model->verticesIndex.resize(indexCount);

	for (auto& index : model->verticesIndex) {
		fread(&index, sizeof(unsigned int), 1, fp);
	}
}

void K3D::PMDLoader::LoadMaterial(FILE * fp, PMDModel * model)
{
	unsigned int materialCount = 0;
	fread(&materialCount, sizeof(unsigned int), 1, fp);

	model->materials.resize(materialCount);
	for (auto& material : model->materials) {
		fread(&material, sizeof(PMDMaterial), 1, fp);
	}
}

void K3D::PMDLoader::LoadSkelton(FILE * fp, PMDModel * model)
{
	unsigned short		boneCount = 0;
	fread(&boneCount, sizeof(unsigned int), 1, fp);

	model->bonesInfo.resize(boneCount);
	for (auto& bone : model->bonesInfo) {
		fread(&bone, sizeof(PMDBone), 1, fp);
	}
	model->bones.resize(boneCount);
	for (unsigned int i = 0; i < boneCount; ++i) {

		model->bones[i].head = model->bonesInfo[i].boneHeadPosition;
		model->bones[i].tail = model->bonesInfo[model->bonesInfo[i].tailIndex].boneHeadPosition;
	}
}

void K3D::PMDLoader::LoadIK(FILE * fp, PMDModel * model)
{
	unsigned short ikDataCount = 0;
	unsigned int	loadSize = sizeof(unsigned int) * 2 + sizeof(unsigned int) + sizeof(unsigned char) + sizeof(float);
	fread(&ikDataCount, sizeof(unsigned short), 1, fp);

	model->ikData.resize(ikDataCount);

	for (auto& ikData : model->ikData) {
		fread(&ikData, loadSize, 1, fp);
		ikData.childBone.resize(ikData.chainLength);

		for (auto& child : ikData.childBone) {
			fread(&child, sizeof(unsigned short), 1, fp);
		}
	}
}

void K3D::PMDLoader::LoadMorph(FILE * fp, PMDModel * model)
{
}

std::shared_ptr<K3D::PMDModel> K3D::PMDLoader::Load(std::string path)
{

	//�t�@�C���ǂݍ���
	if (path.length() == 0) {
		return nullptr;
	}

	FILE* fp;
	std::shared_ptr<PMDModel> model;
	fopen_s(&fp, path.c_str(), "rb");
	if ((fp) == NULL) {

		return nullptr;
	}

	model = std::make_shared<PMDModel>();

	//�w�b�_�ǂݍ���
	PMDHeader header{};
	fread(&header, sizeof(PMDHeader), 1, fp);

	//�ꉞ�O�̂���283�o�C�g�ʒu�ɃV�[�N���Ƃ�sizeof(PMDHeader)�ł�������i�P�o�C�g�A���C�������g���Ă邩��)
	fseek(fp, 283, SEEK_SET);


	model->modelPath = path;
	//���_���X�g�ǂݍ���
	{
		LoadVertex(fp, model.get());
	}
	//���_�C���f�b�N�X�z��ǂݍ���
	{
		LoadIndexList(fp, model.get());
	}
	//�}�e���A���ǂݍ���
	{
		LoadMaterial(fp, model.get());
	}
	//�{�[���ǂݍ���
	{
		LoadSkelton(fp, model.get());
	}
	//IK�ǂݍ���
	{
		LoadIK(fp, model.get());
	}
	fclose(fp);
	return model;
}
