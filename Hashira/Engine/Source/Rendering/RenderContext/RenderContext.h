#pragma once
#include "Engine/Source/DescriptorHeap/DescriptorHeap.h"

namespace Hashira {

	class CommandList;
	class CommandAllocator;
	class Fence;
	class CommandQueue;
	class RenderingDevice;
	class Buffer;
	class SwapChain;
	class RenderingManager;

	class DescriptorAllocator;
	class GlobalDescriptorHeap;
	class Descriptor;
	struct DescriptorInfo;



	// 各シーン毎のレンダーコンテキスト
	//一時
	class RenderContext
	{


	public:

		friend class RenderingManager;

		enum class RC_COMMAND_LIST_TYPE
		{
			BEGIN = 0,
			END = 1
		};
	private:

		unsigned int _frameNum;

		unsigned int _currentIndex;

		int _node;
		
		UINT64 _currentFence;
		
		bool _isDiscarded;

		std::shared_ptr<RenderingDevice> _parentDevice;

		std::vector<std::array<std::shared_ptr<CommandList>, 2>> _cmdLists;
		
		std::mutex _allocatorMutex;

		std::vector<std::shared_ptr<CommandAllocator>> _cmdAllocators;

		std::vector<Fence> _fences;

		std::vector<std::vector<std::shared_ptr<CommandList>>> _listsVector;

		std::shared_ptr<CommandQueue> _queueRef;

		std::shared_ptr<SwapChain> _swapChain;

		std::function<bool(INT64,INT64,bool)> _flushFunc;

		std::unique_ptr<GlobalDescriptorHeap> _globalDescriptorHeap;
		std::unique_ptr<DescriptorAllocator> _viewHeapAllocator;
		std::unique_ptr<DescriptorAllocator> _samplerHeapAllocator;
		std::unique_ptr<DescriptorAllocator> _rtvHeapAllocator;
		std::unique_ptr<DescriptorAllocator> _dsvHeapAllocator;
		std::shared_ptr<DescriptorStackList> _renderDescStackList;
		DescriptorInfo _defaultViewDescriptorInfo;
		DescriptorInfo _defaultSamplerDescriptorInfo;


	public:

		virtual ~RenderContext();

		HRESULT Initialize(Uint32 viewDescMaxNum, Uint32 dsvDescMaxNum, Uint32 rtvDescMaxNum, Uint32 samplerMaxNum);

		HRESULT IntializeAllocators(std::shared_ptr<RenderingDevice>& device, Uint32 viewDescMaxNum = 65535u, Uint32 dsvDescMaxNum =  128u, Uint32 rtvDescMaxNum = 256u, Uint32 samplerMaxNum = 2048u);

		HRESULT CreateCommandList(std::shared_ptr<D3D12Device>& device,D3D12_COMMAND_LIST_TYPE type, std::shared_ptr<CommandList>& commandList);
		HRESULT CreateCommandList(D3D12_COMMAND_LIST_TYPE type, std::shared_ptr<CommandList>& commandList);

		int GetCurrentIndex();

		int Flip();

		void ClearCmdLists();

		std::weak_ptr<Hashira::CommandList> GetResourceUpdateCmdList(RC_COMMAND_LIST_TYPE listType);
		
		std::weak_ptr<Hashira::CommandAllocator> GetCurrentCmdAllocator();

		Hashira::Fence& GetCurrentFence();

		std::weak_ptr<CommandQueue> GetCommandQueue();
		std::shared_ptr<Hashira::SwapChain> GetSwapChain();
		std::shared_ptr<RenderingDevice>& GetRenderingDevice();
		std::unique_ptr<GlobalDescriptorHeap>& GetGlobalDescriptorHeap();
		std::unique_ptr<DescriptorAllocator>& GetViewDescriptorHeap();
		std::unique_ptr<DescriptorAllocator>& GetSamplerDescriptorHeap();
		std::unique_ptr<DescriptorAllocator>& GetRtvDescriptorHeap();
		std::unique_ptr<DescriptorAllocator>& GetDsvDescriptorHeap();
		std::shared_ptr<Hashira::DescriptorStackList> GetDescriptorStackList();

		DescriptorInfo& GetViewDescriptorHeapInfo();
		DescriptorInfo& GetSamplerDescriptorHeapInfo();

		void PushFrontCmdList(std::shared_ptr<CommandList> list);

		void PushBackCmdList(std::shared_ptr<CommandList> list);

		void ExecuteCmdList3DQueue();

		void ExecuteCmdListCopyQueue();

		void ExecuteCmdListComputeQueue();

		void WaitForGPU(std::shared_ptr<CommandQueue>& commandQueue,bool waitNow = false);

		void ResetAllocators();

		void ResetCurrentCommandAllocator();

		void ResetCommandList(std::shared_ptr<CommandList>& list);

		void Present(unsigned int syncValue, unsigned int flags = 0);

		void Discard();

	private:

		RenderContext()noexcept;

		void PreInitialize(std::shared_ptr<RenderingDevice>& device, int frameNum, int nodeMask, std::shared_ptr<CommandQueue>& queue, std::shared_ptr<SwapChain>& swapChain);

	};

}