#include "CommandList.h"
#include "Engine/Source/CommandList/CommandListManager.h"
#include "Engine/Source/Debug/Logger.h"
#include "Engine/Source/Device/RenderingDevice.h"
#include "Engine/Source/Rendering/RenderContext/RenderContext.h"


Hashira::CommandListManager::CommandListManager()
{

}


Hashira::CommandListManager::~CommandListManager()
{
	Discard();
}


HRESULT Hashira::CommandListManager::Create(std::string commandListName, std::shared_ptr<RenderingDevice>& device, std::shared_ptr<RenderContext>& renderContext, unsigned int nodeMask, D3D12_COMMAND_LIST_TYPE listType)
{
	if (_library.find(commandListName) == _library.end()) {
		this->_library[commandListName] = std::make_shared<CommandList>();
		auto hr = this->_library[commandListName]->Initialize(device->GetD3D12Device(),nodeMask, listType,renderContext->GetCurrentCmdAllocator().lock());
		this->_library[commandListName]->SetName(commandListName);
		if (SUCCEEDED(hr)) {
			DEBUG_LOG(std::string( "CommandList‚ª³í‚Éì¬E“o˜^‚³‚ê‚Ü‚µ‚½ : " + commandListName));
			return hr;
		}
		return hr;
	}
	return E_FAIL;
}

void Hashira::CommandListManager::Set(std::string commandListName, std::shared_ptr<CommandList> commandList)
{
	if (_library.find(commandListName) == _library.end()) {
		this->_library[commandListName].reset();
		this->_library[commandListName] = commandList;
		DEBUG_LOG(std::string("CommandList‚ª³í‚Éì¬E“o˜^‚³‚ê‚Ü‚µ‚½ : " + commandListName));

	}
}

std::shared_ptr<Hashira::CommandList> Hashira::CommandListManager::Get(std::string commandListName)
{
	if (_library.find(commandListName) != _library.end()) {
		return  this->_library[commandListName];
	}
	ERROR_LOG(std::string("•s³ŒŸõƒ[ƒh‚Å‚· : " + commandListName));

	return nullptr;
}

void Hashira::CommandListManager::Erase(std::string commandListName)
{
	if (_library.find(commandListName) != _library.end()) {
		this->_library.erase(commandListName);
		DEBUG_LOG(std::string("CommandList‚ª³í‚Éíœ‚³‚ê‚Ü‚µ‚½ : " + commandListName));

	}
}

void Hashira::CommandListManager::Discard()
{
	for (auto& list : _library) {
		list.second->Discard();
		list.second.reset();
	}
	_library.clear();
}

