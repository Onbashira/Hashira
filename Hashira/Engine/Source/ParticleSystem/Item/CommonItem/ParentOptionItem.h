#pragma once
#include "../ParticleItem.h"

namespace Hashira {

	//�e�A�C�e������A�C�e���̎擾(���󖳌��j
	class ParentOptionItem :
		public ParticleItem
	{
	public:

	private:

	public:
		ParentOptionItem();
		virtual ~ParentOptionItem();

		// �v�f�̒l�ւ�VOID�^�|�C���^
		virtual void* GetValue();

		//�p�[�e�B�N���P�ʂ̃f�[�^�̍X�V��GPU�����ǁA�G�~�b�^�P�ʂ̃f�[�^�̍X�V��ParticleItem���s��
		virtual void UpdateItem();

		//GPU�Ƀ}�b�s���O����ۂɏ������ރT�C�Y�̎擾
		virtual unsigned int GetElementSize();
	private:
	};

}