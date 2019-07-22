#include "ShaderResource.h"
#include "Engine/Source/CoreSystem/Framework.h"



Hashira::ShaderResource::ShaderResource()
{
}


Hashira::ShaderResource::~ShaderResource()
{
}

HRESULT Hashira::ShaderResource::Initialize(std::shared_ptr<D3D12Device>& device,UINT texHeight, UINT texWidth, UINT texDepth, DXGI_FORMAT texFormat, D3D12_RESOURCE_FLAGS allowFlags, const Vector4& clearColor)
{

	D3D12_HEAP_PROPERTIES props = {};
	props.Type = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT;
	props.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	props.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
	props.CreationNodeMask = 0;
	props.VisibleNodeMask = 0;

	D3D12_RESOURCE_DESC resDesc = {};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	resDesc.Width = texWidth;
	resDesc.Height = texWidth;
	resDesc.DepthOrArraySize = texDepth;
	resDesc.MipLevels = 1;
	resDesc.Format = texFormat;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_UNKNOWN;
	resDesc.SampleDesc.Count = 1;
	resDesc.SampleDesc.Quality = 0;
	resDesc.Flags = allowFlags;
	if (allowFlags & D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET) {
		allowFlags |= D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
		
	}
	D3D12_CLEAR_VALUE clearValue;
	clearValue.Format = texFormat;
	clearValue.DepthStencil.Depth = 0.0f;
	clearValue.DepthStencil.Stencil = 0;
	clearValue.Color[0] = clearColor.x;
	clearValue.Color[1] = clearColor.y;
	clearValue.Color[2] = clearColor.z;
	clearValue.Color[3] = clearColor.w;

	auto hr = Buffer::Initialize(device , props, D3D12_HEAP_FLAG_NONE, resDesc,
		(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE | D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE), &clearValue);

	return hr;
}

UINT64 Hashira::ShaderResource::GetWidth()
{
	return GetResource()->GetDesc().Width;
}

UINT64 Hashira::ShaderResource::GetHeight()
{
	return GetResource()->GetDesc().Height;
}

UINT64 Hashira::ShaderResource::GetDepthOfArarry()
{
	return GetResource()->GetDesc().DepthOrArraySize;
}

void Hashira::ShaderResource::Discard()
{
	Buffer::Discard();
}


