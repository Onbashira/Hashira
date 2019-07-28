#pragma once
#include "Engine/Source/Buffer/UploadBuffer.h"
#include "Engine/Source/DescriptorHeap/DescriptorHeap.h"

namespace Hashira {

	class RenderContext;
	class RenderingDevice;
	class RenderingPassHolder;
	class Camera;
	class ConstantBuffer;

	struct SceneConstant
	{
		Vector2 resolution;
		float time;
	};

	//シーンを構成する基底クラス
	class Scene
	{

		friend class RenderingManager;
		
	public:

	protected:

		std::shared_ptr<RenderingDevice>& _renderingDevice;

		std::shared_ptr<RenderContext> _renderContext;

		std::shared_ptr<Camera> _mainCamera;

		UploadBuffer<SceneConstant> _sceneConstant;

		DescriptorInfo _sceneConstantDescriptor;


	private:
	public:
		
		Scene(Uint32 viewDescMaxNum, Uint32 dsvDescMaxNum, Uint32 rtvDescMaxNum, Uint32 samplerMaxNum);
		
		~Scene();

		virtual void SceneBegin();

		virtual void Update() = 0;

		virtual void Rendering() = 0;

		virtual void ExecutePath();

		virtual void SceneEnd();

		virtual void Discard();

	private:

	};

}