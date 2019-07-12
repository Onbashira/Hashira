#include "stdafx.h"
#include "ParticleHeaderBinary.h"
#include "Engine/Source/Buffer/L1Buffer.h"


Hashira::ParticleHeaderBinary::ParticleHeaderBinary()
{
}


Hashira::ParticleHeaderBinary::~ParticleHeaderBinary()
{
}

HRESULT Hashira::ParticleHeaderBinary::Initalize(std::shared_ptr<D3D12Device>& device, size_t bufferSize)
{
	_headerBuffer = std::make_unique<L1Buffer>();
	auto hr = _headerBuffer->Initialize(device, bufferSize, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
#ifdef _DEBUG
	SystemLogger::GetInstance().Log(LOG_LEVEL::Debug, hr);
#endif

	return hr;
}

void Hashira::ParticleHeaderBinary::DiscardMemory()
{	
	
	_headerBuffer->Discard();

}