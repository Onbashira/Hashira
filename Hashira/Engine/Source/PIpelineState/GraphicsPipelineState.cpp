#include "stdafx.h"
#include "GraphicsPipelineState.h"
#include "DefaultPipelineStates.h"
#include "..//Signature/RootSignature.h"
#include "Engine/Source/ShaderObject/Shader.h"
#include "Engine/Source/Device/D3D12Device.h"

Hashira::RenderTargetBlendDesc::RenderTargetBlendDesc() :
	isBlendEnable(false),
	isLogicBlendEnable(false),
	srcBlendColor(D3D12_BLEND::D3D12_BLEND_ONE),
	dstBlendColor(D3D12_BLEND::D3D12_BLEND_ZERO),
	blendOpColor(D3D12_BLEND_OP::D3D12_BLEND_OP_MAX),
	srcBlendAlpha(D3D12_BLEND::D3D12_BLEND_ONE),
	dstBlendAlpha(D3D12_BLEND::D3D12_BLEND_ZERO),
	blendOpAlpha(D3D12_BLEND_OP::D3D12_BLEND_OP_MAX),
	logicOp(D3D12_LOGIC_OP::D3D12_LOGIC_OP_AND),
	writeMask(D3D12_COLOR_WRITE_ENABLE_ALL)
{
}

Hashira::RenderTargetBlendDesc::~RenderTargetBlendDesc()
{
}

Hashira::BlendDesc::BlendDesc() :
	isAlphaToCoverageEnable(false),
	isIndependentBlend(false),
	sampleMask(0xffff)

{

	auto defaultStates = Hashira::DefaultRenderTargetBlendNone();
	for (unsigned int i = 0; i < 8; i++) {
		rtDesc[i] = defaultStates;
	}
}

Hashira::BlendDesc::~BlendDesc()
{
}

Hashira::RasterizerDesc::RasterizerDesc() :
	fillMode(D3D12_FILL_MODE_SOLID),
	cullMode(D3D12_CULL_MODE_FRONT),
	isFrontCCW(false),
	depthBias(D3D12_DEFAULT_DEPTH_BIAS),
	depthBiasClamp(D3D12_DEFAULT_DEPTH_BIAS_CLAMP),
	slopeScaledDepthBias(D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS),
	isDepthClipEnable(true),
	isMultisampleEnable(false),
	isAntialiasedLineEnable(false),
	isConservativeRasterEnable(false)
{
}

Hashira::RasterizerDesc::~RasterizerDesc()
{
}

Hashira::DepthStencilDesc::DepthStencilDesc() :
	isDepthEnable(false),
	isDepthWriteEnable(true),
	depthFunc(D3D12_COMPARISON_FUNC::D3D12_COMPARISON_FUNC_LESS),
	isStencilEnable(false),
	stencilReadMask(0),
	stencilWriteMask(0x00ff),
	stencilFrontFace(D3D12_DEPTH_STENCILOP_DESC()),
	stencilBackFace(D3D12_DEPTH_STENCILOP_DESC())
{
	stencilFrontFace.StencilDepthFailOp;
	stencilFrontFace.StencilFailOp;
	stencilFrontFace.StencilFunc;
	stencilFrontFace.StencilPassOp;

	stencilBackFace.StencilDepthFailOp;
	stencilBackFace.StencilFailOp;
	stencilBackFace.StencilFunc;
	stencilBackFace.StencilPassOp;

}

Hashira::DepthStencilDesc::~DepthStencilDesc()
{
}

Hashira::InputLayoutDesc::InputLayoutDesc() :
	pElements(nullptr),
	numElements(0)

{
}

Hashira::InputLayoutDesc::~InputLayoutDesc()
{

}

