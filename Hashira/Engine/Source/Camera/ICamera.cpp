#include "stdafx.h"
#include "ICamera.h"
#include "Engine/Source/Rendering/RenderContext/RenderContext.h"
#include "Engine/Source/Device/RenderingDevice.h"
#include "Engine/Source/Buffer/DepthStencilBuffer.h"
Hashira::ICamera::ICamera() :GameObject()
{
}

Hashira::ICamera::~ICamera()
{
}

HRESULT Hashira::ICamera::Initialize(std::shared_ptr<RenderContext>& context, const CameraInitInfo& cameraInfo)
{
	auto& d3d12Dev = context->GetRenderingDevice()->GetD3D12Device();
	auto hr = _transformBuffer.Initialize(context->GetRenderingDevice()->GetD3D12Device(), sizeof(CameraInfo));
	if (FAILED(hr))
	{
		return hr;
	}
	DepthStencil* stencil = new DepthStencil();
	stencil->Initialize(d3d12Dev, cameraInfo.width, cameraInfo.height,
		DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT_S8X24_UINT, DXGI_FORMAT::DXGI_FORMAT_R32G32_UINT);
	this->_depthStencil.reset(stencil);

	Update();
	return hr;
}

const Hashira::CameraInfo& Hashira::ICamera::GetCameraInfo() const
{
	return _cameraInfo;
}

const D3D12_VIEWPORT* Hashira::ICamera::GetViewport(Uint32 index)
{
	return _viewports.data();
}

const Hashira::Uint32 Hashira::ICamera::GetViewportNum()
{
	return _viewports.size();
}

std::vector<D3D12_VIEWPORT>& Hashira::ICamera::GetViewportArray()
{
	return _viewports;

}

const D3D12_RECT* Hashira::ICamera::GetScissor()
{
	return &_scissorRect;
}

void Hashira::ICamera::ChangeFov(float fov)
{
	CameraInitInfo info{};
	info = &this->_cameraInfo;
	info.fov = fov;
	ChangeParameter(info, false);

}

void Hashira::ICamera::ChangeNearFarClip(const Vector2& clipRange)
{
	CameraInitInfo info{};
	info = &this->_cameraInfo;

	info.nearClip = clipRange.x;
	info.farClip = clipRange.y;

	ChangeParameter(info, false);
}

void Hashira::ICamera::ChangeParameter(const CameraInitInfo& cameraInfo , bool changeTargetPos)
{
	//

	this->_cameraInfo = cameraInfo;

	_transform.SetPos(cameraInfo.position);
	if (changeTargetPos)
	{
		_transform.LookAt(cameraInfo.target, cameraInfo.up);
	}

	this->_cameraInfo.view = this->_transform.GetView();
	this->_cameraInfo.invView = Matrix::Invert(_cameraInfo.view);

	Update();

}

void Hashira::ICamera::AddViewport(const D3D12_VIEWPORT& viewport)
{
	this->_viewports.push_back(viewport);
}

void Hashira::ICamera::SetScissorRect(const D3D12_RECT& rect)
{
	this->_scissorRect = rect;
}

Hashira::DescriptorInfo* Hashira::ICamera::GetDescriptor()
{
	return _descInfo.data();
}

Hashira::DescriptorInfo& Hashira::ICamera::GetDSVDescriptor()
{
	// TODO: return ステートメントをここに挿入します
}

Hashira::DescriptorInfo& Hashira::ICamera::GetRTVDescriptor()
{
	// TODO: return ステートメントをここに挿入します
}

Hashira::DescriptorInfo& Hashira::ICamera::GetCameraCBVDescriptor()
{
	// TODO: return ステートメントをここに挿入します
}

void Hashira::ICamera::Update()
{
	this->_transformBuffer.Alignment256ByteUpdate(&this->_cameraInfo, sizeof(CameraInfo));
}

void Hashira::ICamera::Discard()
{
	this->_depthStencil->Discard();
	this->_renderTarget->Discard();
	for (auto& desc : this->_descInfo)
	{
		desc.Free();
	}
}

void Hashira::ICamera::InitializeDepthStencil(std::shared_ptr<D3D12Device>& dev, DXGI_FORMAT depthFormat, DXGI_FORMAT clearFormat)
{
}

void Hashira::ICamera::InitializeRenderTarget(std::shared_ptr<D3D12Device>& dev, DXGI_FORMAT depthFormat, DXGI_FORMAT clearFormat, float* clearColor)
{
}
