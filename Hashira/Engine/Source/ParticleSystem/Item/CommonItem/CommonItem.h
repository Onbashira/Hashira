#pragma once
#include "./../ParticleItem.h"
namespace Hashira {
	class CommonItem :
		public ParticleItem
	{

	public:

	private:

	public:
		CommonItem();
		virtual ~CommonItem();

		// �v�f�̒l�ւ�VOID�^�|�C���^
		virtual void* GetValue();

		//�p�[�e�B�N���P�ʂ̃f�[�^�̍X�V��GPU�����ǁA��p����A�C�e���f�[�^�̍X�V��ParticleItem���s��
		virtual void UpdateItem();

		//GPU�Ƀ}�b�s���O����ۂɏ������ރT�C�Y�̎擾
		virtual unsigned int GetElementSize();


	};

};