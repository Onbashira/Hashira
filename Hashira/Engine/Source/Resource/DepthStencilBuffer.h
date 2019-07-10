#pragma once
#include <memory>
#include"Engine/Source/Utility/D3D12Common.h"
#include"Engine/Source/DescriptorHeap/DescriptorHeap.h"
#include "ShaderResource.h"

namespace Hashira {

	class CommandList;
	class D3D12Device;

	class DepthStencil : public Resource
	{
	private:

		float _clearColor[4];

	public:

		DepthStencil();

		~DepthStencil();

		HRESULT Initialize(std::shared_ptr<D3D12Device>& device, UINT width, UINT height, DXGI_FORMAT resourceFormat, DXGI_FORMAT clearValueFormat);

		void Discard();

	private:


	};

}