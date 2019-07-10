#pragma once
namespace Hashira {

	class RenderContext;
	class RenderingDevice;
	class RenderingPassHolder;
	class GlobalDescriptorHeap;
	class Camera;

	//シーンを構成する基底クラス
	class Scene
	{

		friend class RenderingManager;
		
	public:

	protected:

		std::shared_ptr<RenderingDevice>& _renderingDevice;

		std::shared_ptr<RenderContext> _renderContext;

		std::shared_ptr<Camera> _mainCamera;
	private:
	public:
		
		Scene(std::shared_ptr<RenderingDevice>& renderingDevice);
		
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