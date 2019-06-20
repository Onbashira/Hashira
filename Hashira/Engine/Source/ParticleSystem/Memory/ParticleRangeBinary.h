#pragma once
namespace Hashira {
	class L1Buffer;
	class D3D12Device;

	struct Particle {
		UINT64 AliveBegin = 0;
		UINT64 AliveEnd = 0;
		UINT64 DeadBegin = 0;
		UINT64 DeadEnd = 0;
	};


	class ParticleRangeBinary
	{
	public:

	private:
	
		std::unique_ptr<L1Buffer> _rangeBuffer;

	public:

		ParticleRangeBinary();

		~ParticleRangeBinary();

		HRESULT Initalize(std::shared_ptr<D3D12Device>& device, size_t  bufferSize);

		std::unique_ptr<L1Buffer>& GetBuffer() {
			return _rangeBuffer;
		};
	};

}