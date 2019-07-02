#pragma once

#include "Engine/Source/Resource/MultipleBuffer.h"

namespace Hashira {

	class Emitter;
	class CommonItem;
	class EmitterBinary;
	class EmitterHeaderBinary;
	class EmitterIdxTblBinary;
	class ParticleBinary;
	class ParticleHeaderBinary;
	class ParticleIdxTblBinary;
	class D3D12Device;
	class GameHeap;
	class CommandList;
	struct Descriptor;

	struct EffectInitializeInfo {
		UINT64 emtBinarySize = 0;
		UINT64 ptBinarySize = 0;
		UINT64 emtHeaderNum = 0;
		UINT64 emtCount = 0;

	};

	//�e�o�C�i���A���[�g�w�b�_�A�G�t�F�N�g�ɕK�v�ȃp�C�v���C���A�V�F�[�_�[�ݒ��ێ�
	class ParticleEffect {


	public:

	protected:

		size_t _totalSize;

		std::vector<std::unique_ptr<Emitter>> _emitters;

		std::vector<std::shared_ptr<Descriptor>> _descriptors;

		std::weak_ptr<GameHeap> _gameheap;

	public:

		ParticleEffect();

		virtual ~ParticleEffect();

		virtual void Initialize(std::shared_ptr < D3D12Device>& device);

		virtual void Update();

		//�G�~�b�^�̓o�^
		virtual void RegistEmitter(std::unique_ptr<Emitter>& emitter);

		size_t GetEffectSize();

		virtual void Discard();

	protected:


	private:

	};

}