#include "PipelineStateObject.h"
#include "Engine/Source/CoreSystem/Framework.h"
#include "Engine/Source/GraphicsManager/GraphicsContextManager.h"
#include "Engine/Source/Signature/RootSignature.h"
#include "Engine/Source/Debug/Logger.h"
#include "Engine/Source/Utility/Utility.h"
#include <string>

Hashira::PipelineStateObject::PipelineStateObject() :
	_pipelineState(), _name("UnNamed PipelineState")
{

}

Hashira::PipelineStateObject::~PipelineStateObject()
{
	Discard();
}


HRESULT Hashira::PipelineStateObject::Initialize(std::string name, D3D12_GRAPHICS_PIPELINE_STATE_DESC & gpsDesc, ID3DBlob * shader)
{
	if (shader != nullptr) {
		GraphicsContextManager::GetInstance().CreateRootSignature(name + "RootSignature", shader);
		_rootSignature = GraphicsContextManager::GetInstance().GetRootSignature(name + "RootSignature");
		gpsDesc.pRootSignature = _rootSignature.lock()->GetSignature().Get();
	}
	auto hr = Framework::GetInstance().GetDevice()->GetDevice()->CreateGraphicsPipelineState(&gpsDesc, IID_PPV_ARGS(&_pipelineState));
	return hr;
}

HRESULT Hashira::PipelineStateObject::Initialize(std::shared_ptr<D3D12Device>& device, std::string name, D3D12_GRAPHICS_PIPELINE_STATE_DESC & gpsDesc, ID3DBlob * shader)
{
	if (shader != nullptr) {
		GraphicsContextManager::GetInstance().CreateRootSignature(name + "RootSignature", shader);
		_rootSignature = GraphicsContextManager::GetInstance().GetRootSignature(name + "RootSignature");
		gpsDesc.pRootSignature = _rootSignature.lock()->GetSignature().Get();
	}
	auto hr = device->GetDevice()->CreateGraphicsPipelineState(&gpsDesc, IID_PPV_ARGS(&_pipelineState));
	return hr;
}

HRESULT Hashira::PipelineStateObject::Initialize(std::string name, D3D12_COMPUTE_PIPELINE_STATE_DESC & cpsDesc, ID3DBlob * shader)
{
	if (shader != nullptr) {
		GraphicsContextManager::GetInstance().CreateRootSignature(name + "RootSignature", shader);
		_rootSignature = GraphicsContextManager::GetInstance().GetRootSignature(name + "RootSignature");
		cpsDesc.pRootSignature = _rootSignature.lock()->GetSignature().Get();
	}

	auto hr = Framework::GetInstance().GetDevice()->GetDevice()->CreateComputePipelineState(&cpsDesc, IID_PPV_ARGS(&_pipelineState));
	SetName(name);

	return hr;
}

HRESULT Hashira::PipelineStateObject::Initialize(std::shared_ptr<D3D12Device>& device, std::string name, D3D12_COMPUTE_PIPELINE_STATE_DESC & cpsDesc, ID3DBlob * shader)
{
	if (shader != nullptr) {
		GraphicsContextManager::GetInstance().CreateRootSignature(name + "RootSignature", shader);
		_rootSignature = GraphicsContextManager::GetInstance().GetRootSignature(name + "RootSignature");
		cpsDesc.pRootSignature = _rootSignature.lock()->GetSignature().Get();
	}

	auto hr = device->GetDevice()->CreateComputePipelineState(&cpsDesc, IID_PPV_ARGS(&_pipelineState));
	SetName(name);

	return hr;
}

HRESULT Hashira::PipelineStateObject::Initialize(std::string name, D3D12_GRAPHICS_PIPELINE_STATE_DESC & gpsDesc, std::weak_ptr<RootSignature> rootSignature)
{
	_rootSignature = rootSignature;
	gpsDesc.pRootSignature = _rootSignature.lock()->GetSignature().Get();
	auto hr = Framework::GetInstance().GetDevice()->GetDevice()->CreateGraphicsPipelineState(&gpsDesc, IID_PPV_ARGS(&_pipelineState));
	SetName(name);

	return hr;
}

HRESULT Hashira::PipelineStateObject::Initialize(std::shared_ptr<D3D12Device>& device, std::string name, D3D12_GRAPHICS_PIPELINE_STATE_DESC & gpsDesc, std::weak_ptr<RootSignature> rootSignature)
{
	_rootSignature = rootSignature;
	gpsDesc.pRootSignature = _rootSignature.lock()->GetSignature().Get();
	auto hr = device->GetDevice()->CreateGraphicsPipelineState(&gpsDesc, IID_PPV_ARGS(&_pipelineState));
	SetName(name);

	return hr;
}

HRESULT Hashira::PipelineStateObject::Initialize(std::string name, D3D12_COMPUTE_PIPELINE_STATE_DESC & cpsDesc, std::weak_ptr<RootSignature> rootSignature)
{
	_rootSignature = rootSignature;
	cpsDesc.pRootSignature = _rootSignature.lock()->GetSignature().Get();

	auto hr = Framework::GetInstance().GetDevice()->GetDevice()->CreateComputePipelineState(&cpsDesc, IID_PPV_ARGS(&_pipelineState));
	SetName(name);

	return hr;
}

HRESULT Hashira::PipelineStateObject::Initialize(std::shared_ptr<D3D12Device>& device, std::string name, D3D12_COMPUTE_PIPELINE_STATE_DESC & cpsDesc, std::weak_ptr<RootSignature> rootSignature)
{
	_rootSignature = rootSignature;
	cpsDesc.pRootSignature = _rootSignature.lock()->GetSignature().Get();

	auto hr = device->GetDevice()->CreateComputePipelineState(&cpsDesc, IID_PPV_ARGS(&_pipelineState));
	SetName(name);

	return hr;
}

void Hashira::PipelineStateObject::SetName(std::string name)
{
	_pipelineState->SetName(Util::StringToWString(name).c_str());
	_name = name;
}

std::weak_ptr<Hashira::RootSignature> Hashira::PipelineStateObject::GetRootSignature()
{
	return this->_rootSignature;
}

Microsoft::WRL::ComPtr<ID3D12PipelineState>& Hashira::PipelineStateObject::GetPSO()
{
	return this->_pipelineState;
}

void Hashira::PipelineStateObject::Discard()
{
	if (_pipelineState.Get() != nullptr) {
		_pipelineState.Reset();
		
		DEBUG_LOG(std::string("PipelineState : " + _name + " is  Reset"));
	}
	_rootSignature.reset();
}
