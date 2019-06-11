#include "stdafx.h"
#include "CommandAllocator.h"
#include "Engine/Source/CoreSystem/Framework.h"

Hashira::CommandAllocator::CommandAllocator() :
	_name("UnNamed_CmdAllocator"),
	_isDuringExecuting(false)
{
}


Hashira::CommandAllocator::~CommandAllocator()
{
	Discard();
}

HRESULT Hashira::CommandAllocator::Initialize(D3D12_COMMAND_LIST_TYPE listType)
{
	
	auto result = Hashira::Framework::GetInstance().GetDevice()->GetDevice()->CreateCommandAllocator(listType, IID_PPV_ARGS(&_allocator));
	if (result != S_OK) {
		return E_FAIL;
	}
	return result;
}

HRESULT Hashira::CommandAllocator::Initialize(std::shared_ptr<D3D12Device>&device, D3D12_COMMAND_LIST_TYPE listType)
{
	auto result = device->GetDevice()->CreateCommandAllocator(listType, IID_PPV_ARGS(&_allocator));
	if (result != S_OK) {
		return E_FAIL;
	}
	return result;
}

Microsoft::WRL::ComPtr<ID3D12CommandAllocator>& Hashira::CommandAllocator::GetAllocator()
{
	return this->_allocator;
}

void Hashira::CommandAllocator::SetName(String name)
{
	_name = name;

	this->_allocator->SetName(Util::StringToWString(name).c_str());
}

Hashira::String & Hashira::CommandAllocator::GetName()
{
	return _name;
}

void Hashira::CommandAllocator::ResetAllocator()
{
	_isDuringExecuting = false;
	SystemLogger::GetInstance().Log(Hashira::LOG_LEVEL::Details, std::string("ResetAllocator\n"));
	this->_allocator->Reset();
}

void Hashira::CommandAllocator::Discard()
{
	if (this->_allocator != nullptr)
	{
		this->_allocator.Reset();
	}
}
