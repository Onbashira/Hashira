#include "stdafx.h"
#include "ParticleBinary.h"
#include "Engine/Source/Resource/L1Buffer.h"


Hashira::ParticleBinary::ParticleBinary()
{
}


Hashira::ParticleBinary::~ParticleBinary()
{
}

HRESULT Hashira::ParticleBinary::Initalize(std::shared_ptr<D3D12Device>& device, size_t bufferSize)
{
	_ptBuffer = std::make_unique<L1Buffer>();
	auto hr = _ptBuffer->Initialize(device, bufferSize, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
#ifdef _DEBUG
	SystemLogger::GetInstance().Log(LOG_LEVEL::Debug, hr);
#endif

	return hr;
}

void Hashira::ParticleBinary::DiscardMemory()
{
	this->_ptBuffer->Discard();
}
