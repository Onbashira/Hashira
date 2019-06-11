#pragma once
#include "Engine/Source/Model/Model/ModelObject.h"

namespace K3D {

	class GraphicsCommandList;
	class PipelineStateObject;
	class ModelConverter;

	struct MMDRendererMaterial
	{
		//�g�U���ˌ� w = alpha
		Vector4 diffuse;
		//���ʔ��ˌ�	w = pow
		Vector4 specular;
		//����
		Vector3 ambient;
		//���Ȕ����F
		Vector3 emissive;
		//�X�t�B�A�}�b�v�̃u�����f�B���O�^�C�v
		int	sphereBlendType;
	};

	//�Q�[���{�҂Ŏg�����f��
	class MMDModel :
		public ModelObject
	{
		friend class ModelConverter;
		friend class ModelPool;

		enum SHADER_ROOT_PARAMATER_INDEX {
			SHADER_ROOT_PARAMATER_INDEX_CAMERA = 0,
			SHADER_ROOT_PARAMATER_INDEX_TRANSFORM,
			SHADER_ROOT_PARAMATER_INDEX_MATERIAL,
			SHADER_ROOT_PARAMATER_INDEX_TEXTURE,
			SHADER_ROOT_PARAMATER_INDEX_TOON_MAP,
			SHADER_ROOT_PARAMATER_INDEX_SPHERE_TEXTURE,
		};
	private:
		
	protected:
		//���f���̃I���W�i���f�[�^
		std::weak_ptr<MMDModelResourceData>	_resourceData;
		//�A�j���[�^�[�R���g���[���[
		Animator							_animator;

	public:

	private:
		//BDEF1�v�Z
		Matrix CulcBDEF1(K3D::WeightDeform& deform);
		//BDEF2�v�Z
		Matrix CulcBDEF2(WeightDeform& deform);
		//BDEF4�v�Z
		Matrix CulcBDEF4(WeightDeform& deform);
		//SDEF�v�Z
		std::pair<Vector3, float> CulcSDEF(WeightDeform& deform);
		//QDEF�v�Z
		Matrix CulcQDEF(WeightDeform& deform);
		//SDEF�d�݌v�Z
		std::pair<float, float> CulcSDEFWaight(Vector3 r0, Vector3 r1);

	protected:
		//�g�����X�t�H�[���A�b�v�f�[�g
		void	TransformUpdate();
		//�A�j���[�V�����̃A�b�v�f�[�g
		void	AnimationUpdate();
		//�{�[���̃A�b�v�f�[�g
		void	SkeltonReset();
		//�A�j���[�V�����f�[�^�[�̓o�^�ƃ��[�h
		void	SetAnimationData(std::string motionPath);
		//�o���h���ւ̓o�^
		virtual void RegisterToBundle()override;

	public:
		//�g�����X�t�H�[���̃A�b�v�f�[�g
		virtual void Update()override;
		//���X�g��Draw�R�}���h��ςݍ���(�W�I���g�������_�����O�j
		virtual void Draw()override;

		virtual Animator& Animator();

		MMDModel();
		virtual ~MMDModel();
	};
}