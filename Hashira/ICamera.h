#pragma once

#include "Engine/Source/Component/GameObject/GameObject.h"


namespace Hashira
{

	class D3D12Device;
	class CommandList;
	class DescriptorHeap;
	class DescriptorAllocator;
	class DescriptorInfo;
	class RenderingContext;
	class DescriptorInfo;

	enum CAMERA_TYPE
	{
		Perspective,
		Orthogonal
	};
	struct CameraInfo
	{
		Matrix view;
		Matrix projection;
		Matrix invView;
		Matrix invViewProj;
		float  windowWidth;
		float  windowHeight;
	};

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
	};

	class ICamera : GameObject
	{
	public:

	protected:
		CameraInfo _cameraInfo;
		D3D12_RECT _scissorRect;
		CAMERA_TYPE _cameraType;
		std::unique_ptr<Buffer> _depthStencill;
		std::unique_ptr<Buffer> _renderTarget;
		std::unique_ptr<Buffer> _cameraBuffer;
		std::vector<D3D12_VIEWPORT> _viewports;

	private:
	
	public:
		ICamera();
		virtual ~ICamera();

		virtual HRESULT Initialize(std::shared_ptr<RenderingContext> & context , const CameraInitInfo& cameraInfo) = 0;
		virtual const CameraInfo& GetCameraInfo()const;
		const D3D12_VIEWPORT* GetViewport(Uint32 index = 0);
		const Uint32 GetViewportNum();
		std::vector<D3D12_VIEWPORT>& GetViewportArray();
		const D3D12_RECT* GetScissor();
		void ChangeFov(float fov);
		void ChangeNearFarClip(const Vector2& clipRange);
		void AddViewport(const D3D12_VIEWPORT& viewport);
		void SetScissorRect(const D3D12_RECT& rect);
		DescriptorInfo* GetDescriptor();
		virtual void Update() = 0;
		
		void Discard();



	protected:
		virtual HRESULT InitializeTransformBuffer(std::shared_ptr<D3D12Device>& device, size_t size);

	private:

		//hide
		virtual void Draw(std::shared_ptr<CommandList>& cmdList)override {};

	};

}