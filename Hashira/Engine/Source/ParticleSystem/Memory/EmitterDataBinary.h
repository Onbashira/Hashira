#pragma once
namespace Hashira {
	class Emitter;
	class L1Buffer;
	class D3D12Device;

	//�V�[��
	class EmitterDataBinary
	{

	public:

	private:

		unsigned int _emtCount;

		UINT64 _offset;

		//GPU�ɔz�u����o�C�i���{��
		std::unique_ptr<L1Buffer> _bin;

	public:

		EmitterDataBinary();

		~EmitterDataBinary();

		HRESULT Initialize(std::shared_ptr<D3D12Device> device, UINT64 memorySize);

		void DiscardMemory();

	private:

	};

}