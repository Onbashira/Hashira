#include "ShaderObjectManager.h"
#include "Engine/Source/Debug/Logger.h"
#include "Engine/Source/Signature/CommandSignature.h"
#include "Engine/Source/Signature/RootSignature.h"
#include "Engine/Source/PipelineState/PipelineStateObject.h"




Hashira::ShaderObjectManager::ShaderObjectManager()
{
}


Hashira::ShaderObjectManager::~ShaderObjectManager()
{
	Discard();
}


HRESULT Hashira::ShaderObjectManager::CreatePSO(std::string psoName, D3D12_GRAPHICS_PIPELINE_STATE_DESC& gps, ID3DBlob * signature)
{
	if (_psolibrary.find(psoName) == _psolibrary.end()) {
		HRESULT hr = {};
		this->_psolibrary[psoName] = std::make_shared<Hashira::PipelineStateObject>();
		if (signature != nullptr) {
			hr = CreateRootSignature(psoName, signature);
			hr = this->_psolibrary[psoName]->Initialize(psoName, gps, GetRootSignature(psoName ));

		}
		else {
			hr = this->_psolibrary[psoName]->Initialize(psoName, gps, nullptr);
		}
		if (SUCCEEDED(hr)) {

			DEBUG_LOG(std::string("PSO‚ª³í‚Éì¬E“o˜^‚³‚ê‚Ü‚µ‚½ : " + psoName ));

			return hr;
		}
		return hr;
	}
	return E_FAIL;
}

HRESULT Hashira::ShaderObjectManager::CreatePSO(std::shared_ptr<D3D12Device>& device, std::string psoName, D3D12_GRAPHICS_PIPELINE_STATE_DESC & gps, ID3DBlob * signature)
{
	return E_NOTIMPL;
}

HRESULT Hashira::ShaderObjectManager::CreatePSO(std::string psoName, D3D12_COMPUTE_PIPELINE_STATE_DESC& cps, ID3DBlob * signature)
{
	if (_psolibrary.find(psoName) == _psolibrary.end()) {
		this->_psolibrary[psoName] = std::make_shared<Hashira::PipelineStateObject>();
		HRESULT hr = {};
		if (signature != nullptr) {
			hr = CreateRootSignature(psoName + "RootSignature", signature);
			hr = this->_psolibrary[psoName]->Initialize(psoName, cps, GetRootSignature(psoName + "RootSignature"));

		}
		else {
			hr = this->_psolibrary[psoName]->Initialize(psoName, cps, nullptr);
		}
		if (SUCCEEDED(hr)) {
			DEBUG_LOG(std::string("PSO‚ª³í‚Éì¬E“o˜^‚³‚ê‚Ü‚µ‚½ : " + psoName));
			return hr;
		}
		return hr;
	}
	return E_FAIL;
}

HRESULT Hashira::ShaderObjectManager::CreatePSO(std::shared_ptr<D3D12Device>& device, std::string psoName, D3D12_COMPUTE_PIPELINE_STATE_DESC & gps, ID3DBlob * rootSignature)
{
	return E_NOTIMPL;
}

HRESULT Hashira::ShaderObjectManager::CreateRootSignature(std::string rsName, ID3DBlob * signature)
{
	if (_rootSignaturelibrary.find(rsName) == _rootSignaturelibrary.end()) {
		this->_rootSignaturelibrary[rsName] = std::make_shared<Hashira::RootSignature>();
		auto hr = this->_rootSignaturelibrary[rsName]->CreateFromShader(signature);
		this->_rootSignaturelibrary[rsName]->SetName(rsName);

		if (SUCCEEDED(hr)) {
			DEBUG_LOG(std::string("RootSignature‚ª³í‚Éì¬E“o˜^‚³‚ê‚Ü‚µ‚½ : " + rsName));

			return hr;
		}
		return hr;
	}
	return E_FAIL;
}

HRESULT Hashira::ShaderObjectManager::CreateRootSignature(std::string rsName, D3D12_ROOT_SIGNATURE_DESC * desc)
{
	if (_rootSignaturelibrary.find(rsName) == _rootSignaturelibrary.end()) {
		this->_rootSignaturelibrary[rsName] = std::make_shared<Hashira::RootSignature>();
		auto hr = this->_rootSignaturelibrary[rsName]->CreateFromDesc(desc);
		this->_rootSignaturelibrary[rsName]->SetName(rsName);

		if (SUCCEEDED(hr)) {
			DEBUG_LOG(std::string("RootSignature‚ª³í‚Éì¬E“o˜^‚³‚ê‚Ü‚µ‚½ : " + rsName));

			return hr;
		}
		return hr;
	}
	return E_FAIL;
}

void Hashira::ShaderObjectManager::SetPSO(std::string psoName, std::shared_ptr<Hashira::PipelineStateObject> pso)
{
	if (_psolibrary.find(psoName) == _psolibrary.end()) {
		this->_psolibrary[psoName] = pso;
		DEBUG_LOG(std::string("PSO‚ğ“o˜^‚µ‚Ü‚µ‚½ : " + psoName));

	}
}

void Hashira::ShaderObjectManager::SetRootSignature(std::string rsName, std::shared_ptr<Hashira::RootSignature> rootSignature)
{
	if (_rootSignaturelibrary.find(rsName) == _rootSignaturelibrary.end()) {
		this->_rootSignaturelibrary[rsName] = rootSignature;
		DEBUG_LOG(std::string("RootSignature‚ğ“o˜^‚µ‚Ü‚µ‚½ : " + rsName ));
	}
}

std::weak_ptr<Hashira::PipelineStateObject> Hashira::ShaderObjectManager::GetPSO(std::string psoName)
{
	if (_psolibrary.find(psoName) != _psolibrary.end()) {
		return  this->_psolibrary[psoName];
	}
	ERROR_LOG(std::string("•s³ŒŸõƒ[ƒh‚Å‚· : " + psoName ));

	return std::weak_ptr<Hashira::PipelineStateObject>();
}

std::weak_ptr<Hashira::RootSignature> Hashira::ShaderObjectManager::GetRootSignature(std::string rsName)
{
	if (_rootSignaturelibrary.find(rsName) != _rootSignaturelibrary.end()) {
		return  this->_rootSignaturelibrary[rsName];
	}
	ERROR_LOG(std::string("•s³ŒŸõƒ[ƒh‚Å‚· : " + rsName));

	return std::weak_ptr<Hashira::RootSignature>();
}

void Hashira::ShaderObjectManager::ErasePSO(std::string psoName)
{
	if (_psolibrary.find(psoName) != _psolibrary.end()) {
		this->_psolibrary.erase(psoName);
		DEBUG_LOG(std::string("PSO‚ğíœ‚µ‚Ü‚µ‚½ : " + psoName));

	}
}

void Hashira::ShaderObjectManager::EraseRootSignature(std::string rsName)
{
	if (_rootSignaturelibrary.find(rsName) != _rootSignaturelibrary.end()) {
		this->_rootSignaturelibrary.erase(rsName);
		DEBUG_LOG(std::string("RootSignature‚ğíœ‚µ‚Ü‚µ‚½ : " + rsName));

	}
}

void Hashira::ShaderObjectManager::Discard()
{

	this->_psolibrary.clear();
	this->_rootSignaturelibrary.clear();
}
