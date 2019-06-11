#pragma once
#include "Engine/Source/Math/Math.h"
#include <vector>

//�V�F�[�_�[�ɓn�����_���
namespace K3D {
	struct ModelTranportVertex {
		Vector3 pos;
		Vector3 normal;
		Vector2 texCoord;
	};

	//�V�F�[�_�[�n�����߂̌ÓT�I�V�F�[�f�B���O�}�e���A��
	struct ModelTrasnpotingMaterial {
		Vector3 diffuse;
		float	diffusePower;
		Vector3 specular;
		float	specularPower;

	};

	//�{�[���c���[�̃m�[�h���`
	struct ModelBoneNode {
		//���g�̃C���f�b�N�X
		unsigned short index;
		//�e�̃C���f�b�N�X
		unsigned short parentIndex;
		//�{�[���̈ʒu
		Vector3 pos;
		//
		Vector3 parentPos;
		///�q��
		std::vector<ModelBoneNode> children;
	};

	struct ModelBoneTree {
		ModelBoneNode* rootNode;
	};

	//�v�Z�p�{�[���c���[���܂ރ{�[���̏��Q
	struct ModelBoneInformation {
		///�{�[������
		unsigned int boneNum;
	};

	//�V�F�[�_�[�ɓn�����߂̃e�N�X�`���f�[�^
	struct ModelTextureData {

	};

	//��ʉ��������f���̃��\�[�X
	class ModelResource
	{

	public:

	private:
		//�ύX���Ȃ�
		IndexBuffer	_indexBuffer;
		//�ύX�����\��������i�����{�[���̏�񂪕K�v�j
		VertexBuffer _vertexBuffer;
		//�ύX����Ȃ��͂��B���邩��
		ConstantBuffer _materialBuffer;
		//�{�[�����̓V�F�[�_�ɓn���Ȃ��H�̂ŁA�����|�[�Y�̃{�[���z��������Ă����i�c���[�\���j
		std::vector<int>	_indexList;
		//
		std::vector<Vertex3D> _vertexes;
		// 
	public:

		ModelResource();

		~ModelResource();

	private:

	};

}