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


HRESULT Hashira::RootSignature::InitializeFromShader(ID3DBlob * shader)
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

HRESULT Hashira::RootSignature::InitializeFromDesc(D3D12_ROOT_SIGNATURE_DESC * desc)
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

HRESULT Hashira::RootSignature::InitializeFromDesc(std::shared_ptr<D3D12Device>& device, RootSignatureDesc & desc)
{
	constexpr Uint32 MaxParameters = 64;

	D3D12_DESCRIPTOR_RANGE range[MaxParameters];
	D3D12_ROOT_PARAMETER rootParam[MaxParameters];

	//レンジ取得ラムダ式作成
	auto GetRangeFunc = [&](Uint32 index) {
		static const D3D12_DESCRIPTOR_RANGE_TYPE type[] = {
			D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_CBV,
			D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_SRV,
			D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_UAV,
			D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER,

		};
		return type[desc.pParameters[index].type];
	};

	//レンジ取得ラムダ式作成
	auto GetShaderVisibilityFunc = [&](Uint32 index) {
		ShaderVisibility::Type v; ;
		switch (desc.pParameters[index].shaderVisibility)
		{
		case Hashira::ShaderVisibility::Vertex: return D3D12_SHADER_VISIBILITY_VERTEX;
		case Hashira::ShaderVisibility::Pixel:return D3D12_SHADER_VISIBILITY_PIXEL;
		case Hashira::ShaderVisibility::Geometry:return D3D12_SHADER_VISIBILITY_GEOMETRY;
		case Hashira::ShaderVisibility::Domain:return D3D12_SHADER_VISIBILITY_DOMAIN;
		case Hashira::ShaderVisibility::Hull:return D3D12_SHADER_VISIBILITY_HULL;
		case Hashira::ShaderVisibility::Compute:
		case Hashira::ShaderVisibility::All:
		default:
			return D3D12_SHADER_VISIBILITY_ALL;
			break;
		}
	};

	//セット処理
	for (int i = 0; i < desc.numParameters;++i)
	{
		range[i].RangeType = GetRangeFunc(i);
		range[i].NumDescriptors = 1;
		range[i].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
		range[i].RegisterSpace = 0;
		range[i].BaseShaderRegister = desc.pParameters[i].registerIndex;

		rootParam[i].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParam[i].DescriptorTable.NumDescriptorRanges = 1;
		rootParam[i].DescriptorTable.pDescriptorRanges = &range[i];
		rootParam[i].ShaderVisibility = GetShaderVisibilityFunc(i);

	}

	D3D12_ROOT_SIGNATURE_DESC rsDesc = {};
	rsDesc.NumParameters = desc.numParameters;
	rsDesc.NumStaticSamplers = 0;
	rsDesc.pParameters = rootParam;
	rsDesc.pStaticSamplers = nullptr;
	rsDesc.Flags = desc.flags;

	//イニシャライズ処理
	Microsoft::WRL::ComPtr<ID3DBlob> rs;
	Microsoft::WRL::ComPtr<ID3DBlob> error;
	auto hr = D3D12SerializeRootSignature(&rsDesc, D3D_ROOT_SIGNATURE_VERSION::D3D_ROOT_SIGNATURE_VERSION_1, rs.GetAddressOf(), error.GetAddressOf());
	if (FAILED(hr))
	{

		SafeRelease(rs);
		SafeRelease(error);
		return hr;
	}

	hr = device->GetDevice()->CreateRootSignature(1, rs->GetBufferPointer(), rs->GetBufferSize(), IID_PPV_ARGS(&_rootSignature));
	if (FAILED(hr))
	{
		return hr;
	}
	SafeRelease(rs);
	SafeRelease(error);

	return hr;
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