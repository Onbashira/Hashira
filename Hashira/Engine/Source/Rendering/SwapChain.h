#pragma once
#include "Engine/Source/DescriptorHeap/DescriptorHeap.h"
#include <memory>
#include <vector>

namespace Hashira {

	class CommandList;
	class CommandQueue;
	class Factory;
	class Window;
	class D3D12Device;
	class RenderinDevice;
	class Resource;

	class SwapChain
	{

		friend class RenderingManager;

	public:

		inline static Uint32 FrameBufferMax = 3u;

	private:
		unsigned int _bufferNum;

		unsigned int _currentIndex;

		unsigned int _width;

		unsigned int _height;

		Microsoft::WRL::ComPtr<IDXGISwapChain4> _swapChain;

		std::vector<std::shared_ptr<Resource>> _rtResource;

		std::unique_ptr<DescriptorAllocator> _localRtHeap;

		std::vector<DescriptorInfo> _allocatedDesc;

	public:

		SwapChain();

		~SwapChain();

		HRESULT Initialize(CommandQueue& commandQueue, std::shared_ptr<D3D12Device>& device, Factory& factory, Window & window, UINT windowWidth, UINT windowHeight, unsigned int bufferNum = 3, DXGI_FORMAT targetFormat = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM);

		unsigned int GetBufferNum();

		unsigned int GetCurrentBuckBuffer();

		std::unique_ptr<DescriptorAllocator>& GetLocalRtDescriptortAlolocator();

		HRESULT SetStatePresent(std::shared_ptr<CommandList> list);

		HRESULT SetStateRenderTarget(std::shared_ptr<CommandList> list);

		HRESULT	SetStateCopyDest(std::shared_ptr<CommandList> list);

		HRESULT	SetStateGenericRead(std::shared_ptr<CommandList> list);

		HRESULT	CopyToRenderTarget(std::shared_ptr<CommandList> list, Resource* pSrc);

		void SetRenderTarget(std::shared_ptr<CommandList> list, D3D12_CPU_DESCRIPTOR_HANDLE* depthHandle = nullptr);

		void ClearScreen(std::shared_ptr<CommandList> list);

		void ReSizeRenderTarget(std::shared_ptr<D3D12Device>& device, unsigned int width, unsigned int height, unsigned int backBufferNum);

		void FlipScreen();

		HRESULT Present(unsigned int sysncInterval = 1, unsigned int flags = 0);

		void Discard();

	private:

		HRESULT CreateSwapChain(CommandQueue& commandQueue, std::shared_ptr<D3D12Device>& device, Factory& factory, Window& window, UINT windowWidth, UINT windowHeight, unsigned int bufferNum, DXGI_FORMAT targetFormat);

		HRESULT CreateRenderTargets(std::shared_ptr<D3D12Device>& device, unsigned int bufferNum);

	};

};