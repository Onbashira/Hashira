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


HRESULT Hashira::DepthStencil::Initialize(UINT width, UINT height, DXGI_FORMAT resourceFormat, DXGI_FORMAT clearValueFormat)
{

	D3D12_DESCRIPTOR_HEAP_DESC	heapDesc = {};
	//DSV(深度ステンシルビュー)用のディスクリプタヒープの設定
	heapDesc.NumDescriptors = 1;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	heapDesc.NodeMask = 0;

	_dsvHeap.Initialize(&heapDesc);
	_dsvHeap.SetName("DepthStencilDSVHeap");

	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

	_readOnlySRVHeap.Initialize(&heapDesc);
	_readOnlySRVHeap.SetName("DepthStencilReadOnlyTextureHeap");


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

	//深度ステンシルビューの設定
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = clearValueFormat;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Flags = D3D12_DSV_FLAGS::D3D12_DSV_FLAG_NONE;

	Framework::GetInstance().GetDevice()->GetDevice()->CreateDepthStencilView(GetResource().Get(), &dsvDesc, _dsvHeap.GetCPUHandle(0));


	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};

	if (resourceFormat == DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT) {
		srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	}
	else if (resourceFormat == DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT_S8X24_UINT) {
		srvDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;
	}
	else if (resourceFormat == DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT) {
		srvDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	}
	else {
		srvDesc.Format = resourceFormat;
	}
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.PlaneSlice = 0;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.0F;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	auto hr = this->CreateView(srvDesc, _readOnlySRVHeap.GetCPUHandle(0));
	return S_OK;
}

HRESULT Hashira::DepthStencil::CreateView(D3D12_SHADER_RESOURCE_VIEW_DESC srv, D3D12_CPU_DESCRIPTOR_HANDLE handle)
{

	Framework::GetInstance().GetDevice()->GetDevice()->CreateShaderResourceView(this->GetResource().Get(), &srv, handle);
	return S_OK;
}

void Hashira::DepthStencil::Discard()
{
	_readOnlySRVHeap.Discard();
	_dsvHeap.Discard();
	Resource::Discard();

}

void Hashira::DepthStencil::ClearDepthStencil(std::shared_ptr<Hashira::CommandList> list)
{
	list->GetCommandList()->ClearDepthStencilView(this->_dsvHeap->GetCPUDescriptorHandleForHeapStart(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

}

