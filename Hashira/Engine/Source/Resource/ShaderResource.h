#pragma once
#include"Engine/Source/Utility/D3D12Common.h"
#include "Resource.h"

namespace Hashira {

	class ShaderResource : public Resource
	{

	private:

	public:	

		ShaderResource();
		
		virtual ~ShaderResource();

		HRESULT	Initialize(UINT texHeight, UINT texWidth, UINT texDepth ,DXGI_FORMAT texFormat,D3D12_RESOURCE_FLAGS allowFlags = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE,const Vector4& clearColor = Vector4(0.0f,0.0f,0.0f,1.0f));

		void CreateView(D3D12_SHADER_RESOURCE_VIEW_DESC& srv, D3D12_CPU_DESCRIPTOR_HANDLE cpuDescriptorHandle);

		void CreateView(D3D12_RENDER_TARGET_VIEW_DESC& rtv, D3D12_CPU_DESCRIPTOR_HANDLE cpuDescriptorHandle);

		void CreateView(D3D12_UNORDERED_ACCESS_VIEW_DESC& uav, D3D12_CPU_DESCRIPTOR_HANDLE cpuDescriptorHandle, ID3D12Resource* counterResource = nullptr);

		UINT64 GetWidth();
		
		UINT64 GetHeight();

		UINT64 GetDepthOfArarry();

		void Discard();

	private:

	};

}