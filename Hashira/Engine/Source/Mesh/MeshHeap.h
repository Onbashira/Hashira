#pragma once
#include "Engine/Source/DescriptorHeap/Descriptor.h"
#include "Engine/Source/Buffer/ConstantBuffer.h"
#include <memory>
#include <vector>

namespace Hashira {

	class ShaderResource;
	class TextureObject;
	class CommandList;
	class ModelMesh;

	//Mesh�������_�����O���邽�߂̃N���X�B��ɃV�F�[�_�[�ɓ����邽�߂̃q�[�v���Ǘ�����
	class MeshHeap
	{
		friend class ModelMesh;
	public:
	protected:
	private:


		//!�ό`���f�X�N���v�^�̊J�n�ʒu (maybe 0)
		unsigned int _transformStartPoint;
		//!�}�e���A���f�X�N���v�^�̊J�n�ʒu (maybe 0+1 ~ x)
		unsigned int _materialStartPoint;
		//!�e�N�X�`���f�X�N���v�^�̊J�n�ʒu (maybe x+1 ~ y)
		unsigned int _textureStartPoint;
		//!�}�e���A�����̃o�b�t�@ �@�@
		Hashira::ConstantBuffer _materialBuffer;
		//!�e�N�X�`���ւ̎Q��
		std::vector<std::weak_ptr<Hashira::TextureObject>> _textureResource;
		
		//��{���C�A�E�g
		//1�J�����@2�g�����X�t�H�[���A3~���}�e���A�� (x+1)~y �e�N�X�`��

	public:

		MeshHeap();

		virtual ~MeshHeap();

		void SetTransformDescStartIndex(unsigned int startIndex = 0);

		void SetMaterialDescStartIndex(unsigned int startIndex = 1);

		void SetTextureDescStartIndex(unsigned int startIndex = 2);

		unsigned int GetTransformDescStartIndex();

		unsigned int GetTextureDescStartIndex();

		unsigned int GetMaterialDescStartIndex();

		ConstantBuffer& GetMaterialBufffer();

		std::vector<std::weak_ptr<Hashira::TextureObject>>& GetTexturesRef();

		std::weak_ptr<Hashira::TextureObject> GetTextureRef(unsigned int textureIndex);

		MeshHeap& AddTextureRef(std::weak_ptr<Hashira::TextureObject> textureRef);

		void Discard();

	protected:

	private:

	};
}

