#pragma once
namespace Hashira {
	class L1Buffer;
	class D3D12Device;
	class ParticleIdxTblBinary
	{
	public:

	private:

		std::unique_ptr<L1Buffer> _idxTblBuffer;
	public:
		ParticleIdxTblBinary();
		~ParticleIdxTblBinary();

		HRESULT Initalize(std::shared_ptr<D3D12Device>& device, size_t  bufferSize);

		std::unique_ptr<L1Buffer>& GetBuffer() {
			return _idxTblBuffer;
		};

		void DiscardMemory();

	};

}