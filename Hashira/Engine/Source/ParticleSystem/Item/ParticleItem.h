#pragma once
namespace Hashira {

	//�v�f���Ƃ̃A�C�e��
	class ParticleItem
	{
	public:

		ParticleItem() {};

		virtual ~ParticleItem() {};

		// �v�f�̒l�ւ�VOID�^�|�C���^
		virtual void* GetValue() = 0;
		
		//�p�[�e�B�N���P�ʂ̃f�[�^�̍X�V��GPU�����ǁA�G�~�b�^�P�ʂ̃f�[�^�̍X�V��ParticleItem���s��
		virtual void UpdateItem() = 0;

		//GPU�Ƀ}�b�s���O����ۂɏ������ރT�C�Y�̎擾
		virtual unsigned int GetElementSize() = 0;

		
	};
}