#pragma once
#include "../ParticleItem.h"
#include "Engine/Source/Component/Transform/Transform.h"
namespace Hashira {

	class Transform;

	//�G�~�b�^�̈ʒu�A��]�A�X�P�[�����T�|�[�g����f�t�H���g�A�C�e��
	class TransformItem : public ParticleItem
	{
	private:

		Transform _transform;

	public:
		TransformItem();
		~TransformItem();

		// �v�f�̒l�ւ�VOID�^�|�C���^
		virtual void* GetValue();

		//�p�[�e�B�N���P�ʂ̃f�[�^�̍X�V��GPU�����ǁA�G�~�b�^�P�ʂ̃f�[�^�̍X�V��ParticleItem���s��
		virtual void UpdateItem();

		//GPU�Ƀ}�b�s���O����ۂɏ������ރT�C�Y�̎擾
		virtual unsigned int GetElementSize();

	private:
	};

}