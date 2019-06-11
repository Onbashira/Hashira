#pragma once
#include <memory>
#include"Engine/Source/Utility/D3D12Common.h"
#include"Engine/Source/DescriptorHeap/DescriptorHeap.h"
#include "ShaderResource.h"

namespace Hashira {

	class CommandList;

	class DepthStencil : public Resource
	{
	private:

		float _clearColor[4];

		DescriptorHeap _dsvHeap;

		DescriptorHeap _readOnlySRVHeap;

	public:

		DepthStencil();

		~DepthStencil();

		HRESULT Initialize(UINT width, UINT height, DXGI_FORMAT resourceFormat, DXGI_FORMAT clearValueFormat);

		HRESULT CreateView(D3D12_SHADER_RESOURCE_VIEW_DESC srv, D3D12_CPU_DESCRIPTOR_HANDLE handle);

		void Discard();

		DescriptorHeap& GetDSVHeapPtr() { return _dsvHeap; };

		DescriptorHeap& GetReadOnlySRVHeap() { return _readOnlySRVHeap; };

		void ClearDepthStencil(std::shared_ptr<Hashira::CommandList> list);

	private:


	};

}