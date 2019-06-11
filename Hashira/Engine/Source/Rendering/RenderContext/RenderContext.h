#pragma once
namespace Hashira {

	class CommandList;
	class CommandAllocator;
	class Fence;
	class CommandQueue;
	class D3D12Device;
	class Resource;
	class SwapChain;
	class RenderingManager;

	// 各シーン毎のレンダーコンテキスト
	//このクラスはスクリーンクリア用、スクリーンフリップ用、シーンのコマンドリストアロケータ
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

		std::vector<std::array<std::shared_ptr<CommandList>, 2>> _cmdLists;
		
		std::mutex _allocatorMutex;

		std::vector<std::shared_ptr<CommandAllocator>> _cmdAllocators;

		std::vector<Fence> _fences;

		std::vector<std::vector<std::shared_ptr<CommandList>>> _listsVector;

		std::shared_ptr<CommandQueue> _queueRef;

		std::shared_ptr<SwapChain> _swapChain;

		std::function<bool(INT64,INT64,bool)> _flushFunc;

	public:

		virtual ~RenderContext();

		HRESULT Initialize(std::shared_ptr<D3D12Device>& device,int frameNum, int nodeMask,std::shared_ptr<CommandQueue>& queue,std::shared_ptr<SwapChain>& swapChain);

		HRESULT CreateCommandList(std::shared_ptr<D3D12Device>& device,D3D12_COMMAND_LIST_TYPE type, std::shared_ptr<CommandList>& commandList);

		int GetCurrentIndex();

		int Flip();

		void ClearCmdLists();

		std::weak_ptr<Hashira::CommandList> GetResourceUpdateCmdList(RC_COMMAND_LIST_TYPE listType);
		
		std::weak_ptr<Hashira::CommandAllocator> GetCurrentCmdAllocator();

		Hashira::Fence& GetCurrentFence();

		std::weak_ptr<CommandQueue> GetCommandQueue();

		std::shared_ptr<Hashira::SwapChain> GetSwapChain();

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

		RenderContext();

	};

}