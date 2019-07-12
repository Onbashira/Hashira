#include "stdafx.h"
#include "ParticleRangeBinary.h"
#include "Engine/Source/Buffer/L1Buffer.h"


Hashira::ParticleRangeBinary::ParticleRangeBinary()
{
}


Hashira::ParticleRangeBinary::~ParticleRangeBinary()
{
}

HRESULT Hashira::ParticleRangeBinary::Initalize(std::shared_ptr<D3D12Device>& device, size_t  bufferSize)
{
	_rangeBuffer = std::make_unique<L1Buffer>();
	auto hr = _rangeBuffer->Initialize(device, bufferSize, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
#ifdef _DEBUG
	SystemLogger::GetInstance().Log(LOG_LEVEL::Debug, hr);
#endif

	return hr;
}

void Hashira::ParticleRangeBinary::DiscardMemory()
{
	this->_rangeBuffer->Discard();
}
