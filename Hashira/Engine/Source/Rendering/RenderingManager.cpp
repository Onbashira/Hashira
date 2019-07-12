#include "RenderingManager.h"
#include "Engine/Source/CoreSystem/Framework.h"
#include "Engine/Source/CommandQueue/CommandQueue.h"
#include "Engine/Source/Factory/Factory.h"
#include "Engine/Source/Window/Window.h"
#include "Engine/Source/Device/RenderingDevice.h"
#include "Engine/Source/Rendering/RenderContext/RenderContext.h"
#include "Engine/Source/Scene/Scene.h"
#include "Engine/Source/DescriptorHeap/DescriptorHeap.h"
#include "Engine/Source/Buffer/Buffer.h"

Hashira::RenderingManager::~RenderingManager()
{
	Term();
}

HRESULT Hashira::RenderingManager::Initialize(std::shared_ptr<D3D12Device>& device, Factory* factory, Window* window, UINT windowWidth, UINT windowHeight, unsigned int bufferNum)
{

	D3D12_COMMAND_QUEUE_DESC desc = {};
	desc.Flags = D3D12_COMMAND_QUEUE_FLAGS::D3D12_COMMAND_QUEUE_FLAG_NONE;
	desc.NodeMask = device->GetDevice()->GetNodeCount();
	desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY::D3D12_COMMAND_QUEUE_PRIORITY_HIGH;
	desc.Type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT;
	_masterQueue = std::make_shared<CommandQueue>();
	
	CHECK_RESULT(this->_masterQueue->Initialize(device,desc));

	_swapChain = std::make_shared<SwapChain>();
	CHECK_RESULT(_swapChain->Initialize(*this->_masterQueue.get(),device, *factory, *window, windowWidth, windowHeight, bufferNum));

	_renderingDevice = std::make_shared<RenderingDevice>();
	CHECK_RESULT(_renderingDevice->Initialize(device,_masterQueue,factory));

	return S_OK;
}

void Hashira::RenderingManager::ClearScreen(std::weak_ptr<CommandList> list)
{
	_swapChain->ClearScreen(list.lock());
}

void Hashira::RenderingManager::SetMainRenderTarget(std::shared_ptr<CommandList>& list, D3D12_CPU_DESCRIPTOR_HANDLE * depthHandle)
{
	this->_swapChain->SetRenderTarget(list, depthHandle);
}

void Hashira::RenderingManager::FlipScreen()
{
	_swapChain->FlipScreen();
}

void Hashira::RenderingManager::CopyToRenderTarget(std::weak_ptr<CommandList> list, Buffer* src)
{
	src->ResourceTransition(list, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST);
	this->_swapChain->CopyToRenderTarget(list.lock(), src);
	src->ResourceTransition(list, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ);

}

void Hashira::RenderingManager::SetStatePresentRT(std::shared_ptr<CommandList> list)
{
	_swapChain->SetStatePresent(list);
}

void Hashira::RenderingManager::Present(unsigned int sysncInterval, unsigned int flags)
{
	_swapChain->Present(sysncInterval, flags);
}

std::vector<std::shared_ptr<Hashira::Buffer>> Hashira::RenderingManager::GetDisplayRenderTargets()
{
	return _swapChain->_rtResource;
}

std::shared_ptr<Hashira::CommandQueue> Hashira::RenderingManager::GetQueue()
{
	return _masterQueue;
}

std::shared_ptr<Hashira::RenderingDevice>& Hashira::RenderingManager::GetRenderingDevice()
{
	return _renderingDevice;
}

std::shared_ptr<Hashira::RenderContext>  Hashira::RenderingManager::CreateRenderContext()
{
	std::shared_ptr<RenderContext> ret(new RenderContext());
	ret->Initialize(_renderingDevice, _swapChain->_bufferNum, 0, _masterQueue,_swapChain);
	return std::move(ret);
}

unsigned int Hashira::RenderingManager::GetBackBufferNum()
{
	return _swapChain->_bufferNum;
}

void Hashira::RenderingManager::Term()
{
	_swapChain->Discard();
	_masterQueue->Discard();
}
