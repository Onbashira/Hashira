#pragma once
namespace Hashira {
	class Factory;
	class D3D12Device;
	class D3D11On12Device;
	class D2DDevice;
	class CommandQueue;

	class RenderingDevice
	{
	public:

	private:

		std::shared_ptr<D3D12Device> _d3d12Device;

		std::shared_ptr<D3D11On12Device> _d3d11On12Devcie;

		std::shared_ptr<D2DDevice> _d2dDevice;
	public:
		
		RenderingDevice();
		
		~RenderingDevice();

		HRESULT Initialize(std::shared_ptr<D3D12Device> d3d12Device,std::shared_ptr<CommandQueue>& queue,Factory* factory);

		std::shared_ptr<D3D12Device> GetD3D12Device();

		std::shared_ptr<D3D11On12Device> GetD3D11On12Device();

		std::shared_ptr<D2DDevice> GetD2DDevice();

		void Discard();

	private:

	};

}