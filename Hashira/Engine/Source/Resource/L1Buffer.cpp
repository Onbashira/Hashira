#include "stdafx.h"
#include "L1Buffer.h"
#include "Engine/Source/Device/D3D12Device.h"


Hashira::L1Buffer::L1Buffer()
{
}


Hashira::L1Buffer::~L1Buffer()
{
}


HRESULT Hashira::L1Buffer::Initialize(std::shared_ptr<D3D12Device>& device, size_t bufferSize, const D3D12_RESOURCE_STATES & state, D3D12_CLEAR_VALUE * clearValue)
{
	D3D12_HEAP_PROPERTIES prop = {};

	prop.Type = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_CUSTOM;
	prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_L1;
	prop.CreationNodeMask = device->GetDevice()->GetNodeCount();
	prop.VisibleNodeMask = 1;

	D3D12_RESOURCE_DESC desc = {};
	desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Alignment = 0;
	desc.Width = bufferSize;
	desc.Height = 1;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 1;
	desc.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Layout = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_UNKNOWN;
	desc.Flags = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;

	auto hr = Resource::Initialize(prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, desc, state, clearValue);

#ifdef _DEBUG
	SystemLogger::GetInstance().Log(LOG_LEVEL::Debug, hr);
#endif

	return hr;
}