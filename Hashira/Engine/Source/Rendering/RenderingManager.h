#pragma once
#include "Engine/Source/Utility/D3D12Common.h"
#include "Engine/Source/Utility/NonCopyable.h"
#include "Engine/Source/Rendering/SwapChain.h"
namespace Hashira {

	class CommandQueue;
	class Factory;
	class Window;
	class RenderContext;
	class RenderingPassHolder;
	class RenderingDevice;
	class Scene;
	class DescriptorHeap;

	class RenderingManager : private NonCopyable
	{

		friend class Framework;

	public:

	private:

		std::shared_ptr<SwapChain> _swapChain;

		std::shared_ptr<CommandQueue> _masterQueue;

		std::shared_ptr<RenderingDevice> _renderingDevice;

	public:

		~RenderingManager();

		HRESULT Initialize(std::shared_ptr<D3D12Device>& device,Factory* factory, Window* window, UINT windowWidth, UINT windowHeight, unsigned int bufferNum = 2);

		void ClearScreen(std::weak_ptr<CommandList> list);

		void SetMainRenderTarget(std::shared_ptr<CommandList>& list, D3D12_CPU_DESCRIPTOR_HANDLE* depthHandle = nullptr);

		void FlipScreen();

		void CopyToRenderTarget(std::weak_ptr<CommandList> list, Resource* src);

		void SetStatePresentRT(std::shared_ptr<CommandList> list);

		void Present(unsigned int sysncInterval, unsigned int flags);

		std::vector<std::shared_ptr<Hashira::Resource>> GetDisplayRenderTargets();

		std::shared_ptr<CommandQueue> GetQueue();

		std::shared_ptr<RenderingDevice>& GetRenderingDevice();

		std::shared_ptr<Hashira::RenderContext> CreateRenderContext();

		unsigned int GetBackBufferNum();

		void Term();

	private:

		RenderingManager() {};

		RenderingManager(const RenderingManager&) {};

		RenderingManager(RenderingManager&&) {};

		RenderingManager& operator =(const RenderingManager&) { return *this; };

		RenderingManager& operator =(RenderingManager&&) { return *this; };

	};
}
