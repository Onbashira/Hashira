#pragma once

#include "Engine/Source/Component/GameObject/GameObject.h"
#include "Engine/Source/DescriptorHeap/DescriptorHeap.h"

namespace Hashira
{

	class D3D12Device;
	class CommandList;
	class RenderContext;

	enum CAMERA_TYPE
	{
		Perspective,
		Orthogonal
	};

	struct CameraInfo;

	struct CameraInitInfo
	{
		float nearClip;
		float farClip;
		float fov;
		float width;
		float height;
		Vector3 position;
		Vector3 target;
		Vector3 up;

		CameraInitInfo& operator = (CameraInfo* cInfo);
	};

	struct CameraInfo
	{
		Matrix view;
		Matrix projection;
		Matrix invView;
		Matrix invProj;
		float  windowWidth;
		float  windowHeight;
		float  nearClip;
		float  farClip;
		float  fov;

		CameraInfo& operator = (const CameraInitInfo& val)
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
		}
	};

	CameraInitInfo& Hashira::CameraInitInfo::operator=(CameraInfo* cInfo)
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
		info.target = (info.position + Vector3(v.axisX.z,v.axisY.z,v.axisZ.z));
	}

	class ICamera : GameObject
	{
	public:

	protected:
		CameraInfo _cameraInfo;
		
		D3D12_RECT _scissorRect;
		CAMERA_TYPE _cameraType;
		std::unique_ptr<Buffer> _depthStencil;
		std::unique_ptr<Buffer> _renderTarget;
		std::vector<D3D12_VIEWPORT> _viewports;
		std::vector<DescriptorInfo> _descInfo;
		inline static Uint32 size = sizeof(CameraInfo);

	private:
	
	public:
		ICamera();
		virtual ~ICamera();

		virtual HRESULT Initialize(std::shared_ptr<RenderContext> & context , const CameraInitInfo& cameraInfo);
		virtual const CameraInfo& GetCameraInfo()const;
		const D3D12_VIEWPORT* GetViewport(Uint32 index = 0);
		const Uint32 GetViewportNum();
		std::vector<D3D12_VIEWPORT>& GetViewportArray();
		const D3D12_RECT* GetScissor();
		void ChangeFov(float fov);
		void ChangeNearFarClip(const Vector2& clipRange);
		void ChangeParameter(const CameraInitInfo& cameraInfo , bool chanegTargetPos = false);
		void AddViewport(const D3D12_VIEWPORT& viewport);
		void SetScissorRect(const D3D12_RECT& rect);
		DescriptorInfo* GetDescriptor();
		DescriptorInfo& GetDSVDescriptor();
		DescriptorInfo& GetRTVDescriptor();
		DescriptorInfo& GetCameraCBVDescriptor();
		virtual void Update();
		
		void Discard();
	protected:

		void InitializeDepthStencil(std::shared_ptr<D3D12Device>& dev, DXGI_FORMAT depthFormat , DXGI_FORMAT clearFormat);
		void InitializeRenderTarget(std::shared_ptr<D3D12Device>& dev, DXGI_FORMAT depthFormat, DXGI_FORMAT clearFormat, float* clearColor);

	private:

		//hide
		virtual void Draw(std::shared_ptr<CommandList>& cmdList)override {};

	

	};

}