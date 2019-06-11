#include <string>
#include "CommandSignature.h"
#include "Engine/Source/Utility/D3D12Common.h"
#include "Engine/Source/Utility/Utility.h"
#include "Engine/Source/Debug/Logger.h"
#include "Engine/Source/CoreSystem/Framework.h"
#include "Engine/Source/Signature/RootSignature.h"

Hashira::CommandSignature::CommandSignature()  :
	_name("UnNamedCommandSignature")
{
}


Hashira::CommandSignature::~CommandSignature()
{
	Discard();
}


HRESULT Hashira::CommandSignature::Initialize(const D3D12_COMMAND_SIGNATURE_DESC & desc, std::weak_ptr<Hashira::RootSignature> rs)
{
	auto res = Framework::GetInstance().GetDevice()->GetDevice()->CreateCommandSignature(&desc, rs.lock()->GetSignature().Get(), IID_PPV_ARGS(&_commandSignature));
	return E_NOTIMPL;
}

HRESULT Hashira::CommandSignature::Initialize(std::shared_ptr<D3D12Device>& device, const D3D12_COMMAND_SIGNATURE_DESC & desc, std::weak_ptr<Hashira::RootSignature> rs)
{
	auto res = device->GetDevice()->CreateCommandSignature(&desc, rs.lock()->GetSignature().Get(), IID_PPV_ARGS(&_commandSignature));
	return E_NOTIMPL;
}

HRESULT Hashira::CommandSignature::Initialize(const D3D12_COMMAND_SIGNATURE_DESC & desc)
{
	auto res = Framework::GetInstance().GetDevice()->GetDevice()->CreateCommandSignature(&desc, nullptr, IID_PPV_ARGS(&_commandSignature));
	return E_NOTIMPL;
}

HRESULT Hashira::CommandSignature::Initialize(std::shared_ptr<D3D12Device>& device, const D3D12_COMMAND_SIGNATURE_DESC & desc)
{
	auto res = device->GetDevice()->CreateCommandSignature(&desc, nullptr, IID_PPV_ARGS(&_commandSignature));
	return E_NOTIMPL;
}

Microsoft::WRL::ComPtr<ID3D12CommandSignature>& Hashira::CommandSignature::GetSignature()
{
	return this->_commandSignature;
}

void Hashira::CommandSignature::SetName(std::string name)
{
	if (_commandSignature.Get() != nullptr) {
		this->_commandSignature->SetName(Util::StringToWString(name).c_str());
		_name = name;
	}
}

void Hashira::CommandSignature::Discard()
{
	if (_commandSignature.Get() != nullptr) {
		_commandSignature.Reset();
		DEBUG_LOG(std::string("CommandSignature : " + _name + " is  Reset"));
	}
}


