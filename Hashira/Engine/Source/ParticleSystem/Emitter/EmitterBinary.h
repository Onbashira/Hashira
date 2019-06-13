#pragma once
namespace Hashira {
	class Emitter;
	class Resource;
	class D3D12Device;


	struct EmitterBinInfo
	{
		size_t Begin;
		size_t End;
	};

	//エフェクト単位でのエミッタのバイナリ
	class EmitterBinary
	{

	public:

		//エミッタ数
		int _emtCount;

		//オフセット
		size_t _offset;

		std::vector<EmitterBinInfo> _binInfoVec;

		//GPUに配置するバイナリ本体
		std::unique_ptr<Resource> _bin;


	private:

	public:

		EmitterBinary();

		~EmitterBinary();

		HRESULT Initialize(std::shared_ptr<D3D12Device> device, UINT64 memorySize);

		void Write(Emitter* emitter);

		void Delete(int emtIndex);

		void DiscardMemory();

	private:
	};

}