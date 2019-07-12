#include "GraphicsContextManager.h"
#include "Engine/Source/CommandList/CommandListManager.h"
#include "Engine/Source/CommandList/CommandList.h"
#include "Engine/Source/CommandQueue/CommandQueue.h"
#include "Engine/Source/Async/Fence.h"
#include "Engine/Source/Signature/RootSignature.h"
#include "Engine/Source/PipelineState/PipelineStateObject.h"
#include "Engine/Source/Rendering/RenderContext/RenderContext.h"
#include "Engine/Source/Device/RenderingDevice.h"
#include <memory>


Hashira::GraphicsContextManager::GraphicsContextManager() :
	_commandListLibrary(), _shaderObjectLibrary()
{

}


Hashira::GraphicsContextManager::~GraphicsContextManager()
{
	Discard();
}


void Hashira::GraphicsContextManager::CloseAllCommandLists()
{
	for (auto itr = _commandListLibrary._library.begin(); itr != _commandListLibrary._library.end(); itr++) {
		itr->second->CloseCommandList();
	}
}

void Hashira::GraphicsContextManager::CloseCommandList(std::string commandListName)
{
	_commandListLibrary.Get(commandListName)->CloseCommandList();
}

void Hashira::GraphicsContextManager::ResetCommandList(std::string commandListName,std::shared_ptr<RenderContext>& renderContext)
{
	auto allocator = renderContext->GetCurrentCmdAllocator().lock();
	_commandListLibrary.Get(commandListName)->ResetCommandList(allocator);

}

HRESULT Hashira::GraphicsContextManager::ExecutionCommandListAsync(CommandQueue * queue, std::vector<std::string> commandListNames, Fence * fence)
{

	std::vector<ID3D12CommandList*> lists(commandListNames.size());

	for (int i = 0; i < commandListNames.size(); ++i) {
		lists[i] = GetCommandList(commandListNames[i])->GetCommandList().Get();
	}

	queue->GetQueue()->ExecuteCommandLists(static_cast<unsigned int>(lists.size()), &lists[0]);

	return S_OK;
}

HRESULT Hashira::GraphicsContextManager::ExecutionAllCommandLists(CommandQueue * queue, Fence* fence)
{

	std::vector< ID3D12CommandList *> lists;

	lists.reserve(_commandListLibrary._library.size());
	for (auto itr = _commandListLibrary._library.begin(); itr != _commandListLibrary._library.end(); itr++) {
		lists.push_back(itr->second->GetCommandList().Get());
	}
	queue->GetQueue()->ExecuteCommandLists(static_cast<unsigned int>(lists.size()), &lists[0]);
	if (fence != nullptr) {
		fence->Wait(queue);
	}
	return S_OK;
}

HRESULT Hashira::GraphicsContextManager::CreateCommandList(std::string commandListName, std::shared_ptr<RenderingDevice>& device, std::shared_ptr<RenderContext>& renderContext, unsigned int nodeMask, D3D12_COMMAND_LIST_TYPE listType)
{
	return _commandListLibrary.Create(commandListName,device,renderContext, nodeMask, listType);
}

HRESULT Hashira::GraphicsContextManager::CreatePSO(std::string psoName, D3D12_GRAPHICS_PIPELINE_STATE_DESC& gps, ID3DBlob * rootSignature)
{
	return _shaderObjectLibrary.CreatePSO(psoName, gps, rootSignature);
}

HRESULT Hashira::GraphicsContextManager::CreatePSO(std::shared_ptr<D3D12Device>& device, std::string psoName, D3D12_GRAPHICS_PIPELINE_STATE_DESC & gps, ID3DBlob * rootSignature)
{
	return _shaderObjectLibrary.CreatePSO(device,psoName, gps, rootSignature);
}

HRESULT Hashira::GraphicsContextManager::CreatePSO(std::string psoName, D3D12_COMPUTE_PIPELINE_STATE_DESC& cps, ID3DBlob * rootSignature)
{
	return _shaderObjectLibrary.CreatePSO(psoName, cps, rootSignature);
}

HRESULT Hashira::GraphicsContextManager::CreatePSO(std::shared_ptr<D3D12Device>& device, std::string psoName, D3D12_COMPUTE_PIPELINE_STATE_DESC & cps, ID3DBlob * rootSignature)
{
	return _shaderObjectLibrary.CreatePSO(device,psoName, cps, rootSignature);
}

//HRESULT Hashira::GraphicsContextManager::CreateCommandQueue(std::string queueName, D3D12_COMMAND_QUEUE_DESC& desc)
//{
//	return this->_queueManager.Create(queueName,desc);
//}

HRESULT Hashira::GraphicsContextManager::CreateRootSignature(std::string rsName, ID3DBlob * signature)
{
	return _shaderObjectLibrary.CreateRootSignature(rsName, signature);
}

HRESULT Hashira::GraphicsContextManager::CreateRootSignature(std::string rsName, D3D12_ROOT_SIGNATURE_DESC * signature)
{
	return _shaderObjectLibrary.CreateRootSignature(rsName, signature);
}

void Hashira::GraphicsContextManager::SetCommandList(std::string commandListName, std::shared_ptr<CommandList> commandList)
{
	_commandListLibrary.Set(commandListName, commandList);
}

void Hashira::GraphicsContextManager::SetPSO(std::string psoName, std::shared_ptr<PipelineStateObject> pso)
{
	_shaderObjectLibrary.SetPSO(psoName, pso);
}

void Hashira::GraphicsContextManager::SetRootSignature(std::string rsName, std::shared_ptr<RootSignature> rootSignature)
{
	_shaderObjectLibrary.SetRootSignature(rsName, rootSignature);
}

std::shared_ptr<Hashira::CommandList> Hashira::GraphicsContextManager::GetCommandList(std::string commandListName)
{
	return _commandListLibrary.Get(commandListName);
}

std::shared_ptr<Hashira::PipelineStateObject> Hashira::GraphicsContextManager::GetPSO(std::string psoName)
{
	return _shaderObjectLibrary.GetPSO(psoName).lock();
}

//std::shared_ptr<Hashira::CommandQueue> Hashira::GraphicsContextManager::GetCommandQueue(std::string queueName)
//{
//	return std::shared_ptr<CommandQueue>();
//}

std::shared_ptr<Hashira::RootSignature> Hashira::GraphicsContextManager::GetRootSignature(std::string rsName)
{
	return _shaderObjectLibrary.GetRootSignature(rsName).lock();
}

void Hashira::GraphicsContextManager::EraseCommandList(std::string commandListName)
{
	_commandListLibrary.Erase(commandListName);
}

void Hashira::GraphicsContextManager::ErasePSO(std::string psoName)
{
	_shaderObjectLibrary.ErasePSO(psoName);
}

void Hashira::GraphicsContextManager::EraseRootSignature(std::string rsName)
{
	_shaderObjectLibrary.EraseRootSignature(rsName);
}

//void Hashira::GraphicsContextManager::EraseCommandQueue(std::string queueName)
//{
//	_queueManager.Erase(queueName);
//}

void Hashira::GraphicsContextManager::Discard()
{
	_shaderObjectLibrary.Discard();
	_commandListLibrary.Discard();
}

Hashira::GraphicsContextManager & Hashira::GraphicsContextManager::GetInstance()
{
	static Hashira::GraphicsContextManager instance;
	return instance;
}
