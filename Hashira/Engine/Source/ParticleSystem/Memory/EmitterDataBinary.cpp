#include "stdafx.h"
#include "EmitterDataBinary.h"
#include "Engine/Source/ParticleSystem/Emitter/Emitter.h"
#include "Engine/Source/Buffer/L1Buffer.h"
#include "Engine/Source/Device/D3D12Device.h"

Hashira::EmitterDataBinary::EmitterDataBinary() : _offset(0)
{
}


Hashira::EmitterDataBinary::~EmitterDataBinary()
{
}

HRESULT Hashira::EmitterDataBinary::Initialize(std::shared_ptr<D3D12Device> device, UINT64 memorySize)
{
	_bin = std::make_unique<L1Buffer>();
	auto hr = _bin->Initialize(device, memorySize,D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
#ifdef _DEBUG
	SystemLogger::GetInstance().Log(LOG_LEVEL::Debug, hr);
#endif

	return hr;
}

void Hashira::EmitterDataBinary::DiscardMemory()
{
	_bin->Discard();
}
