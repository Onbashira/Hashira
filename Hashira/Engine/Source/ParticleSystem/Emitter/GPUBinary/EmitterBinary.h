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

	//�G�t�F�N�g�P�ʂł̃G�~�b�^�̃o�C�i��
	class EmitterBinary
	{

	public:

		//�G�~�b�^��
		int _emtCount;

		//�I�t�Z�b�g
		size_t _offset;

		std::vector<EmitterBinInfo> _binInfoVec;

		//GPU�ɔz�u����o�C�i���{��
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