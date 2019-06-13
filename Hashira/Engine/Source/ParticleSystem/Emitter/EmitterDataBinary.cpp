#include "stdafx.h"
#include "EmitterDataBinary.h"
#include "Engine/Source/ParticleSystem/Emitter/Emitter.h"
#include "Engine/Source/Resource/Resource.h"
#include "Engine/Source/Device/D3D12Device.h"

Hashira::EmitterDataBinary::EmitterDataBinary() : _offset(0)
{
}


Hashira::EmitterDataBinary::~EmitterDataBinary()
{
}

HRESULT Hashira::EmitterDataBinary::Initialize(std::shared_ptr<D3D12Device> device, UINT64 memorySize)
{
	D3D12_HEAP_PROPERTIES prop = {};
	prop.Type = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_CUSTOM;
	prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_WRITE_COMBINE;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_L0;
	prop.CreationNodeMask = device->GetDevice()->GetNodeCount();;
	prop.VisibleNodeMask = 1;

	D3D12_RESOURCE_DESC desc = {};
	desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Alignment = 0;
	desc.Width = memorySize;
	desc.Height = 1;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 1;
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Layout = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	desc.Flags = D3D12_RESOURCE_FLAG_NONE;

	_bin = std::make_unique<Resource>();
	auto hr = _bin->Initialize(device, prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET);
#ifdef _DEBUG
	SystemLogger::GetInstance().Log(LOG_LEVEL::Debug, hr);
#endif

	return hr;
}

void Hashira::EmitterDataBinary::Write(Emitter * emitter)
{
}

void Hashira::EmitterDataBinary::Delete(int emtIndex)
{
}

void Hashira::EmitterDataBinary::DiscardMemory()
{
	_bin->Discard();
}
