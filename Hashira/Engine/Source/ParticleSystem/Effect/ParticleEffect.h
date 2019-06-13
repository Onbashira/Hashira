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
		UINT64 EmtBinarySize = 0;
		UINT64 PtBinarySize = 0;
		UINT64 EmtHeaderNum = 0;
		UINT64 EmtCount = 0;

	};

	//各バイナリ、ルートヘッダ、エフェクトに必要なパイプライン、シェーダー設定を保持
	class ParticleEffect {


	public:

	protected:

		std::shared_ptr<EmitterBinary> _emtBin;

		std::unique_ptr<EmitterHeaderBinary> _emtHeaderBin;

		std::unique_ptr<EmitterIdxTblBinary> _emtIdxTblBin;

		std::shared_ptr<ParticleBinary> _ptBin;

		std::unique_ptr<ParticleHeaderBinary> _ptHeaderBin;

		std::unique_ptr<ParticleIdxTblBinary> _ptIdxTblBin;

		std::vector<std::unique_ptr<Emitter>> _emitters;

		std::vector<std::shared_ptr<Descriptor>> _descriptors;

		std::weak_ptr<GameHeap> _gameheap;

	public:

		ParticleEffect();

		virtual ~ParticleEffect();

		virtual void Initialize(std::shared_ptr < D3D12Device>& device, std::shared_ptr<GameHeap>& gameheap);

		virtual void Initialize(std::shared_ptr < D3D12Device>& device, std::shared_ptr<GameHeap>& gameheap);


		virtual void Update() = 0;

		virtual void Draw() = 0;

		virtual void Terminate() = 0;

		//エミッタの登録
		virtual void RegistEmitter(std::unique_ptr<Emitter>& emitter);

	protected:


		virtual void FixPtIdxTbl();

		virtual void FixPtBin();

		virtual void Discard();

	private:

	};

}