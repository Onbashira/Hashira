#pragma once
namespace Hashira {
	class Emitter;
	class Resource;
	class D3D12Device;
	struct CPUEmitterHeader;

	class EmitterHeaderBinary
	{
	public:
		
	private:
		
		int _emtHeaderCount;

		std::unique_ptr<Resource> _bin;



	public:
		EmitterHeaderBinary();
		~EmitterHeaderBinary();

		void Initialize(UINT64 memorySize);

		void Write(CPUEmitterHeader* emtHeader);

		void Delete(int emtIndex);

		void DiscardMemory();

	private:

	};

}