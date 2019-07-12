#include "stdafx.h"
#include "ParticleIdxTblBinary.h"
#include "Engine/Source/Buffer/L1Buffer.h"


Hashira::ParticleIdxTblBinary::ParticleIdxTblBinary()
{
}


Hashira::ParticleIdxTblBinary::~ParticleIdxTblBinary()
{
}

HRESULT Hashira::ParticleIdxTblBinary::Initalize(std::shared_ptr<D3D12Device>& device, size_t bufferSize)
{
	_idxTblBuffer = std::make_unique<L1Buffer>();
	auto hr = _idxTblBuffer->Initialize(device, bufferSize, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
#ifdef _DEBUG
	SystemLogger::GetInstance().Log(LOG_LEVEL::Debug, hr);
#endif

	return hr;
}

void Hashira::ParticleIdxTblBinary::DiscardMemory()
{
	this->_idxTblBuffer->Discard();
}
