#include "stdafx.h"
#include "Camera.h"
#include "Engine/Source/Rendering/RenderContext/RenderContext.h"
#include "Engine/Source/Device/RenderingDevice.h"
#include "Engine/Source/Device/D3D12Device.h"
#include "Engine/Source/Buffer/DepthStencilBuffer.h"
#include "Engine/Source/Buffer/ShaderResource.h"
#include "DefaultComponent/CameraGraphicsComponent.h"
#include "DefaultComponent/CameraInputComponent.h"
#include "DefaultComponent/CameraPhysicsComponent.h"
#include "Engine/Source/CommandList/CommandList.h"

Hashira::Camera::Camera() :
	GameObject(new CameraGraphicsComponent(), new CameraInputComponent(), new CameraPhysicsComponent()),
	_cameraType(CAMERA_TYPE::Perspective)
{
}

Hashira::Camera::~Camera()
{
}

HRESULT Hashira::Camera::Initialize(std::shared_ptr<RenderContext>& context, const CameraInitInfo& cameraInfo)
{
	auto& d3d12Dev = context->GetRenderingDevice()->GetD3D12Device();
	auto hr = _transformBuffer.Initialize(context->GetRenderingDevice()->GetD3D12Device(), sizeof(CameraInfo));
	if (FAILED(hr))
	{
		return hr;
	}

	hr = this->InitializeDepthStencil(d3d12Dev, DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT, DXGI_FORMAT::DXGI_FORMAT_R32_TYPELESS);
	if (FAILED(hr))
	{
		return hr;
	}

	{
		_cameraCbvDescriptor = context->GetViewDescriptorHeap()->Allocate();
		D3D12_CONSTANT_BUFFER_VIEW_DESC desc{};
		desc.BufferLocation = this->_transformBuffer.GetResource()->GetGPUVirtualAddress();
		desc.SizeInBytes = static_cast<Uint32>(this->_transformBuffer.GetResourceDesc().Width);
		context->GetRenderingDevice()->GetD3D12Device()->CreateConstantBufferView(&_cameraCbvDescriptor, &desc);

	}

	{

		D3D12_DEPTH_STENCIL_VIEW_DESC desc{};
		desc.Flags = D3D12_DSV_FLAGS::D3D12_DSV_FLAG_NONE;
		desc.Format = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
		desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipSlice = 1;

		for (int i = 0; i < CameraDepthStencilMax; ++i)
		{
			_cameraDsvDescriptors.Set(i, context->GetDsvDescriptorHeap()->Allocate());
			context->GetRenderingDevice()->GetD3D12Device()->CreateDepthStencilView(this->_depthStencils.Get().get(), &_cameraDsvDescriptors.Get(), &desc);
		}
	}

	D3D12_RECT scissor{};
	scissor.bottom = LONG(_cameraInfo.windowHeight);
	scissor.right = LONG(_cameraInfo.windowWidth);
	SetScissorRect(scissor);

	D3D12_VIEWPORT port{ };
	port.Height = _cameraInfo.windowHeight;
	port.Width = _cameraInfo.windowWidth;
	port.TopLeftX = 0.0f;
	port.TopLeftY = 0.0f;
	port.MaxDepth = 1.0f;
	port.MinDepth = 0.0f;
	Update();
	return hr;
}

const Hashira::CameraInfo& Hashira::Camera::GetCameraInfo() const
{
	return _cameraInfo;
}

const D3D12_VIEWPORT* Hashira::Camera::GetViewport(Uint32 index)
{
	return _viewports.data();
}

const Hashira::Uint32 Hashira::Camera::GetViewportNum()
{
	return static_cast<Uint32>(_viewports.size());
}

std::vector<D3D12_VIEWPORT>& Hashira::Camera::GetViewportArray()
{
	return _viewports;

}

const D3D12_RECT* Hashira::Camera::GetScissor()
{
	return &_scissorRect;
}

void Hashira::Camera::ChangeFov(float fov)
{
	CameraInitInfo info{};
	info = &this->_cameraInfo;
	info.fov = fov;
	ChangeParameter(info, false);

}

void Hashira::Camera::ChangeNearFarClip(const Vector2& clipRange)
{
	CameraInitInfo info{};
	info = &this->_cameraInfo;

	info.nearClip = clipRange.x;
	info.farClip = clipRange.y;

	ChangeParameter(info, false);
}

void Hashira::Camera::ChangeParameter(const CameraInitInfo& cameraInfo, bool changeTargetPos)
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

void Hashira::Camera::AddViewport(const D3D12_VIEWPORT& viewport)
{
	this->_viewports.push_back(viewport);
}

void Hashira::Camera::SetScissorRect(const D3D12_RECT& rect)
{
	this->_scissorRect = rect;
}

Hashira::DescriptorInfo& Hashira::Camera::GetCurrentDSVDescriptor()
{
	return _cameraDsvDescriptors.Get();
}

Hashira::DescriptorInfo& Hashira::Camera::GetCameraCBVDescriptor()
{
	return _cameraCbvDescriptor;
}

std::unique_ptr<Hashira::Buffer>& Hashira::Camera::GetCurrentDepthStencilBuffer()
{
	return  _depthStencils.Get();
}

