#include "stdafx.h"
#include "DefaultPipelineStates.h"

Hashira::RenderTargetBlendDesc Hashira::DefaultRenderTargetBlendNone()
{
	RenderTargetBlendDesc ret{};
	ret.isBlendEnable = false;
	ret.isLogicBlendEnable = false;
	ret.writeMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	return ret;
}

Hashira::RenderTargetBlendDesc Hashira::DefaultRenderTargetBlendAlpha()
{
	RenderTargetBlendDesc ret{};
	ret.isBlendEnable = true;
	ret.isLogicBlendEnable = false;
	ret.srcBlendColor = D3D12_BLEND_SRC_ALPHA;
	ret.dstBlendColor = D3D12_BLEND_INV_SRC_ALPHA;
	ret.blendOpColor = D3D12_BLEND_OP_ADD;
	ret.srcBlendAlpha = D3D12_BLEND_ONE;
	ret.dstBlendAlpha = D3D12_BLEND_INV_SRC_ALPHA;
	ret.blendOpAlpha = D3D12_BLEND_OP_ADD;
	ret.writeMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	return ret;
}

Hashira::RenderTargetBlendDesc Hashira::DefaultRenderTargetBlendAdd()
{
	RenderTargetBlendDesc ret{};
	ret.isBlendEnable = true;
	ret.isLogicBlendEnable = false;
	ret.srcBlendColor = D3D12_BLEND_SRC_ALPHA;
	ret.dstBlendColor = D3D12_BLEND_ONE;
	ret.blendOpColor = D3D12_BLEND_OP_ADD;
	ret.srcBlendAlpha = D3D12_BLEND_ONE;
	ret.dstBlendAlpha = D3D12_BLEND_ONE;
	ret.blendOpAlpha = D3D12_BLEND_OP_ADD;
	ret.writeMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	return ret;
}

Hashira::RenderTargetBlendDesc Hashira::DefaultRenderTargetBlendSub()
{
	RenderTargetBlendDesc ret{};
	ret.isBlendEnable = true;
	ret.isLogicBlendEnable = false;
	ret.srcBlendColor = D3D12_BLEND_SRC_ALPHA;
	ret.dstBlendColor = D3D12_BLEND_ONE;
	ret.blendOpColor = D3D12_BLEND_OP_SUBTRACT;
	ret.srcBlendAlpha = D3D12_BLEND_ONE;
	ret.dstBlendAlpha = D3D12_BLEND_ONE;
	ret.blendOpAlpha = D3D12_BLEND_OP_SUBTRACT;
	ret.writeMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	return ret;
}

Hashira::RenderTargetBlendDesc Hashira::DefaultRenderTargetBlendMul()
{
	RenderTargetBlendDesc ret{};
	ret.isBlendEnable = true;
	ret.isLogicBlendEnable = false;
	ret.srcBlendColor = D3D12_BLEND_DEST_COLOR;
	ret.dstBlendColor = D3D12_BLEND_ZERO;
	ret.blendOpColor = D3D12_BLEND_OP_ADD;
	ret.srcBlendAlpha = D3D12_BLEND_DEST_ALPHA;
	ret.dstBlendAlpha = D3D12_BLEND_ZERO;
	ret.blendOpAlpha = D3D12_BLEND_OP_ADD;
	ret.writeMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	return ret;
}

Hashira::DepthStencilDesc Hashira::DefaultDepthStateDisableDisable()
{
	DepthStencilDesc ret{};
	ret.isDepthEnable = false;
	ret.isDepthWriteEnable = false;
	ret.depthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	return ret;
}

Hashira::DepthStencilDesc Hashira::DefaultDepthStateEnableDisable()
{
	DepthStencilDesc ret{};
	ret.isDepthEnable = true;
	ret.isDepthWriteEnable = false;
	ret.depthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	return ret;
}

Hashira::DepthStencilDesc Hashira::DefaultDepthStateEnableEnable()
{
	DepthStencilDesc ret{};
	ret.isDepthEnable = true;
	ret.isDepthWriteEnable = true;
	ret.depthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	return ret;
}

Hashira::RasterizerDesc Hashira::DefaultRasterizerStateStandard(int depth_bias, float depth_clamp, float slope_bias)
{
	RasterizerDesc ret{};
	ret.fillMode = D3D12_FILL_MODE_SOLID;
	ret.cullMode = D3D12_CULL_MODE_FRONT;
	ret.isFrontCCW = false;
	ret.depthBias = depth_bias;
	ret.depthBiasClamp = depth_clamp;
	ret.slopeScaledDepthBias = slope_bias;
	ret.isDepthClipEnable = true;
	ret.isMultisampleEnable = false;
	ret.isAntialiasedLineEnable = false;
	ret.isConservativeRasterEnable = false;
	return ret;
}
