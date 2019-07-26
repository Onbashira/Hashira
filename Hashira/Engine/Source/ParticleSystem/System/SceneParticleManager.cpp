#include "stdafx.h"
#include "SceneParticleManager.h"


Hashira::SceneParticleManager::SceneParticleManager()
{
}


Hashira::SceneParticleManager::~SceneParticleManager()
{
}

HRESULT Hashira::SceneParticleManager::Initialize(D3D12Device* device, const SceneEffectParticleInitializeInfo& initializeInfo)
{
	return E_NOTIMPL;
}

void Hashira::SceneParticleManager::AddEffect(const ParticleEffect& effectPt)
{
}

void Hashira::SceneParticleManager::UpdateParticles(RenderContext& rc)
{
}

void Hashira::SceneParticleManager::DrawParticles(RenderContext& rc)
{
}

void Hashira::SceneParticleManager::FixParticle(RenderContext& rc)
{
}

void Hashira::SceneParticleManager::Discard()
{
}
