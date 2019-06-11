#include "RootSignature.h"
#include "Engine/Source/Utility/Utility.h"
#include "Engine/Source/Debug/Logger.h"
#include "Engine/Source/CoreSystem/Framework.h"
#include <string>


Hashira::RootSignature::RootSignature()  :
	_name("UnNamed RootSignature")
{
}


Hashira::RootSignature::~RootSignature()
{
	Discard();
}


HRESULT Hashira::RootSignature::CreateFromShader(ID3DBlob * shader)
{
	Microsoft::WRL::ComPtr<ID3DBlob> signature;

	if (FAILED(D3DGetBlobPart(shader->GetBufferPointer(), shader->GetBufferSize(), D3D_BLOB_ROOT_SIGNATURE, 0, &signature)))
	{
		return E_FAIL;
	}
	auto hr = Framework::GetInstance().GetDevice()->GetDevice()->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&_rootSignature));
	if (FAILED(hr)) {
		return hr;
	};
	signature.Reset();
	return S_OK;
}

HRESULT Hashira::RootSignature::CreateFromDesc(D3D12_ROOT_SIGNATURE_DESC * desc)
{
	Microsoft::WRL::ComPtr<ID3DBlob> signature;
	Microsoft::WRL::ComPtr<ID3DBlob> error;

	auto hr = D3D12SerializeRootSignature(desc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);
	if (FAILED(hr)) {
		OutputDebugStringA((char*)error->GetBufferPointer());
		return E_FAIL;
	}

	Framework::GetInstance().GetDevice()->GetDevice()->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&_rootSignature));
	signature.Reset();
	error.Reset();
	return S_OK;
}

Microsoft::WRL::ComPtr<ID3D12RootSignature>& Hashira::RootSignature::GetSignature()
{
	return this->_rootSignature;
}

void Hashira::RootSignature::SetName(std::string name)
{
	if (_rootSignature.Get() != nullptr) {
		this->_rootSignature->SetName(Util::StringToWString(name).c_str());
		_name = name;
	}
}

void Hashira::RootSignature::Discard()
{
	if (_rootSignature.Get() != nullptr) {
		_rootSignature.Reset();
		DEBUG_LOG(std::string("RootSignature : " + _name + " is  Reset"));
	}
}