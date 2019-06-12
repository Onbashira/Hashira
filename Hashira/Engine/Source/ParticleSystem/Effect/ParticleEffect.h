#pragma once

namespace Hashira {

	class Emitter;
	class CommonItem;
	class ParticleBinary;
	class EmitterBinary;
	class D3D12Device;

	//�e�o�C�i���A���[�g�w�b�_�A�G�t�F�N�g�ɕK�v�ȃp�C�v���C���A�V�F�[�_�[�ݒ��ێ�
	class ParticleEffect {


	public:
	private:

		std::shared_ptr<EmitterBinary> _emtBin;

		std::shared_ptr<ParticleBinary> _ptBin;

	public:

		ParticleEffect();
		virtual ~ParticleEffect();

		virtual void Update() = 0;

		virtual void Terminate() = 0;

	private:

	};

}