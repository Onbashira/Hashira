#pragma once
#include "Engine/Source/ParticleSystem/Emitter/Emitter.h"

namespace Hashira {
	class Emitter;
	class L0Buffer;
	class D3D12Device;

	//シーン単位でのエミッタヘッダのメモリを管理

	class EmitterHeaderBinary
	{
	public:
		
	private:
		
		int _emtHeaderCount;

		std::vector<GPUEmitterHeader> _headers;

		std::unique_ptr<L0Buffer> _bin;

	public:
		EmitterHeaderBinary();
		~EmitterHeaderBinary();

		HRESULT Initialize(std::shared_ptr<D3D12Device> device,UINT64 memorySize);

		void Write(CPUEmitterHeader* emtHeader);

		void Delete(int emtIndex);

		void DiscardMemory();

	private:

	};

}