#pragma once

namespace Hashira {

	class Factory;

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

		const D3D_FEATURE_LEVEL&				GetFeatureLevel();

		void									Discard();

	private:

	};

};