Hashira::GraphicsPipelineStateDesc::GraphicsPipelineStateDesc() :
	rootSignature(nullptr),
	vs(nullptr),
	ps(nullptr),
	gs(nullptr),
	ds(nullptr),
	hs(nullptr),
	cs(nullptr),
	blendDesc(),
	rasterizerDesc(),
	depthStencilDesc(),
	inputLayout(),
	primitiveTopology(D3D_PRIMITIVE_TOPOLOGY_UNDEFINED),
	numRTVs(0),
	rtvFormats(),
	dsvFormat(DXGI_FORMAT::DXGI_FORMAT_UNKNOWN),
	multiSamplerCount(1)
{
	for (int i = 0; i < 8; i++)
	{
		rtvFormats[i] = DXGI_FORMAT_UNKNOWN;
	}

}

Hashira::GraphicsPipelineStateDesc::~GraphicsPipelineStateDesc()
{
}

Hashira::GraphicsPipelineState::GraphicsPipelineState()
{
}

Hashira::GraphicsPipelineState::~GraphicsPipelineState()
{
	Discard();
}

HRESULT Hashira::GraphicsPipelineState::Initialize(std::shared_ptr<D3D12Device>& device, const GraphicsPipelineStateDesc& desc)
{

	if (!desc.rootSignature) {
		return E_FAIL;
	}

	auto RTBlendFunc = [&](D3D12_RENDER_TARGET_BLEND_DESC& dst, Uint32 index)
	{
		auto&& src = desc.blendDesc.rtDesc[index];
		dst.BlendEnable = src.isBlendEnable;
		dst.LogicOpEnable = src.isLogicBlendEnable;
		dst.SrcBlend = src.srcBlendColor;
		dst.DestBlend = src.dstBlendColor;
		dst.BlendOp = src.blendOpColor;
		dst.SrcBlendAlpha = src.srcBlendAlpha;
		dst.DestBlendAlpha = src.dstBlendAlpha;
		dst.BlendOpAlpha = src.blendOpAlpha;
		dst.LogicOp = src.logicOp;
		dst.RenderTargetWriteMask = src.writeMask;
	};

	auto BlendFunc = [&](D3D12_BLEND_DESC& dst)
	{
		dst.AlphaToCoverageEnable = desc.blendDesc.isAlphaToCoverageEnable;
		dst.IndependentBlendEnable = desc.blendDesc.isIndependentBlend;
		RTBlendFunc(dst.RenderTarget[0], 0);
		if (dst.IndependentBlendEnable)
		{
			for (Uint32 i = 1; i < 8; i++)
			{
				RTBlendFunc(dst.RenderTarget[i], i);
			}
		}
	};

	auto RasterFunc = [&](D3D12_RASTERIZER_DESC& dst)
	{
		dst.FillMode = desc.rasterizerDesc.fillMode;
		dst.CullMode = desc.rasterizerDesc.cullMode;
		dst.FrontCounterClockwise = desc.rasterizerDesc.isFrontCCW;
		dst.DepthBias = desc.rasterizerDesc.depthBias;
		dst.DepthBiasClamp = desc.rasterizerDesc.depthBiasClamp;
		dst.SlopeScaledDepthBias = desc.rasterizerDesc.slopeScaledDepthBias;
		dst.DepthClipEnable = desc.rasterizerDesc.isDepthClipEnable;
		dst.MultisampleEnable = desc.rasterizerDesc.isMultisampleEnable;
		dst.AntialiasedLineEnable = desc.rasterizerDesc.isAntialiasedLineEnable;
		dst.ForcedSampleCount = 0;
		dst.ConservativeRaster = desc.rasterizerDesc.isConservativeRasterEnable ? D3D12_CONSERVATIVE_RASTERIZATION_MODE_ON : D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
	};

	auto DepthFunc = [&](D3D12_DEPTH_STENCIL_DESC& dst)
	{
		dst.DepthEnable = desc.depthStencilDesc.isDepthEnable;
		dst.DepthWriteMask = desc.depthStencilDesc.isDepthWriteEnable ? D3D12_DEPTH_WRITE_MASK_ALL : D3D12_DEPTH_WRITE_MASK_ZERO;
		dst.DepthFunc = desc.depthStencilDesc.depthFunc;
		dst.StencilEnable = desc.depthStencilDesc.isStencilEnable;
		dst.StencilReadMask = desc.depthStencilDesc.stencilReadMask;
		dst.StencilWriteMask = desc.depthStencilDesc.stencilWriteMask;
		dst.FrontFace = desc.depthStencilDesc.stencilFrontFace;
		dst.BackFace = desc.depthStencilDesc.stencilBackFace;
	};

	auto TopologyFunc = [&](D3D_PRIMITIVE_TOPOLOGY t)
	{
		D3D12_PRIMITIVE_TOPOLOGY_TYPE kTypes[] = {
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_UNDEFINED,		// D3D_PRIMITIVE_TOPOLOGY_UNDEFINED
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT,			// D3D_PRIMITIVE_TOPOLOGY_POINTLIST
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE,				// D3D_PRIMITIVE_TOPOLOGY_LINELIST
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE,				// D3D_PRIMITIVE_TOPOLOGY_LINESTRIP
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE,			// D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE,			// D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE,				// D3D_PRIMITIVE_TOPOLOGY_LINELIST_ADJ
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE,				// D3D_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE,			// D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE,			// D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ
		};
		return ((t < D3D_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST) ? kTypes[t] : D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH);
	};

	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	psoDesc.InputLayout = { desc.inputLayout.pElements, desc.inputLayout.numElements };
	psoDesc.pRootSignature = desc.rootSignature->GetSignature().Get();
	if (desc.vs)
	{
		psoDesc.VS.pShaderBytecode = desc.vs->GetShader(Shader::Type::SHADER_TYPE_VERTEX)->GetBufferPointer();
		psoDesc.VS.BytecodeLength = desc.vs->GetShader(Shader::Type::SHADER_TYPE_VERTEX)->GetBufferSize();

	}
	if (desc.ps)
	{
		psoDesc.PS.pShaderBytecode = desc.ps->GetShader(Shader::Type::SHADER_TYPE_PIXEL)->GetBufferPointer();
		psoDesc.PS.BytecodeLength = desc.ps->GetShader(Shader::Type::SHADER_TYPE_PIXEL)->GetBufferSize();

	}
	if (desc.gs)
	{
		psoDesc.GS = { reinterpret_cast<const UINT8*>(
		desc.gs->GetShader(Shader::Type::SHADER_TYPE_GEOMETRY)->GetBufferPointer()),
		desc.gs->GetShader(Shader::Type::SHADER_TYPE_GEOMETRY)->GetBufferSize() };
	}
	if (desc.ds)
	{
		psoDesc.DS = { reinterpret_cast<const UINT8*>(
		desc.ds->GetShader(Shader::Type::SHADER_TYPE_DOMAIN)->GetBufferPointer()),
		desc.ds->GetShader(Shader::Type::SHADER_TYPE_DOMAIN)->GetBufferSize() };
	}
	if (desc.hs)
	{
		psoDesc.HS = { reinterpret_cast<const UINT8*>(
		desc.hs->GetShader(Shader::Type::SHADER_TYPE_HULL)->GetBufferPointer()),
		desc.hs->GetShader(Shader::Type::SHADER_TYPE_HULL)->GetBufferSize() };
	}

	BlendFunc(psoDesc.BlendState);
	psoDesc.SampleMask = desc.blendDesc.sampleMask;
	RasterFunc(psoDesc.RasterizerState);
	DepthFunc(psoDesc.DepthStencilState);
	psoDesc.PrimitiveTopologyType = TopologyFunc(desc.primitiveTopology);
	psoDesc.NumRenderTargets = desc.numRTVs;
	for (Uint32 i = 0; i < desc.numRTVs; i++)
	{
		psoDesc.RTVFormats[i] = desc.rtvFormats[i];
	}
	psoDesc.DSVFormat = desc.dsvFormat;
	psoDesc.SampleDesc.Count = desc.multiSamplerCount;

	auto hr = device->GetDevice()->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&_pso));
	if (FAILED(hr))
	{
		return hr;
	}

	return hr;
}

Microsoft::WRL::ComPtr<ID3D12PipelineState>& Hashira::GraphicsPipelineState::GetPSO()
{
	return _pso;
}

void Hashira::GraphicsPipelineState::Discard()
{
	_pso.Reset();
}
