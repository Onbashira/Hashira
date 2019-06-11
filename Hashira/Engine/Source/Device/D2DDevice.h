#pragma once

namespace Hashira {

	class D3D11On12Device;


	class D2DDevice
	{

	private:
		Microsoft::WRL::ComPtr<ID2D1Device2> _d2dDevice;
		Microsoft::WRL::ComPtr<ID2D1Factory3> _d2dFactory;
		Microsoft::WRL::ComPtr<ID2D1DeviceContext2> _d2dDeviceContext;
		Microsoft::WRL::ComPtr<IDWriteFactory> _dwriteFacotry;

	public:
		
		D2DDevice();
		
		~D2DDevice();

		HRESULT Initialize(std::shared_ptr< D3D11On12Device>& d3d11On12Device);

		Microsoft::WRL::ComPtr<ID2D1Device2>& GetD2DDevice();

		Microsoft::WRL::ComPtr<ID2D1Factory3>& GetD2DFactory();

		Microsoft::WRL::ComPtr<ID2D1DeviceContext2>& GetD2DDeviceContext();

		Microsoft::WRL::ComPtr<IDWriteFactory>& GetDWriteFactory();

		void Discard();

	private:

		HRESULT CreateD2DFactory(std::shared_ptr<D3D11On12Device>& d3d11On12Device, Microsoft::WRL::ComPtr<IDXGIDevice>& dxgiDevice);

		HRESULT CreateD2DDevice(std::shared_ptr<D3D11On12Device>& d3d11On12Device, Microsoft::WRL::ComPtr<IDXGIDevice>& dxgiDevice);

		HRESULT CreateD2DDeviceContext();

		HRESULT CreateDWriteDevice();

	};
}
