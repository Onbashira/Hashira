#include "stdafx.h"
#include "RenderContext.h"
#include "Engine/Source/CommandAllocator/CommandAllocator.h"
#include "Engine/Source/CommandList/CommandList.h"
#include "Engine/Source/CommandQueue/CommandQueue.h"
#include "Engine/Source/Device/D3D12Device.h"
#include "Engine/Source/Buffer/Buffer.h"
#include "Engine/Source/Rendering/SwapChain.h"
#include "Engine/Source/Device/RenderingDevice.h"

Hashira::RenderContext::RenderContext()noexcept :
	_frameNum(0),
	_currentIndex(0),
	_node(0),
	_currentFence(0),
	_isDiscarded(false),
	_defaultViewDescriptorInfo(),
	_defaultSamplerDescriptorInfo()
{

}


Hashira::RenderContext::~RenderContext()
{
	Discard();
}

HRESULT Hashira::RenderContext::Initialize(std::shared_ptr<RenderingDevice>& device, int frameNum, int nodeMask, std::shared_ptr<CommandQueue>& queue, std::shared_ptr<SwapChain>& swapChain)
{
	_parentDevice = device;
	this->_currentIndex = 0;
	this->_currentFence = 0LL;
	this->_node = nodeMask;
	this->_frameNum = frameNum;
	_queueRef = queue;
	_swapChain = swapChain;
	HRESULT hret = {};

	if (_frameNum > 2) {
		_flushFunc = [&](INT64 displayFence , INT64 completeValue , bool waitNow)->bool {
			if ((completeValue < displayFence && _currentFence >= _frameNum)
				|| waitNow && (completeValue < static_cast<INT64>(_currentFence)))
			{
				return true;
			}
			return false;
		};
	}
	else {
		_flushFunc = [&](INT64 displayFence, INT64 completeValue, bool waitNow)-> bool {
			return (completeValue < displayFence && _currentFence >= _frameNum); 
		};
	}

	this->_cmdAllocators.resize(_frameNum);
	this->_cmdLists.resize(_frameNum);
	this->_fences.resize(_frameNum);
	this->_listsVector.resize(_frameNum);
	for (unsigned int i = 0; i < frameNum; ++i) {
		_cmdAllocators[i] = std::make_shared<CommandAllocator>();

		hret = _cmdAllocators[i]->Initialize(device->GetD3D12Device(), D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT);
		if (FAILED(hret))
			return hret;
		std::stringstream ss;
		ss << "RenderContext Allocator" << i;
		_cmdAllocators[i]->SetName(ss.str());
		_cmdLists[i][0u] = std::make_shared<CommandList>();
		_cmdLists[i][1u] = std::make_shared<CommandList>();

		_cmdLists[i][0u]->Initialize(device->GetD3D12Device(), nodeMask,
			D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT, _cmdAllocators[i]);
		if (FAILED(hret))
			return hret;
		//多重レコードの回避のためのClose
		_cmdLists[i][0u]->CloseCommandList();

		_cmdLists[i][1u]->Initialize(device->GetD3D12Device(), nodeMask,
			D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT, _cmdAllocators[i]);
		if (FAILED(hret))
			return hret;
		//多重レコードの回避のためのClose
		_cmdLists[i][1u]->CloseCommandList();


		_fences[i].Initialize(0u, D3D12_FENCE_FLAGS::D3D12_FENCE_FLAG_NONE);
		if (FAILED(hret))
			return hret;

	}

	return hret;

}

