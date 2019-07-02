#pragma once
namespace Hashira {
	class L1Buffer;
	class D3D12Device;
	class ParticleHeaderBinary
	{
	public:

	private:

		std::unique_ptr<L1Buffer> _headerBuffer;
	public:
		ParticleHeaderBinary();
		~ParticleHeaderBinary();

		HRESULT Initalize(std::shared_ptr<D3D12Device>& device, size_t  bufferSize);


		std::unique_ptr<L1Buffer>& GetBuffer() {
			return _headerBuffer;
		};

		void DiscardMemory();

	};

}