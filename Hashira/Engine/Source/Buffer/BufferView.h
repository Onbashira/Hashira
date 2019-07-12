#pragma once
#include "Engine/Source/DescriptorHeap/DescriptorHeap.h"

namespace Hashira {

	class D3D12Device;
	class RenderContext;
	class Buffer;
	class TextureObject;

	class BufferView
	{
	private:

		DescriptorInfo _descriptorInfo;

	public:
		BufferView();
		~BufferView();
		HRESULT Initialize(RenderContext* context, Buffer* buffer, Uint64 stride, Uint64 firstElement);
		DescriptorInfo& GetDescInfo();
		void Discard();
	};

	class ConstantBufferView
	{
	private:
		DescriptorInfo _descriptorInfo;
	public:
		ConstantBufferView();
		~ConstantBufferView();
		HRESULT Initialize(RenderContext* context, Buffer* buffer);
		DescriptorInfo& GetDescInfo();
		void Discard();
	};

	class VertexBufferView
	{
	private:
		D3D12_VERTEX_BUFFER_VIEW _view;
	public:
		VertexBufferView();
		~VertexBufferView();
		HRESULT Initialize(RenderContext* context, Buffer* buffer);
		D3D12_VERTEX_BUFFER_VIEW GetView();
		void Discard();
	};

	class IndexBufferView
	{
	private:
		D3D12_INDEX_BUFFER_VIEW _view;
	public:
		IndexBufferView();
		~IndexBufferView();
		HRESULT Initialize(RenderContext* context, Buffer* buffer);
		D3D12_INDEX_BUFFER_VIEW GetView();
		void Discard();

	};

	class ShaderResourceView
	{
	private:

		DescriptorInfo _descriptorInfo;

	public:
		ShaderResourceView();
		~ShaderResourceView();
		HRESULT Initialize(RenderContext* context, TextureObject* buffer, Uint64 stride, Uint64 firstElement);
		DescriptorInfo& GetDescInfo();
		void Discard();
	};

	class UnorderedAccessView
	{
	private:

		DescriptorInfo _descriptorInfo;

	public:
		UnorderedAccessView();
		~UnorderedAccessView();
		HRESULT Initialize(RenderContext* context, Buffer* buffer, Uint64 stride, Uint64 firstElement);
		HRESULT Initialize(RenderContext* context, TextureObject* buffer, Uint64 stride, Uint64 firstElement);
		DescriptorInfo& GetDescInfo();
		void Discard();
	};

	class RenderTargetView
	{
	private:

		DescriptorInfo _descriptorInfo;
		DXGI_FORMAT _format;
	public:
		RenderTargetView();
		~RenderTargetView();
		HRESULT Initialize(RenderContext* context, TextureObject* buffer, Uint64 stride, Uint64 firstElement);
		DescriptorInfo& GetDescInfo();
		void Discard();
	};

	class DepthStencilView
	{
	private:

		DescriptorInfo _descriptorInfo;
		DXGI_FORMAT _format;

	public:
		DepthStencilView();
		~DepthStencilView();
		HRESULT Initialize(RenderContext* context, TextureObject* buffer, Uint64 stride, Uint64 firstElement);
		DescriptorInfo& GetDescInfo();
		void Discard();
	};
};