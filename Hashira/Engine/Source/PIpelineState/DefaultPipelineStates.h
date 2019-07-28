#pragma once

#include "./GraphicsPipelineState.h"

namespace Hashira
{
	//RT
	RenderTargetBlendDesc DefaultRenderTargetBlendNone();
	RenderTargetBlendDesc DefaultRenderTargetBlendAlpha();
	RenderTargetBlendDesc DefaultRenderTargetBlendAdd();
	RenderTargetBlendDesc DefaultRenderTargetBlendSub();
	RenderTargetBlendDesc DefaultRenderTargetBlendMul();

	DepthStencilDesc DefaultDepthStateDisableDisable();
	DepthStencilDesc DefaultDepthStateEnableDisable();
	DepthStencilDesc DefaultDepthStateEnableEnable();

	RasterizerDesc DefaultRasterizerStateStandard(int depth_bias = D3D12_DEFAULT_DEPTH_BIAS, float depth_clamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP, float slope_bias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS);

}