#pragma once
#include "../ParticleItem.h"

namespace Hashira {
	class RotateAnimItem : public ParticleItem
	{
	public:
		RotateAnimItem();
		~RotateAnimItem();

		// �v�f�̒l�ւ�VOID�^�|�C���^
		virtual void* GetValue()override;

		//�p�[�e�B�N���P�ʂ̃f�[�^�̍X�V��GPU�����ǁA�G�~�b�^�P�ʂ̃f�[�^�̍X�V��ParticleItem���s��
		virtual void UpdateItem() = 0;

		//GPU�Ƀ}�b�s���O����ۂɏ������ރT�C�Y�̎擾
		virtual unsigned int GetElementSize()override;
	};

}