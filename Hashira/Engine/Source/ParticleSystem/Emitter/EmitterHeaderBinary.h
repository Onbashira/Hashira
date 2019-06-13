#pragma once
#include "Engine/Source/ParticleSystem/Emitter/Emitter.h"

namespace Hashira {
	class Emitter;
	class Resource;
	class D3D12Device;

	class EmitterHeaderBinary
	{
	public:
		
	private:
		
		int _emtHeaderCount;

		std::vector<GPUEmitterHeader> _headers;

		std::unique_ptr<Resource> _bin;

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