#include "ConstantBuffer.h"
#include "Engine/Source/CoreSystem/Framework.h"


Hashira::ConstantBuffer::ConstantBuffer()
{

}


Hashira::ConstantBuffer::~ConstantBuffer()
{
	Buffer::Discard();
}

HRESULT Hashira::ConstantBuffer::Initialize(std::shared_ptr<D3D12Device>& device, ULONG64 size)
{
	_initializeSize = size;
	size = Util::Alignment256Bytes(size);
	if (_resource.Get() != nullptr) {
		Buffer::Discard();
	}

	D3D12_HEAP_PROPERTIES prop = {};
	prop.Type = D3D12_HEAP_TYPE_UPLOAD;
	prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	prop.CreationNodeMask = 1;
	prop.VisibleNodeMask = 1;

	D3D12_RESOURCE_DESC desc = {};
	desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Alignment = 0;
	desc.Width = size;
	desc.Height = 1;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 1;
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Layout = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	desc.Flags = D3D12_RESOURCE_FLAG_NONE;

	auto hr = Buffer::Initialize(prop, D3D12_HEAP_FLAG_NONE, desc, D3D12_RESOURCE_STATE_GENERIC_READ);
	if (FAILED(hr)) {
		return E_FAIL;
	}

	//システム終了までUnmapしない

	hr = Map(0, nullptr);
	if (FAILED(hr)) {
		return E_FAIL;
	}
	return S_OK;
}

HRESULT Hashira::ConstantBuffer::CreateView(D3D12_CONSTANT_BUFFER_VIEW_DESC& cbvDesc, D3D12_CPU_DESCRIPTOR_HANDLE cpuDescriptorHandle)
{

	Framework::GetInstance().GetDevice()->GetDevice()->CreateConstantBufferView(&cbvDesc, cpuDescriptorHandle);
	return S_OK;
}

unsigned long long Hashira::ConstantBuffer::GetInitializeSize()
{
	return _initializeSize;
}
