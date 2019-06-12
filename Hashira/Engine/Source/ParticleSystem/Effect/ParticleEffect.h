#pragma once

namespace Hashira {

	class Emitter;
	class CommonItem;
	class ParticleBinary;
	class EmitterBinary;
	class D3D12Device;

	//各バイナリ、ルートヘッダ、エフェクトに必要なパイプライン、シェーダー設定を保持
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