HRESULT Hashira::RenderContext::IntializeAllocators(std::shared_ptr<RenderingDevice>& device, Uint32 viewDescMaxNum, Uint32 dsvDescMaxNum, Uint32 rtvDescMaxNum, Uint32 samplerMaxNum)
{

	D3D12_DESCRIPTOR_HEAP_DESC desc{};
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.NodeMask = 1;
	desc.NumDescriptors = viewDescMaxNum;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	auto hr = this->_viewHeapAllocator->Initialize(device->GetD3D12Device(), desc);
	if (FAILED(hr)) {
		return hr;
	}
	this->_defaultViewDescriptorInfo = _viewHeapAllocator->Allocate();

	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.NodeMask = 1;
	desc.NumDescriptors = samplerMaxNum;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
	hr = this->_samplerHeapAllocator->Initialize(device->GetD3D12Device(), desc);
	if (FAILED(hr)) {
		return hr;
	}
	this->_defaultSamplerDescriptorInfo = _samplerHeapAllocator->Allocate();

	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.NodeMask = 1;
	desc.NumDescriptors = dsvDescMaxNum;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	hr = this->_dsvHeapAllocator->Initialize(device->GetD3D12Device(), desc);
	if (FAILED(hr)) {
		return hr;
	}

	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.NodeMask = 1;
	desc.NumDescriptors = rtvDescMaxNum;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	hr = this->_dsvHeapAllocator->Initialize(device->GetD3D12Device(), desc);

	return hr;
}

HRESULT Hashira::RenderContext::CreateCommandList(std::shared_ptr<D3D12Device>& device, D3D12_COMMAND_LIST_TYPE type, std::shared_ptr<CommandList>& commandList)
{

	if (commandList == nullptr) {
		commandList = std::make_shared < CommandList>();
	}
	CHECK_RESULT(commandList->Initialize(device, device->GetDevice()->GetNodeCount(), type, _cmdAllocators[_currentIndex]));
	_listsVector[_currentIndex].push_back(commandList);
	return S_OK;
}

HRESULT Hashira::RenderContext::CreateCommandList(D3D12_COMMAND_LIST_TYPE type, std::shared_ptr<CommandList>& commandList)
{

	if (commandList == nullptr) {
		commandList = std::make_shared < CommandList>();
	}
	CHECK_RESULT(commandList->Initialize(_parentDevice->GetD3D12Device(), _parentDevice->GetD3D12Device()->GetDevice()->GetNodeCount(), type, _cmdAllocators[_currentIndex]));
	_listsVector[_currentIndex].push_back(commandList);
	return S_OK;
}

int Hashira::RenderContext::GetCurrentIndex()
{
	return _currentIndex;
}

int Hashira::RenderContext::Flip()
{
	_currentIndex = (_currentIndex + 1) % _frameNum;
	_swapChain->FlipScreen();
	return _currentIndex;
}

void Hashira::RenderContext::ClearCmdLists()
{
	_listsVector[_currentIndex].clear();
	_listsVector[_currentIndex].resize(0);
}

std::weak_ptr<Hashira::CommandList> Hashira::RenderContext::GetResourceUpdateCmdList(RC_COMMAND_LIST_TYPE listType)
{
	return _cmdLists[_currentIndex][static_cast<unsigned int>(listType)];
}

std::weak_ptr<Hashira::CommandAllocator> Hashira::RenderContext::GetCurrentCmdAllocator()
{
	return _cmdAllocators[_currentIndex];
}

Hashira::Fence & Hashira::RenderContext::GetCurrentFence()
{
	return _fences[_currentIndex];
}

std::weak_ptr<Hashira::CommandQueue> Hashira::RenderContext::GetCommandQueue()
{
	return _queueRef;
}

std::shared_ptr<Hashira::SwapChain> Hashira::RenderContext::GetSwapChain()
{
	return _swapChain;
}

std::shared_ptr<Hashira::RenderingDevice>& Hashira::RenderContext::GetRenderingDevice()
{
	return _parentDevice;
}

std::unique_ptr < Hashira::GlobalDescriptorHeap > & Hashira::RenderContext::GetGlobalDescriptorHeap()
{
	return this->_globalDescriptorHeap;
}

std::unique_ptr<Hashira::DescriptorAllocator>& Hashira::RenderContext::GetViewDescriptorHeap()
{
	return _viewHeapAllocator;
}

std::unique_ptr<Hashira::DescriptorAllocator>& Hashira::RenderContext::GetSamplerDescriptorHeap()
{
	return _samplerHeapAllocator;
}

