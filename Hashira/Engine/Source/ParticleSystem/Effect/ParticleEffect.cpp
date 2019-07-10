#include "stdafx.h"
#include "ParticleEffect.h"
#include "Engine/Source/ParticleSystem/Emitter/Emitter.h"

Hashira::ParticleEffect::ParticleEffect()
{
}


Hashira::ParticleEffect::~ParticleEffect()
{
	Discard();
}

void Hashira::ParticleEffect::Initialize(std::shared_ptr<D3D12Device>& device)
{
}

void Hashira::ParticleEffect::Update()
{
	for (auto& emt : _emitters)
	{
		emt->UpdateItems();
	}
}

void Hashira::ParticleEffect::RegistEmitter(std::unique_ptr<Emitter>& emitter)
{
	this->_emitters.push_back(std::move(emitter));
}

size_t Hashira::ParticleEffect::GetEffectSize()
{
	return _totalSize;
}

void Hashira::ParticleEffect::Discard()
{
	_emitters.clear();
}