Hashira::MultipleUniquePtrBuffer<Hashira::Buffer, Hashira::CameraDepthStencilMax>& Hashira::Camera::GetDepthStencilBuffers()
{
	return _depthStencils;
}

Hashira::MultipleBuffer<Hashira::DescriptorInfo, Hashira::CameraDepthStencilMax>& Hashira::Camera::GetDsvDescriptors()
{
	return _cameraDsvDescriptors;
}

void Hashira::Camera::ClearCurrentDepthStencil(std::shared_ptr<CommandList>& list)
{

	list->ClearDepthStencilView(this->_cameraDsvDescriptors.Get().cpuHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0u, 0, nullptr);

}

void Hashira::Camera::Update()
{
	this->_transformBuffer.Alignment256ByteUpdate(&this->_cameraInfo, sizeof(CameraInfo));
}

void Hashira::Camera::FlipDepthStencilBuffer()
{

	this->_cameraDsvDescriptors.FlipBuffer();
	this->_depthStencils.FlipBuffer();
}

void Hashira::Camera::Discard()
{
	this->_depthStencils.DiscardBuffers();
	for (int i = 0; i < CameraDepthStencilMax; ++i)
	{

		auto& desc = this->_cameraDsvDescriptors.Get();
		desc.Free();
		_cameraDsvDescriptors.FlipBuffer();
	}

	_cameraCbvDescriptor.Free();
}

HRESULT Hashira::Camera::InitializeDepthStencil(std::shared_ptr<D3D12Device>& dev, DXGI_FORMAT depthFormat, DXGI_FORMAT clearFormat)
{


	DepthStencil* stencil = nullptr;
	for (Uint32 i = 0u; i < CameraDepthStencilMax; ++i)
	{
		stencil = new DepthStencil();
		auto hr = stencil->Initialize(dev, static_cast<Uint32>(_cameraInfo.windowWidth), static_cast<Uint32>(_cameraInfo.windowHeight),
			depthFormat, clearFormat);
		std::unique_ptr<Buffer> src;
		src.reset(stencil);
		if (FAILED(hr))
		{
			return hr;
		}

		this->_depthStencils.SetAddressOf(i, std::move(src));

	}
	return S_OK;
}

//HRESULT Hashira::Camera::InitializeRenderTarget(std::shared_ptr<D3D12Device>& dev, DXGI_FORMAT rtFormat, Vector4& clearColor)
//{
//
//	D3D12_HEAP_PROPERTIES props = {};
//
//	props.Type = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT;
//	props.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
//	props.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
//	props.CreationNodeMask = 0;
//	props.VisibleNodeMask = 0;
//
//	D3D12_RESOURCE_DESC resDesc = {};
//	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
//	resDesc.Width = _cameraInfo.windowWidth;
//	resDesc.Height = _cameraInfo.windowHeight;
//	resDesc.DepthOrArraySize = 1;
//	resDesc.MipLevels = 1;
//	resDesc.Format = rtFormat;
//	resDesc.Layout = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_UNKNOWN;
//	resDesc.SampleDesc.Count = 1;
//	resDesc.SampleDesc.Quality = 0;
//	resDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET |
//		D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
//	D3D12_CLEAR_VALUE clearValue;
//	clearValue.Format = rtFormat;
//	clearValue.DepthStencil.Depth = 0.0f;
//	clearValue.DepthStencil.Stencil = 0;
//	clearValue.Color[0] = clearColor.x;
//	clearValue.Color[1] = clearColor.y;
//	clearValue.Color[2] = clearColor.z;
//	clearValue.Color[3] = clearColor.w;
//
//	Buffer* rt = new Buffer();
//
//	auto hr = rt->Initialize(dev, props, D3D12_HEAP_FLAG_NONE, resDesc,
//		D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET,
//		&clearValue);
//
//	if (FAILED(hr))
//	{
//		return hr;
//	}
//	this->_renderTarget.reset(rt);
//	return hr;
//}

Hashira::CameraInfo& Hashira::CameraInfo::operator=(const CameraInitInfo& val)
{
	nearClip = val.nearClip;
	farClip = val.farClip;
	fov = val.fov;
	windowWidth = val.width;
	windowHeight = val.height;
	view = (Matrix::CreateLookAt(val.position, val.target, val.up));
	view.Invert();
	projection = Matrix::CreatePerspectiveFOV(
		val.fov, (val.height / val.width), val.nearClip, val.farClip);
	invProj = Matrix::Invert(projection);
	invView = Matrix::Invert(view);

	return *this;
}

Hashira::CameraInitInfo& Hashira::CameraInitInfo::operator=(CameraInfo* cInfo)
{


	CameraInitInfo info{};
	info.nearClip = cInfo->nearClip;
	info.farClip = cInfo->farClip;
	info.fov = cInfo->fov;
	info.width = cInfo->windowWidth;
	info.height = cInfo->windowHeight;

	Matrix v = cInfo->view;
	info.position = v.axisW;
	info.up = Vector3(v.axisX.y, v.axisY.y, v.axisZ.y);
	info.target = (info.position + Vector3(v.axisX.z, v.axisY.z, v.axisZ.z));
	return *this;


}
