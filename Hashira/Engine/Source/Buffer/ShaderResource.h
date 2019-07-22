#pragma once
#include"Engine/Source/Utility/D3D12Common.h"
#include "Buffer.h"

namespace Hashira {

	class ShaderResource : public Buffer
	{

	private:

	public:	

		ShaderResource();
		
		virtual ~ShaderResource();

		HRESULT	Initialize(std::shared_ptr<D3D12Device>& device, UINT texHeight, UINT texWidth, UINT texDepth ,DXGI_FORMAT texFormat,D3D12_RESOURCE_FLAGS allowFlags = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE,const Vector4& clearColor = Vector4(0.0f,0.0f,0.0f,1.0f));

		UINT64 GetWidth();
		
		UINT64 GetHeight();

		UINT64 GetDepthOfArarry();

		void Discard();

	private:

	};

}