#include <memory>
#include "Engine/Source/CommandList/CommandList.h"
#include "DepthStencilBuffer.h"
#include "Resource.h"
#include "Engine/Source/CoreSystem/Framework.h"


Hashira::DepthStencil::DepthStencil()
{

}

Hashira::DepthStencil::~DepthStencil()
{
	this->Discard();
}

HRESULT Hashira::DepthStencil::Initialize(std::shared_ptr<D3D12Device>& device, UINT width, UINT height, DXGI_FORMAT resourceFormat, DXGI_FORMAT clearValueFormat)
{
	//ヒーププロパティの設定
	D3D12_HEAP_PROPERTIES heapProp;
	heapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
	heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProp.CreationNodeMask = 1;
	heapProp.VisibleNodeMask = 1;

	// リソースの設定.
	D3D12_RESOURCE_DESC resDesc;
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	resDesc.Alignment = 0;
	resDesc.Width = width;
	resDesc.Height = height;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.Format = resourceFormat;
	resDesc.SampleDesc.Count = 1;
	resDesc.SampleDesc.Quality = 0;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	resDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	// クリア値の設定.
	D3D12_CLEAR_VALUE clearValue;
	clearValue.Format = clearValueFormat;
	clearValue.DepthStencil.Depth = 1.0f;
	clearValue.DepthStencil.Stencil = 0;

	CHECK_RESULT(Resource::Initialize(heapProp, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, resDesc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_DEPTH_WRITE, &clearValue));

	return S_OK;
}

void Hashira::DepthStencil::Discard()
{
	Resource::Discard();
}