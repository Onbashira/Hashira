#pragma once

//CS�ɓ��������ʁA�Ԑڕ`����s�����߂�Args��ۑ����Ă������߂�L1�ɒu��������

namespace Hashira {

	class L1Buffer;
	class Emitter;
	class D3D12Device;

	struct IAParticleSpawnCountInfo {
		unsigned int PreParticleCount;
		unsigned int CurParticleCount;
	};

	struct ParticleDispatchIndirectArg {

		unsigned int X;
		unsigned int Y;
		unsigned int Z;

	};

	struct ParticleDrawIndirectArg {

		unsigned int VertexCountPerInstance;
		unsigned int InstanceCount;
		unsigned int StartVertexLocation;
		unsigned int StartInstanceLocation;

	};


	class ParticleIndirectArgs
	{
	public:

	private:

		std::unique_ptr<L1Buffer> _indirectArgsBuffer;

	public:

		ParticleIndirectArgs();

		~ParticleIndirectArgs();

		HRESULT Initalize(std::shared_ptr<D3D12Device>& device,UINT64 emitterCount);

		std::unique_ptr<L1Buffer>& GetBuffer() {
			return _indirectArgsBuffer;
		};

		void Discard();

	private:



	};

};