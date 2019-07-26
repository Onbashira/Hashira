#pragma once

#include "Engine/Source/Component/GameObject/GameObject.h"
#include "Engine/Source/DescriptorHeap/DescriptorHeap.h"
#include "Engine/Source/Buffer/MultipleBuffer.h"
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
		float nearClip = 0.01f;
		float farClip = 1000.0f;
		float fov = 0.0f;
		float width = 1280.0f;
		float height = 720.0f;
		Vector3 position = Vector3(0.0f, 0.0f, -10.0f);
		Vector3 target = Vector3::zero;
		Vector3 up = Vector3::up;

		CameraInitInfo& operator = (CameraInfo* cInfo);
	};

	struct CameraInfo
	{
		Matrix view;
		Matrix projection;
		Matrix invView;
		Matrix invProj;
		float  windowWidth = 0.0f;
		float  windowHeight = 0.0f;
		float  nearClip = 0.01f;
		float  farClip = 1000.0f;
		float  fov = DegToRad(90);

		CameraInfo& operator = (const CameraInitInfo& val);

	};


	constexpr Uint32 CameraDepthStencilMax = 3u;

	class Camera : GameObject
	{
	public:


	protected:

		CameraInfo _cameraInfo;

		D3D12_RECT _scissorRect;
		CAMERA_TYPE _cameraType;
		DescriptorInfo _cameraCbvDescriptor;
		MultipleUniquePtrBuffer<Buffer, CameraDepthStencilMax> _depthStencils;
		MultipleBuffer<DescriptorInfo,CameraDepthStencilMax> _cameraDsvDescriptors;
		std::vector<D3D12_VIEWPORT> _viewports;

	private:

	public:
		Camera();
		virtual ~Camera();

		virtual HRESULT Initialize(std::shared_ptr<RenderContext>& context, const CameraInitInfo& cameraInfo);
		virtual const CameraInfo& GetCameraInfo()const;
		const D3D12_VIEWPORT* GetViewport(Uint32 index = 0);
		const Uint32 GetViewportNum();
		std::vector<D3D12_VIEWPORT>& GetViewportArray();
		const D3D12_RECT* GetScissor();
		void ChangeFov(float fov);
		void ChangeNearFarClip(const Vector2& clipRange);
		void ChangeParameter(const CameraInitInfo& cameraInfo, bool chanegTargetPos = false);
		void AddViewport(const D3D12_VIEWPORT& viewport);
		void SetScissorRect(const D3D12_RECT& rect);
		DescriptorInfo& GetCurrentDSVDescriptor();
		DescriptorInfo& GetCameraCBVDescriptor();
		std::unique_ptr<Buffer>& GetCurrentDepthStencilBuffer();
		MultipleUniquePtrBuffer<Buffer, CameraDepthStencilMax>& GetDepthStencilBuffers();
		MultipleBuffer<DescriptorInfo, CameraDepthStencilMax>& GetDsvDescriptors();
		void ClearCurrentDepthStencil(std::shared_ptr<CommandList>& list);
		virtual void Update();
		virtual void FlipDepthStencilBuffer();

		void Discard();
	protected:

		HRESULT InitializeDepthStencil(std::shared_ptr<D3D12Device>& dev, DXGI_FORMAT depthFormat, DXGI_FORMAT clearFormat);

	private:

		//hide
		virtual void Draw(std::shared_ptr<CommandList>& cmdList)override {};



	};

}