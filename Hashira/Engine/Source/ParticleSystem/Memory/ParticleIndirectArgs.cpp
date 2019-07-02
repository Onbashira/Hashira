#include "stdafx.h"
#include "ParticleIndirectArgs.h"
#include "Engine/Source/Resource/L1Buffer.h"
#include "Engine/Source/Device/D3D12Device.h"

Hashira::ParticleIndirectArgs::ParticleIndirectArgs()
{
}


Hashira::ParticleIndirectArgs::~ParticleIndirectArgs()
{
}

HRESULT Hashira::ParticleIndirectArgs::Initalize(std::shared_ptr<D3D12Device>& device, UINT64 emitterCount)
{
	constexpr size_t memorySize = sizeof(Hashira::IAParticleSpawnCountInfo) + sizeof(Hashira::ParticleDispatchIndirectArg);
	_indirectArgsBuffer = std::make_unique<L1Buffer>();
	auto hr = _indirectArgsBuffer->Initialize(device, memorySize + sizeof(Hashira::ParticleDrawIndirectArg) * emitterCount, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS );
#ifdef _DEBUG
	SystemLogger::GetInstance().Log(LOG_LEVEL::Debug, hr);
#endif

	return hr;
}

void Hashira::ParticleIndirectArgs::DiscardMemory()
{
	_indirectArgsBuffer->Discard();
}
