#pragma once
namespace Hashira {
	class Emitter;
	class L0Buffer;
	class D3D12Device;

	//�V�[��
	class EmitterDataBinary
	{

	public:

	private:

		UINT64 _offset;

		//GPU�ɔz�u����o�C�i���{��
		std::unique_ptr<L0Buffer> _bin;

	public:

		EmitterDataBinary();

		~EmitterDataBinary();

		HRESULT Initialize(std::shared_ptr<D3D12Device> device, UINT64 memorySize);

		void Write(Emitter* emitter);

		void Delete(int emtIndex);

		void DiscardMemory();

	private:

	};

}