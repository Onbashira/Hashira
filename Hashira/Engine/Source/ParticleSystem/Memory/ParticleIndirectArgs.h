#pragma once

//CS�ɓ��������ʁA�Ԑڕ`����s�����߂�Args��ۑ����Ă������߂�L1�ɒu��������

namespace Hashira {

	class L1Buffer;
	class Emitter;
	class D3D12Device;

	struct IAParticleSpawnCountInfo {
		unsigned int preParticleCount;
		unsigned int curParticleCount;
	};

	struct ParticleDispatchIndirectArg {

		unsigned int x;
		unsigned int y;
		unsigned int z;

	};

	struct ParticleDrawIndirectArg {

		unsigned int vertexCountPerInstance;
		unsigned int instanceCount;
		unsigned int startVertexLocation;
		unsigned int startInstanceLocation;

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

		void DiscardMemory();

	private:



	};

};