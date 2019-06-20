#pragma once
namespace Hashira {

	class EffectParticle;
	class EmitterBinary;
	class EmitterDataBinary;
	class EmitterHeaderBinary;
	class EmitterIdxTblBinary;
	class ParticleBinary;
	class ParticleHeaderBinary;
	class ParticleIdxTblBinary;
	class ParticleRangeBinary;
	class OfflineDescriptorHeap;
	class OnlineDescriptorHeap;
	class GameHeap;
	class CommandList;
	class RenderContext;


	struct SceneEffectParticleInitializeInfo {
		size_t SceneEffectMaxCount; //シーンで発生させるエフェクトの最大数
		std::shared_ptr<GameHeap>& Heap;
	};


	//シーン単位でパーティクルエフェクトを管理
	class SceneParticleManager
	{

	public:

	private:
		std::vector<std::unique_ptr<EffectParticle>> _sceneEffects;
		std::shared_ptr<EmitterBinary> _sceneEmtMemory;
		std::shared_ptr<EmitterDataBinary> _sceneEmtDataMemory;
		std::shared_ptr<EmitterHeaderBinary> _sceneEmtHeaderMemory;
		std::shared_ptr<EmitterIdxTblBinary> _sceneEmtIdxTblMemory;
		std::shared_ptr<ParticleBinary> _scenePtMemory;
		std::shared_ptr<ParticleHeaderBinary> _scenePtHeaderMemory;
		std::shared_ptr<ParticleIdxTblBinary> _scenePtIdxTblMemory;
		std::shared_ptr<ParticleRangeBinary> _scenePtRangeMemory;
		std::shared_ptr<RenderContext> _renderContext;
	public:

		SceneParticleManager();

		~SceneParticleManager();

		HRESULT Initialize(const SceneEffectParticleInitializeInfo& initializeInfo);

		void AddEffect(const EffectParticle& effectPt);

		void UpdateParticles();

		void DrawParticles();

		void FixParticle();

		void Discard();

		

	private:



	};

}