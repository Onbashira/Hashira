#pragma once
namespace Hashira {
	class Emitter;
	class L1Buffer;
	class D3D12Device;

	//シーン
	class EmitterDataBinary
	{

	public:

	private:

		unsigned int _emtCount;

		UINT64 _offset;

		//GPUに配置するバイナリ本体
		std::unique_ptr<L1Buffer> _bin;

	public:

		EmitterDataBinary();

		~EmitterDataBinary();

		HRESULT Initialize(std::shared_ptr<D3D12Device> device, UINT64 memorySize);

		void DiscardMemory();

	private:

	};

}