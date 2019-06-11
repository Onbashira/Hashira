#pragma once
namespace Hashira {
	class Emitter;
	class Resource;

	//�G�t�F�N�g�P�ʂł̃G�~�b�^�̃o�C�i��
	class EmitterBinary
	{

	public:

		//�G�~�b�^��
		int _emtCount;

		

		//GPU�ɔz�u����o�C�i���{��
		std::unique_ptr<Resource> _bin;


	private:

	public:
		
		EmitterBinary();
		
		~EmitterBinary();

		void Initialize(UINT64 memorySize);
		
		void Write(Emitter* emitter);
		
		void Delete(int emtIndex);

		void DiscardMemory();

	private:
	};

}