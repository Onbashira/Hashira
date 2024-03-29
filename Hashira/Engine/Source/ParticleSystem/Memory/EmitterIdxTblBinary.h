#pragma once
namespace Hashira {
	class Emitter;
	class L0Buffer;
	class D3D12Device;

	//シーン単位でエミッタインデックスのメモリ部を管理
	class EmitterIdxTblBinary
	{
	public:

	private:

		//オフセット
		size_t _offset;

		std::vector<int> _emtIdxTbl;

		//GPUに配置するバイナリ本体
		std::unique_ptr<L0Buffer> _bin;

		//

	public:
		
		EmitterIdxTblBinary();
		
		~EmitterIdxTblBinary();

		HRESULT Initialize(std::shared_ptr<D3D12Device> device, UINT64 memorySize);

		void Write(int emtIndex);

		void Delete(int emtIndex);

		void Sort();

		void DiscardMemory();
	};

}