#pragma once

namespace Hashira {

	class Factory;
	struct DescriptorInfo;
	class Buffer;
	class D3D12Device
	{

	public:

	private:

		Microsoft::WRL::ComPtr<ID3D12Device3>	_device;

		D3D_FEATURE_LEVEL						_featureLevel;

		DXGI_ADAPTER_DESC1						_adapterDesc;

		D3D12_FEATURE_DATA_ARCHITECTURE1		_architectureData;

		bool									_useWarpDevice;

	public:	

		D3D12Device();

		~D3D12Device();

		HRESULT									Initialize(Factory* factory, bool useWarpDevice = false);

		Microsoft::WRL::ComPtr<ID3D12Device3>&	GetDevice();

		void CreateConstantBufferView(DescriptorInfo* allocatedDescriptor, D3D12_CONSTANT_BUFFER_VIEW_DESC* desc);
		void CreateShaderResourceView(Buffer* srcBuffer,DescriptorInfo* allocatedDescriptor, D3D12_SHADER_RESOURCE_VIEW_DESC* desc);
		void CreateUnorderedAccessView(Buffer* srcBuffer, DescriptorInfo* allocatedDescriptor, D3D12_UNORDERED_ACCESS_VIEW_DESC* desc, Buffer* counterBuffer = nullptr);
		void CreateDepthStencilView(Buffer* srcBuffer, DescriptorInfo* allocatedDescriptor, D3D12_DEPTH_STENCIL_VIEW_DESC* desc);
		void CreateRenderTargetView(Buffer* srcBuffer, DescriptorInfo* allocatedDescriptor, D3D12_RENDER_TARGET_VIEW_DESC* desc);
	
		const D3D_FEATURE_LEVEL&				GetFeatureLevel();

		void									Discard();

	private:

	};

};