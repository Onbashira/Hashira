#pragma once
#include "Engine/Source/DescriptorHeap/DescriptorHeap.h"
#include "Engine/Source/DescriptorHeap/Descriptor.h"
#include "Engine/Source/MemoryAllocator/VariableSizeAllocationManager.h"
namespace Hashira {

	class Resource;
	class D3D12Device;
	class CommandList;

	class GameHeap
	{
	public:

		enum class HeapType {
			CPU, RTV, DSV, SAMP, EMPTY
		};

		using DescHeapMap = std::map<Hashira::GameHeap::HeapType, std::shared_ptr<DescriptorHeap>>;

		using ViewMap = std::map<Hashira::GameHeap::HeapType, std::vector<Descriptor::ViewType>>;

		using DescMap = std::map<Hashira::GameHeap::HeapType, std::vector<std::shared_ptr<Descriptor>>>;
		


		struct FreeBlocksInfo {

			int head = -1;
			int tail = -1;


		};

		struct GameHeapDesc {

			unsigned int maxCPUHeapSize;
			unsigned int maxRTHeapSize;
			unsigned int maxDSHeapSize;
			unsigned int maxSampHeapSize;

			GameHeapDesc() : maxCPUHeapSize(4096), maxRTHeapSize(32), maxDSHeapSize(32), maxSampHeapSize(32){};
			GameHeapDesc(unsigned int maxCPUDescNum,unsigned int maxRTDescNum,
				unsigned int maxDSDescNum,unsigned int maxSampDescNum) : 
				maxCPUHeapSize(maxCPUDescNum),maxRTHeapSize(maxRTDescNum), 
				maxDSHeapSize(maxDSDescNum), maxSampHeapSize(maxSampDescNum) {};

			~GameHeapDesc() {};

		};


	private:

		//�f�o�C�X�̎Q��
		std::shared_ptr<D3D12Device> _device;

		//�q�[�v�^�C�v���L�[�l�Ƃ����f�X�N���v�^�q�[�v
		DescHeapMap _heaps;

		//�e�q�[�v�^�C�v�Ƀo�C���h����Ă���view�̃^�C�v
		ViewMap _allocatedViewMap;

		DescMap _allocatedDescMap;

		//cpuHeap�Ƀo�C���h���ꂽ�f�X�N���v�^�̃I�t�Z�b�g�@�󂫃u���b�N�����R�X�g�̍팸�p
		unsigned int _cpuOffset;
		//rtvHeap�Ƀo�C���h���ꂽ�f�X�N���v�^�̃I�t�Z�b�g�@�󂫃u���b�N�����R�X�g�̍팸�p
		unsigned int _rtvOffset;
		//dsvHeap�Ƀo�C���h���ꂽ�f�X�N���v�^�̃I�t�Z�b�g�@�󂫃u���b�N�����R�X�g�̍팸�p
		unsigned int _dsvOffset;
		//sampelrHeap�Ƀo�C���h���ꂽ�f�X�N���v�^�̃I�t�Z�b�g�@�󂫃u���b�N�����R�X�g�̍팸�p
		unsigned int _samplerOffset;

		//�e�~���[�e�b�N�X
		std::mutex _cpuMutex;
		std::mutex _rtvMutex;
		std::mutex _dsvMutex;
		std::mutex _samplerMutex;

		static std::shared_ptr<Hashira::Descriptor> InvalidDescriptor;

	public:

		GameHeap(std::shared_ptr<D3D12Device>& device,GameHeapDesc& desc);

		~GameHeap();

		static std::shared_ptr<GameHeap> CreateGameHeap(std::shared_ptr<D3D12Device>& device ,GameHeapDesc* desc);

		HRESULT ReInitialize(GameHeapDesc* desc);

		HRESULT ReCreateHeap(const GameHeap::HeapType& heapType, unsigned int maxHeapSize);

		std::weak_ptr<Hashira::Descriptor> GetDescriptorHandle(const Hashira::GameHeap::HeapType& heapType, unsigned int handleOffset);

		//����Ƃ��Ă͊�{�I�ɏ㏑��
		std::weak_ptr<Hashira::Descriptor> CreateView(Hashira::GameHeap::HeapType heapType,Hashira::Descriptor::ViewType viewType, void* viewDesc, unsigned int handleOffset, Resource* resource, Resource* counterResource = nullptr);

		//�ȉ��̊֐��͊�{�I�ɒP��f�X�N���v�^�̐���
		std::weak_ptr<Hashira::Descriptor> CreateCBView(D3D12_CONSTANT_BUFFER_VIEW_DESC& desc);

		std::weak_ptr<Hashira::Descriptor> CreateSRView(D3D12_SHADER_RESOURCE_VIEW_DESC& desc, Resource* resource);

		std::weak_ptr<Hashira::Descriptor> CreateUAView(D3D12_UNORDERED_ACCESS_VIEW_DESC& desc, Resource* resource, Resource* counterResource = nullptr);

		std::weak_ptr<Hashira::Descriptor> CreateRTView(D3D12_RENDER_TARGET_VIEW_DESC& desc, Resource* resource);

		std::weak_ptr<Hashira::Descriptor> CreateDSView(D3D12_DEPTH_STENCIL_VIEW_DESC& desc, Resource* resource);

		std::weak_ptr<Hashira::Descriptor> CreateSampView(D3D12_SAMPLER_DESC& desc);

		void SetGameHeap(std::shared_ptr<Hashira::CommandList> list);

		void Discard();

	private:



	};

}