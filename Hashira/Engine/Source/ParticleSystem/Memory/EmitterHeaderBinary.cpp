#include "stdafx.h"
#include "EmitterHeaderBinary.h"
#include "Engine/Source/ParticleSystem/Emitter/Emitter.h"
#include "Engine/Source/Resource/L0Buffer.h"
#include "Engine/Source/Device/D3D12Device.h"

Hashira::EmitterHeaderBinary::EmitterHeaderBinary()
{
}


Hashira::EmitterHeaderBinary::~EmitterHeaderBinary()
{
	this->DiscardMemory();
}

HRESULT Hashira::EmitterHeaderBinary::Initialize(std::shared_ptr<D3D12Device> device, UINT64 memorySize)
{
	_bin = std::make_unique<L0Buffer>();
	auto hr = _bin->Initialize(device , memorySize,D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
#ifdef _DEBUG
	SystemLogger::GetInstance().Log(LOG_LEVEL::Debug, hr);
#endif

	return hr;
}

void Hashira::EmitterHeaderBinary::Write(CPUEmitterHeader * emtHeader)
{
	if ((this->_bin->GetResourceDesc().Width-sizeof(GPUEmitterHeader)) >= static_cast<UINT64>(_emtHeaderCount * sizeof(GPUEmitterHeader))) {
#ifdef _DEBUG
		DEBUG_LOG(std::string("EmtHeaderBin is Full"));
#endif // _DEBUG

		return;
	}


	GPUEmitterHeader header = emtHeader->CreateGPUEmitterHeader();
	_bin->Update(&header,sizeof(GPUEmitterHeader), _emtHeaderCount * sizeof(GPUEmitterHeader));
	++_emtHeaderCount;
	return;

}

void Hashira::EmitterHeaderBinary::Delete(int emtIndex)
{
	
}

void Hashira::EmitterHeaderBinary::DiscardMemory()
{
	_bin->Discard();
	_headers.clear();
	_headers.shrink_to_fit();

}
