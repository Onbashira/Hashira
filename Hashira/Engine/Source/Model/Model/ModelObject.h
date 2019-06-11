#pragma once
#include "Engine/Source/Component/GameObject/GameObject.h"
#include "Engine//Source/Mesh/ModelMesh.h"
#include "Engine/Source/DrawableComponent/DrawableComponent.h"
#include <vector>

namespace K3D {
	class ModelMesh;

	//�x�[�X�̃��f���I�u�W�F�N�g�@��{���b�V�����A�`�悷�邽�߂̃q�[�v�A�`�悷�邽�߂̃p�C�v���C�����A�Q�[���I�u�W�F�N�g����������
	class ModelObject : public GameObject
	{

	public:

	private:


	protected:

		std::unique_ptr<ModelMesh> _meshData;

	private:

	protected:
		//�o���h�����X�g�Ɍp������L�̃h���[�R�[����ςލ�Ƃ�����
		virtual void RegisterToBundle() = 0;
	public:
		//���f���̃A�b�v�f�[�g
		virtual void Update() = 0;
		//�`�掞�Ăяo���֐��̍쐬������
		virtual void Draw() = 0;

		virtual void SetPipelineState(std::weak_ptr<K3D::PipelineStateObject> pipelineState) = 0;

		virtual void SetRootSignature(std::weak_ptr<K3D::RootSignature> rootSignature) = 0;

		virtual void SetCommandList(std::shared_ptr<K3D::CommandList> masterCommandList) = 0;

		virtual void BindingShaderObject() = 0;

		virtual void BindingShaderObjectToBundle() = 0;

		virtual void Draw() = 0;

		virtual void Discard() = 0;

		ModelObject();
		virtual ~ModelObject();
	};
}
