#pragma once
#include "Engine/Source/ParticleSystem/Effect/ParticleEffect.h"

namespace Hashira {

	class D3D12Device;
	class ParticleEffect;
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
	class CommandList;
	class RenderContext;


	struct SceneEffectParticleInitializeInfo {
		size_t sceneEffectMaxCount; //シーンで発生させるエフェクトの最大数 
	};


	//シーン単位でパーティクルエフェクトを管理
	class SceneParticleManager
	{

	public:

	private:
		std::vector<std::shared_ptr<ParticleEffect>>	_sceneParticleEffects;
		std::shared_ptr<EmitterBinary>					_sceneEmtMemory;
		std::shared_ptr<EmitterDataBinary>				_sceneEmtDataMemory;
		std::shared_ptr<EmitterHeaderBinary>			_sceneEmtHeaderMemory;
		std::shared_ptr<EmitterIdxTblBinary>			_sceneEmtIdxTblMemory;
		std::shared_ptr<ParticleBinary>					_scenePtMemory;
		std::shared_ptr<ParticleHeaderBinary>			_scenePtHeaderMemory;
		std::shared_ptr<ParticleIdxTblBinary>			_scenePtIdxTblMemory;
		std::shared_ptr<ParticleRangeBinary>			_scenePtRangeMemory;
		std::shared_ptr<RenderContext>					_renderContext;
	public:

		SceneParticleManager();

		~SceneParticleManager();

		HRESULT Initialize(D3D12Device* device , const SceneEffectParticleInitializeInfo& initializeInfo);

		void AddEffect(const ParticleEffect& effectPt);

		void UpdateParticles(RenderContext& rc);

		void DrawParticles( RenderContext& rc);

		void FixParticle(RenderContext& rc);

		void Discard();

	private:



	};

}