#include "stdafx.h"
#include "D3D11On12Device.h"
#include "Engine/Source/Device/D3D12Device.h"
#include "Engine/Source/CommandQueue/CommandQueue.h"



Hashira::D3D11On12Device::D3D11On12Device()
{
}


Hashira::D3D11On12Device::~D3D11On12Device()
{
	Discard();
}

HRESULT Hashira::D3D11On12Device::Initialize(std::shared_ptr<D3D12Device>& d3d12Device, std::shared_ptr<CommandQueue>& queue)
{
	HRESULT ret = S_OK;

	UINT flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
	flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	Microsoft::WRL::ComPtr<ID3D11Device> d3d11Device;
	ID3D12CommandQueue* queues[] = { queue->GetQueue().Get()};
	ret = D3D11On12CreateDevice(d3d12Device->GetDevice().Get(), flags, nullptr, 0,
		reinterpret_cast<IUnknown**>(queues), _countof(queues), 0,&d3d11Device,
		&_d3d11DeviceContext,nullptr);
	if (FAILED(ret))
	{
		return ret;
	}

	ret = d3d11Device.As(&_d3d11on12Device);

	if (FAILED(ret)) 
	{
		return ret;
	}

//#ifdef _DEBUG
//
//	Microsoft::WRL::ComPtr<ID3D12InfoQueue> infoQueue;
//	ret = d3d12Device->GetDevice()->QueryInterface(IID_PPV_ARGS(&infoQueue));
//	if (SUCCEEDED(ret)) {
//		D3D12_MESSAGE_SEVERITY severities[] =
//		{
//			D3D12_MESSAGE_SEVERITY::D3D12_MESSAGE_SEVERITY_INFO
//		};
//
//		D3D12_MESSAGE_ID denyIds[] =
//		{
//			D3D12_MESSAGE_ID_INVALID_DESCRIPTOR_HANDLE,
//		};
//
//		D3D12_INFO_QUEUE_FILTER filter = {};
//		filter.DenyList.NumSeverities = _countof(severities);
//		filter.DenyList.pSeverityList = severities;
//		filter.DenyList.NumIDs = _countof(denyIds);
//		filter.DenyList.pIDList = denyIds;
//
//		ret = infoQueue->PushStorageFilter(&filter);
//
//		if (FAILED(ret)) 
//		{
//			return ret;
//		}
//	}
//
//#endif // _DEBUG

	return ret;

 }

Microsoft::WRL::ComPtr<ID3D11On12Device>& Hashira::D3D11On12Device::GetDevice()
{
	return this->_d3d11on12Device;
}

Microsoft::WRL::ComPtr<ID3D11DeviceContext> Hashira::D3D11On12Device::Get11DeviceContext()
{
	return _d3d11DeviceContext;
}

void Hashira::D3D11On12Device::Discard()
{

	if (_d3d11DeviceContext.Get() != nullptr) {
		this->_d3d11DeviceContext.Reset();
	}
	if (_d3d11on12Device.Get() != nullptr) {
		this->_d3d11on12Device.Reset();
	}
}
