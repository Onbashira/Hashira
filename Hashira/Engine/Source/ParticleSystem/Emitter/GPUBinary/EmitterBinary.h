#pragma once
namespace Hashira {
	class Emitter;
	class Resource;

	//エフェクト単位でのエミッタのバイナリ
	class EmitterBinary
	{

	public:

		//エミッタ数
		int _emtCount;

		

		//GPUに配置するバイナリ本体
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