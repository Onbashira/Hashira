#include "stdafx.h"
#include "D2DDevice.h"
#include "D3D11on12Device.h"

Hashira::D2DDevice::D2DDevice()
{
}


Hashira::D2DDevice::~D2DDevice()
{
}

HRESULT Hashira::D2DDevice::Initialize(std::shared_ptr<D3D11On12Device>& d3d11On12Device)
{
	Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDevice = {};

	CHECK_RESULT(CreateD2DFactory(d3d11On12Device, dxgiDevice));
	CHECK_RESULT(CreateD2DDevice(d3d11On12Device, dxgiDevice));
	CHECK_RESULT(CreateD2DDeviceContext());
	CHECK_RESULT(CreateDWriteDevice());
	
	return S_OK;

}

Microsoft::WRL::ComPtr<ID2D1Device2>& Hashira::D2DDevice::GetD2DDevice()
{
	return _d2dDevice;

}

Microsoft::WRL::ComPtr<ID2D1Factory3>& Hashira::D2DDevice::GetD2DFactory()
{
	return _d2dFactory;
}

Microsoft::WRL::ComPtr<ID2D1DeviceContext2>& Hashira::D2DDevice::GetD2DDeviceContext()
{
	return _d2dDeviceContext;
}

Microsoft::WRL::ComPtr<IDWriteFactory>& Hashira::D2DDevice::GetDWriteFactory()
{
	return _dwriteFacotry;
}

void Hashira::D2DDevice::Discard()
{
	if (_d2dDevice != nullptr) {
		_d2dDevice.Reset();
	}
	if (_d2dFactory != nullptr) {
		_d2dFactory.Reset();
	}
	if (_d2dDeviceContext != nullptr) {
		_d2dDeviceContext.Reset();
	}
	if (_dwriteFacotry != nullptr) {
		_dwriteFacotry.Reset();
	}
}

HRESULT Hashira::D2DDevice::CreateD2DFactory(std::shared_ptr<D3D11On12Device>& d3d11On12Device, Microsoft::WRL::ComPtr<IDXGIDevice>& dxgiDevice)
{

	HRESULT ret = {};
	D2D1_FACTORY_OPTIONS fopt = {};
	fopt.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;

	ret = d3d11On12Device->GetDevice().As(&dxgiDevice);
	if (FAILED(ret))
	{
		Util::Comment(L"DXGIデバイスの取得に失敗");
		return ret;
	}

	ret = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory3),
		&fopt, &_d2dFactory);
	if (FAILED(ret))
	{
		Util::Comment(L"D2D1ファクトリの生成に失敗");
	}
	return ret;
}

HRESULT Hashira::D2DDevice::CreateD2DDevice(std::shared_ptr<D3D11On12Device>& d3d11On12Device, Microsoft::WRL::ComPtr<IDXGIDevice>& dxgiDevice)
{

	HRESULT ret = {};

	ret = _d2dFactory->CreateDevice(dxgiDevice.Get(), &_d2dDevice);
	if (FAILED(ret))
	{
		Util::Comment(L"D2Dデバイスの生成に失敗");
	}

	return ret;
}



HRESULT Hashira::D2DDevice::CreateD2DDeviceContext()
{
	HRESULT ret = {};
	ret = this->_d2dDevice->CreateDeviceContext(
		D2D1_DEVICE_CONTEXT_OPTIONS::D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
		_d2dDeviceContext.GetAddressOf()
	);
	if (FAILED(ret))
	{
		Util::Comment(L"D2Dデバイスコンテキストの生成に失敗");
	}
	_d2dDeviceContext->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE::D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);
	
	
	return ret;
}

HRESULT Hashira::D2DDevice::CreateDWriteDevice()
{
	HRESULT ret = {};

	ret = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),&_dwriteFacotry);
	if (FAILED(ret))
	{
		Util::Comment(L"DWriteFactoryの生成に失敗");
	}
	return ret;
}
