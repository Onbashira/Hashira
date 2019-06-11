#pragma once
namespace Hashira {
	class Emitter;
	class Resource;
	class D3D12Device;


	class EmitterIdxTblBinary
	{
	public:

	private:

	public:
		
		EmitterIdxTblBinary();
		
		~EmitterIdxTblBinary();

		void Initialize(UINT64 memorySize);

		void Write(int emtIndex);

		void Delete(int emtIndex);

		void DiscardMemory();
	};

}