#include "RenderingDevice.h"
#include "Engine/Source/Factory/Factory.h"
#include "Engine/Source/Device/D2DDevice.h"
#include "Engine/Source/Device/D3D11On12Device.h"
#include "Engine/Source/Device/D3D12Device.h"
#include "Engine/Source/Factory/Factory.h"
#include "Engine/Source/Rendering/RenderContext/RenderContext.h"



Hashira::RenderingDevice::RenderingDevice()
{

}


Hashira::RenderingDevice::~RenderingDevice()
{
	Discard();
}

HRESULT Hashira::RenderingDevice::Initialize(std::shared_ptr<D3D12Device> d3d12Device,std::shared_ptr<CommandQueue>& queue, Factory * factory)
{

	HRESULT ret = {};

	this->_d3d12Device = d3d12Device;


	this->_d3d11On12Devcie = std::make_shared<D3D11On12Device>();
	//ret = this->_d3d11On12Devcie->Initialize(_d3d12Device, queue);
	//if (FAILED(ret)) {
	//	Util::Comment(L"D3D11On12デバイスの作成失敗");
	//	return ret;
	//}

	this->_d2dDevice = std::make_shared<D2DDevice>();
	//ret = this->_d2dDevice->Initialize(_d3d11On12Devcie);
	//if (FAILED(ret)) {
	//	Util::Comment(L"D3D12デバイスの作成失敗");
	//	return ret;
	//}

	return ret;
}

std::shared_ptr<Hashira::D3D12Device > Hashira::RenderingDevice::GetD3D12Device()
{
	return _d3d12Device;
}

std::shared_ptr<Hashira::D3D11On12Device> Hashira::RenderingDevice::GetD3D11On12Device()
{
	return _d3d11On12Devcie;
}

std::shared_ptr<Hashira::D2DDevice> Hashira::RenderingDevice::GetD2DDevice()
{
	return _d2dDevice;
}

void Hashira::RenderingDevice::Discard()
{
	_d2dDevice.reset();
	_d3d11On12Devcie->Discard();
	_d3d12Device->Discard();
}