std::unique_ptr<Hashira::DescriptorAllocator>& Hashira::RenderContext::GetRtvDescriptorHeap()
{
	return _rtvHeapAllocator;
}

std::unique_ptr<Hashira::DescriptorAllocator>& Hashira::RenderContext::GetDsvDescriptorHeap()
{
	return _dsvHeapAllocator;
}

Hashira::DescriptorInfo & Hashira::RenderContext::GetViewDescriptorHeapInfo()
{
	return this->_defaultViewDescriptorInfo;
}

Hashira::DescriptorInfo & Hashira::RenderContext::GetSamplerDescriptorHeapInfo()
{
	return this->_defaultSamplerDescriptorInfo;
}

void Hashira::RenderContext::PushFrontCmdList(std::shared_ptr<CommandList> list)
{
	this->_listsVector[_currentIndex].insert(_listsVector[_currentIndex].begin(),list);
}

void Hashira::RenderContext::PushBackCmdList(std::shared_ptr<CommandList> list)
{
	this->_listsVector[_currentIndex].push_back(list);
}

void Hashira::RenderContext::ExecuteCmdList3DQueue()
{
	GetCurrentCmdAllocator().lock()->ExecutedAllocator();
	this->_queueRef->ExecuteCommandLists(_listsVector[_currentIndex]);
}

void Hashira::RenderContext::ExecuteCmdListCopyQueue()
{
	this->_queueRef->ExecuteCopyCommands(_listsVector[_currentIndex]);
}

void Hashira::RenderContext::ExecuteCmdListComputeQueue()
{
	this->_queueRef->ExecuteComputeCommands(_listsVector[_currentIndex]);
}

void Hashira::RenderContext::WaitForGPU(std::shared_ptr<CommandQueue>& commandQueue, bool waitNow)
{	
	
	_currentFence++;

	//Signal発行
	commandQueue->GetQueue()->Signal(_fences[_currentIndex].GetFence().Get(), _currentFence);

	INT64 displayFence = _currentFence - static_cast<INT64>(_frameNum )+ 1;

	if (waitNow)
	{
		displayFence = _currentFence;
	}

	INT64 completeValue = static_cast<INT64>(_fences[_currentIndex].GetFence()->GetCompletedValue());
	if (_flushFunc(displayFence,completeValue,waitNow))
	{		
		
		SystemLogger::GetInstance().Log(Hashira::LOG_LEVEL::Details, std::string("Wait for GPU...\n"));

		HANDLE eventHandle = CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS);

		_fences[_currentIndex].GetFence()->SetEventOnCompletion(displayFence, eventHandle);
		WaitForSingleObject(eventHandle, INFINITE);
		CloseHandle(eventHandle);
	}
}

void Hashira::RenderContext::ResetAllocators()
{
	std::lock_guard mutexLock(_allocatorMutex);

	for (auto& allocator : _cmdAllocators)
	{
		allocator->ResetAllocator();
	}
}

void Hashira::RenderContext::ResetCurrentCommandAllocator()
{
	std::lock_guard mutexLock(_allocatorMutex);

	_cmdAllocators[_currentIndex]->ResetAllocator();
}

void Hashira::RenderContext::ResetCommandList(std::shared_ptr<CommandList>& list)
{
	list->ResetCommandList(_cmdAllocators[_currentIndex]);
}

void Hashira::RenderContext::Present(unsigned int syncValue, unsigned int flags)
{
	_swapChain->Present(syncValue, flags);
}


void Hashira::RenderContext::Discard()
{
	if (_isDiscarded) {
		return;
	}
	this->ResetAllocators();
	for (int i = 0; i < _frameNum; ++i) {
		this->_cmdAllocators[i]->Discard();
		this->_cmdAllocators[i].reset();
		this->_fences[i].Discard();
		for (int j = 0; j < 2; ++j) {
			this->_cmdLists[i][j]->Discard();
			this->_cmdLists[i][j].reset();
		}
		_listsVector[i].clear();

	}

	_isDiscarded = true;
}
