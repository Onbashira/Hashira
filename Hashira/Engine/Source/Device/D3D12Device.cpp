#include "D3D12Device.h"
#include "Engine/Source/Utility/D3D12Common.h"
#include "Engine/Source/Factory/Factory.h"
#include "Engine/Source/DescriptorHeap/DescriptorHeap.h"
#include "Engine/Source/Buffer/Buffer.h"
#include <string>


//対応フィーチャーレベル
const D3D_FEATURE_LEVEL FeatureLevels[] =
{
	D3D_FEATURE_LEVEL_12_1,
	D3D_FEATURE_LEVEL_12_0,
	D3D_FEATURE_LEVEL_11_1,
	D3D_FEATURE_LEVEL_11_0
};
const std::wstring GPU_CARD_SPEC_LIST[] = {
	L"NVIDIA GeForce GTX 1080",
	L"NVIDIA GeForce GTX 1060",
	L"INTEL HD Graphics"

};


Hashira::D3D12Device::D3D12Device() :
	_device(), _featureLevel(), _useWarpDevice(false)
{
}


Hashira::D3D12Device::~D3D12Device()
{
	Discard();
}


HRESULT Hashira::D3D12Device::Initialize(Factory* factory, bool useWarpDevice)
{
	HRESULT result = {};
	_useWarpDevice = useWarpDevice;
	if (_useWarpDevice) {
		Microsoft::WRL::ComPtr<IDXGIAdapter>	warpAdapter;

		if (FAILED(factory->GetFactory()->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter)))) {
			return E_FAIL;
		}
		for (auto i : FeatureLevels) {
			if (SUCCEEDED(D3D12CreateDevice(warpAdapter.Get(), i, IID_PPV_ARGS(&_device)))) {
				_featureLevel = i;
				break;
			}
		}

	}
	else {
		Microsoft::WRL::ComPtr<IDXGIAdapter1>	hardwareAdapter;
		Microsoft::WRL::ComPtr<IDXGIAdapter1>	adapter;
		hardwareAdapter = nullptr;

		for (UINT i = 0; DXGI_ERROR_NOT_FOUND != factory->GetFactory()->EnumAdapters1(i, &adapter); i++) {
			adapter->GetDesc1(&_adapterDesc);
			if (_adapterDesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
				continue;

			for (auto level : FeatureLevels) {
				if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), level, _uuidof(ID3D12Device3), nullptr))) {
					_featureLevel = level;
					hardwareAdapter = adapter;
					for (auto& spec : GPU_CARD_SPEC_LIST) {
						if (spec == std::wstring(&_adapterDesc.Description[0])) {
							result = D3D12CreateDevice(hardwareAdapter.Get(), _featureLevel, IID_PPV_ARGS(&_device));
							if (FAILED(result)) {
								return result;
							};
							return result;
						}
					}
				}
			}

		}
		adapter.Reset();
	}
	

	return result;
}

Microsoft::WRL::ComPtr<ID3D12Device3>& Hashira::D3D12Device::GetDevice()
{
	return _device;
}

void Hashira::D3D12Device::CreateConstantBufferView(DescriptorInfo* allocatedDescriptor, D3D12_CONSTANT_BUFFER_VIEW_DESC* desc)
{
	this->_device->CreateConstantBufferView(desc, allocatedDescriptor->cpuHandle);
}

void Hashira::D3D12Device::CreateShaderResourceView(Buffer* srcBuffer, DescriptorInfo* allocatedDescriptor, D3D12_SHADER_RESOURCE_VIEW_DESC* desc)
{
	this->_device->CreateShaderResourceView(srcBuffer->GetResource().Get(),desc, allocatedDescriptor->cpuHandle);

}

void Hashira::D3D12Device::CreateUnorderedAccessView(Buffer* srcBuffer, DescriptorInfo* allocatedDescriptor, D3D12_UNORDERED_ACCESS_VIEW_DESC* desc,Buffer* counterBuffer)
{
	this->_device->CreateUnorderedAccessView(srcBuffer->GetResource().Get(), counterBuffer->GetResource().Get(),desc, allocatedDescriptor->cpuHandle);

}

void Hashira::D3D12Device::CreateDepthStencilView(Buffer* srcBuffer, DescriptorInfo* allocatedDescriptor, D3D12_DEPTH_STENCIL_VIEW_DESC* desc)
{
	this->_device->CreateDepthStencilView(srcBuffer->GetResource().Get(),  desc, allocatedDescriptor->cpuHandle);

}

void Hashira::D3D12Device::CreateRenderTargetView(Buffer* srcBuffer, DescriptorInfo* allocatedDescriptor, D3D12_RENDER_TARGET_VIEW_DESC* desc)
{
	this->_device->CreateRenderTargetView(srcBuffer->GetResource().Get(), desc, allocatedDescriptor->cpuHandle);

}


const D3D_FEATURE_LEVEL& Hashira::D3D12Device::GetFeatureLevel()
{
	return this->_featureLevel;
}

void Hashira::D3D12Device::Discard()
{
	_device.Reset();
}

