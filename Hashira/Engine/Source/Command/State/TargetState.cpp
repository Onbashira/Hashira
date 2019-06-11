#include "TargetState.h"

Hashira::TargetState::TargetState()
{
}

Hashira::TargetState::~TargetState()
{
}

void Hashira::TargetState::AddRenderTargetHandle(D3D12_CPU_DESCRIPTOR_HANDLE & handle)
{
	this->_rtvCPUHandles.push_back(handle);
}

void Hashira::TargetState::SetDepthStencilHandle(D3D12_CPU_DESCRIPTOR_HANDLE & handle)
{
	this->_dsvCPUHandle = handle;

}

void Hashira::TargetState::AddViewport(D3D12_VIEWPORT && viewport)
{
	this->_viewports.push_back(std::move(viewport));

}

void Hashira::TargetState::AddScissorRects(D3D12_RECT && scissor)
{
	this->_scissorRects.push_back(std::move(scissor));
}
