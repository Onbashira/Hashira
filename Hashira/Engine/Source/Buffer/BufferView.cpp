#include "stdafx.h"
#include "BufferView.h"
#include "Engine/Source/Texture/TextureObject.h"
#include "Engine/Source/Buffer/Buffer.h"
#include "Engine/Source/Rendering/RenderContext/RenderContext.h"

Hashira::BufferView::BufferView()
{
}

Hashira::BufferView::~BufferView()
{
}

HRESULT Hashira::BufferView::Initialize(RenderContext* context, Buffer* buffer, Uint64 stride, Uint64 firstElement)
{
	if (buffer == nullptr) {
		return E_FAIL;
	}

	return E_NOTIMPL;
}

Hashira::DescriptorInfo& Hashira::BufferView::GetDescInfo()
{
	return _descriptorInfo;
}

void Hashira::BufferView::Discard()
{
}

Hashira::ConstantBufferView::ConstantBufferView()
{
}

Hashira::ConstantBufferView::~ConstantBufferView()
{
}

HRESULT Hashira::ConstantBufferView::Initialize(RenderContext* context, Buffer* buffer)
{
	return E_NOTIMPL;
}

Hashira::DescriptorInfo& Hashira::ConstantBufferView::GetDescInfo()
{
	return _descriptorInfo;
}

void Hashira::ConstantBufferView::Discard()
{
}

Hashira::VertexBufferView::VertexBufferView()
{
}

Hashira::VertexBufferView::~VertexBufferView()
{
}

HRESULT Hashira::VertexBufferView::Initialize(RenderContext* context, Buffer* buffer)
{
	return E_NOTIMPL;
}

D3D12_VERTEX_BUFFER_VIEW Hashira::VertexBufferView::GetView()
{
	return D3D12_VERTEX_BUFFER_VIEW();
}

void Hashira::VertexBufferView::Discard()
{
}

Hashira::IndexBufferView::IndexBufferView()
{
}

Hashira::IndexBufferView::~IndexBufferView()
{
}

HRESULT Hashira::IndexBufferView::Initialize(RenderContext* context, Buffer* buffer)
{
	return E_NOTIMPL;
}

D3D12_INDEX_BUFFER_VIEW Hashira::IndexBufferView::GetView()
{
	return D3D12_INDEX_BUFFER_VIEW();
}

void Hashira::IndexBufferView::Discard()
{
}

Hashira::ShaderResourceView::ShaderResourceView()
{
}

Hashira::ShaderResourceView::~ShaderResourceView()
{
}

HRESULT Hashira::ShaderResourceView::Initialize(RenderContext* context, TextureObject* buffer, Uint64 stride, Uint64 firstElement)
{
	return E_NOTIMPL;
}

Hashira::DescriptorInfo& Hashira::ShaderResourceView::GetDescInfo()
{
	return _descriptorInfo;
}

void Hashira::ShaderResourceView::Discard()
{
}

Hashira::UnorderedAccessView::UnorderedAccessView()
{
}

Hashira::UnorderedAccessView::~UnorderedAccessView()
{
}

HRESULT Hashira::UnorderedAccessView::Initialize(RenderContext* context, Buffer* buffer, Uint64 stride, Uint64 firstElement)
{
	return E_NOTIMPL;
}

HRESULT Hashira::UnorderedAccessView::Initialize(RenderContext* context, TextureObject* buffer, Uint64 stride, Uint64 firstElement)
{
	return E_NOTIMPL;
}

Hashira::DescriptorInfo& Hashira::UnorderedAccessView::GetDescInfo()
{
	return _descriptorInfo;
}

void Hashira::UnorderedAccessView::Discard()
{
}

Hashira::RenderTargetView::RenderTargetView()
{
}

Hashira::RenderTargetView::~RenderTargetView()
{
}

HRESULT Hashira::RenderTargetView::Initialize(RenderContext* context, TextureObject* buffer, Uint64 stride, Uint64 firstElement)
{
	return E_NOTIMPL;
}

Hashira::DescriptorInfo& Hashira::RenderTargetView::GetDescInfo()
{
	return _descriptorInfo;
}

void Hashira::RenderTargetView::Discard()
{
}

Hashira::DepthStencilView::DepthStencilView()
{
}

Hashira::DepthStencilView::~DepthStencilView()
{
}

HRESULT Hashira::DepthStencilView::Initialize(RenderContext* context, TextureObject* buffer, Uint64 stride, Uint64 firstElement)
{
	return E_NOTIMPL;
}

Hashira::DescriptorInfo& Hashira::DepthStencilView::GetDescInfo()
{
	return _descriptorInfo;
}

void Hashira::DepthStencilView::Discard()
{
}
