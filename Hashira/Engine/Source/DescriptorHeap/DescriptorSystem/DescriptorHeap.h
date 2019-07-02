#pragma once
#include"Engine/Source/Utility/D3D12Common.h"
#include <string>

namespace Hashira {

	class D3D12Device;
	class CommandList;

	constexpr Uint32 SamplerMaxCount = 2048u;
	constexpr Uint32 CbvMaxCount = 16;
	constexpr Uint32 SrvMaxCount = 32;
	constexpr Uint32 UavMaxCount = 16;

	namespace DescriptorSystem {

		class Descriptor;
		//ヒープ
		class DescriptorHeap
		{
		public:

		private:
			
			Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> _heap;

			Descriptor* _descriptors;

			Descriptor* _unusedDescriptorsList;

			D3D12_DESCRIPTOR_HEAP_DESC _heapDesc;

			Uint32 _descriptorSize;

			Uint32 _takeNum;

		public:

			DescriptorHeap();

			~DescriptorHeap();

			HRESULT Initialize(std::shared_ptr<D3D12Device>& device, const D3D12_DESCRIPTOR_HEAP_DESC& desc);

			Descriptor* CreateDescriptor();

			void ReleaseDescriptor(Descriptor* p);

			Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& GetHeap();

			void Discard();

		private:

		};

		class DescriptorAllocator;

		//デスクリプタ情報
		struct DescriptorInfo
		{

			DescriptorAllocator* allocator;
			D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle;
			D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle;
			Uint32 index;

			bool IsValid() {
				return allocator == nullptr;
			}

			void Free()
			{
				if (IsValid())
				{
					allocator->FreeDescriptor(*this);
					allocator = nullptr;
				}
			};

		};

		//デスクリプタアロケータ
		class DescriptorAllocator
		{
		public:

		private:
			std::mutex _mutex;
			ID3D12DescriptorHeap* _heap;
			D3D12_DESCRIPTOR_HEAP_DESC _heapDesc;
			D3D12_CPU_DESCRIPTOR_HANDLE _cpuHandleStart;
			D3D12_GPU_DESCRIPTOR_HANDLE _gpuHandleStart;
			Uint8* _usedFlags;
			Uint32 _descSize;
			Uint32 _allocateCount;
			Uint32 _currentSeekPos;

		public:
			DescriptorAllocator();
			
			~DescriptorAllocator();

			HRESULT Initialize(std::shared_ptr<D3D12Device>& device, D3D12_DESCRIPTOR_HEAP_DESC& desc);
			void Discard();

			DescriptorInfo Allocate();

			void FreeDescriptor(DescriptorInfo info);


		private:
		};

		class GlobalDescriptorHeap;

		//スタック
		class DescriptorStack 
		{
			friend class GlobalDescriptorHeap;

		public:

		private:
			D3D12_CPU_DESCRIPTOR_HANDLE _cpuHandleStartPos;
			D3D12_GPU_DESCRIPTOR_HANDLE _gpuHandleStartPos;
			Uint32 _descSize;
			Uint32 _stackMax;
			Uint32 _stackPosition;

		public:

			DescriptorStack();

			~DescriptorStack();

			bool Allocate(Uint32 count, D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle, D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle);

			void Reset()
			{
				_stackPosition = 0;
			}

		private:

		};

		//スタックリスト
		class DescriptorStackList
		{
		public:

		private:
			GlobalDescriptorHeap* _parentHeap;
			std::vector<DescriptorStack> _stacks;
			Uint32 _stackIndex;
		public:

			DescriptorStackList();

			~DescriptorStackList();

			bool Initialize(GlobalDescriptorHeap* parentHeap);

			void Allocate(Uint32 count, D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle, D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle);

			void Reset();

		private:

			bool AddStack();

		};

		//大域ヒープ
		class GlobalDescriptorHeap
		{
		public:

		private:
			std::mutex _mutex;
			ID3D12DescriptorHeap* _heap;
			D3D12_DESCRIPTOR_HEAP_DESC _heapDesc;
			D3D12_CPU_DESCRIPTOR_HANDLE _cpuHandleStartPos;
			D3D12_GPU_DESCRIPTOR_HANDLE _gpuHandleStartPos;
			Uint32 _descSize;
			Uint32 _allocateCount;

		public:

			bool Initialize(std::shared_ptr<D3D12Device>& dev, const D3D12_DESCRIPTOR_HEAP_DESC& desc);

			bool AllocateStack(DescriptorStack& stack, Uint32 count);

			ID3D12DescriptorHeap* GetHeap()
			{
				return _heap;
			};

			void Discard();

		private:

		};

		//サンプラ用ヒープ
		class SamplerDescriptorHeap
		{
		public:

		private:

			ID3D12DescriptorHeap* _heap;
			D3D12_CPU_DESCRIPTOR_HANDLE _cpuHandleStartPos;
			D3D12_GPU_DESCRIPTOR_HANDLE _gpuHandleStartPos;
			Uint32 _descSize;
			Uint32 _allocateCount;

		public:

			bool Initialize(std::shared_ptr<D3D12Device>& device);

			bool Allocate(Uint32 count, D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle, D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle);

			ID3D12DescriptorHeap* GetHeap()
			{
				return _heap;
			}

			void Discard();
		private:

		};

		//サンプラーキャッシュ
		class SamplerDescriptorCache
		{

		public:

		private:
			//マッピング用
			struct MapItem 
			{
				SamplerDescriptorHeap* heap;
				D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle;
				D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle;
			};

			std::shared_ptr<D3D12Device> _device;
			std::vector<std::unique_ptr<SamplerDescriptorHeap>> _heapList;
			SamplerDescriptorHeap* _last;
			SamplerDescriptorHeap* _current;
			std::map<Uint32, MapItem> _descCache;

		public:

			bool Initialize(std::shared_ptr<D3D12Device>& dev);

			bool AllocateAndCopy(Uint32 count, D3D12_CPU_DESCRIPTOR_HANDLE* cpuHandle, D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle);

			ID3D12DescriptorHeap* GetHeap()
			{
				assert(_last != nullptr);
				return _last->GetHeap();
			};

			void Discard();

		private:

			bool AddHeap();

		};

		//DXRヒープイニシャライズ構造体
		struct DXRDescriptorHeapInitializeInfo {
			Uint32 bufferCount;
			Uint32 asCount;
			Uint32 globalCBVCount;
			Uint32 globalSRVCount;
			Uint32 globalUAVCount;
			Uint32 globalSamplerCount;
			Uint32 materialCount;
		};

		//DXR用ヒープ
		class RaytracingDescriptorHeap
		{

		public:

		private:
			ID3D12DescriptorHeap* _viewHeap;
			ID3D12DescriptorHeap* _samplerHeap;

			D3D12_CPU_DESCRIPTOR_HANDLE _viewCpuHandleStart;
			D3D12_GPU_DESCRIPTOR_HANDLE _viewGpuHandleStart;
			D3D12_CPU_DESCRIPTOR_HANDLE _samplerCpuHandleStart;
			D3D12_GPU_DESCRIPTOR_HANDLE _samplerGpuHandleStart;

			Uint32 _viewDescMaxCount;
			Uint32 _samplerDescMaxCount;
			Uint32 _viewDescSize;
			Uint32 _samplerDescSize;

			DXRDescriptorHeapInitializeInfo _info;

			Uint32 _localCBVCount;
			Uint32 _localSRVCount;
			Uint32 _localUAVCount;
			Uint32 _localSamplerCount;

		public:
			RaytracingDescriptorHeap();
			~RaytracingDescriptorHeap();

			bool Initialize(std::shared_ptr<D3D12Device>& device, const DXRDescriptorHeapInitializeInfo& info);

			void GetGlobalViewHandleStart(Uint32 frameIndex, D3D12_CPU_DESCRIPTOR_HANDLE& cpu, D3D12_GPU_DESCRIPTOR_HANDLE& gpu);
			
			void GetGlobalSamplerHandleStart(Uint32 frameIndex, D3D12_CPU_DESCRIPTOR_HANDLE& cpu, D3D12_GPU_DESCRIPTOR_HANDLE& gpu);
			
			void GetLocalViewHandleStart(Uint32 frameIndex, D3D12_CPU_DESCRIPTOR_HANDLE& cpu, D3D12_GPU_DESCRIPTOR_HANDLE& gpu);
			
			void GetLocalSamplerHandleStart(Uint32 frameIndex, D3D12_CPU_DESCRIPTOR_HANDLE& cpu, D3D12_GPU_DESCRIPTOR_HANDLE& gpu);

			bool CanResizeMaterialCount(Uint32 materialCount);

			ID3D12DescriptorHeap* GetViewHeap() 
			{
				return _viewHeap;
			};
			ID3D12DescriptorHeap* GetSamplerHeap()
			{
				return _samplerHeap;
			};

			Uint32 GetViewDescSize()
			{	
				return _viewDescSize;
			};
			Uint32 GetSamplerDescSize()
			{
				return _samplerDescSize;
			};
			Uint32 GetBufferCount()
			{
				return _info.bufferCount;
			};
			Uint32 GetASCount() 
			{
				return _info.asCount;
			};

			Uint32 GetGlobalCBVCount()
			{
				return _info.globalCBVCount ;
			};
			Uint32 GetGlobalSRVCount()
			{
				return _info.globalSRVCount;
			};
			Uint32 GetGlobalUAVCount()
			{
				return _info.globalUAVCount;
			};
			Uint32 GetGlobalSamplerCount()
			{
				return _info.globalSamplerCount;
			};
			Uint32 GetGlobalViewCount()
			{
				return _info.globalCBVCount + _info.globalSRVCount+ _info.globalUAVCount;
			};

			Uint32 GetLocalCBVCount()
			{
				return _localCBVCount;
			};
			Uint32 GetLocalSRVCount()
			{
				return _localSRVCount;
			};
			Uint32 GetLocalUAVCount()
			{
				return _localUAVCount;
			};
			Uint32 GetLocalSamplerCount()
			{
				return _localSamplerCount;
			};
			Uint32 GetLocalViewCount()
			{
				return _localCBVCount + _localSRVCount + _localUAVCount;
			};

			void Discard();
		private:
		};

		struct DXRDescriptorManagerInitializeInfo
		{
			Uint32 renderCount = 0;
			Uint32 asCount = 0;
			Uint32 globalCBVCount = 0;
			Uint32 globalSRVCount = 0;
			Uint32 globalUAVCount = 0;
			Uint32 globalSamplerCount = 0;
			Uint32 materialCount = 0;
		};

		//DXR用ヒープマネージャ
		class RaytracingDescriptorManager
		{

		public:
			struct KillPendingHeap 
			{
				RaytracingDescriptorHeap* heap = nullptr;
				int killCount;
				
				KillPendingHeap(RaytracingDescriptorHeap* heap);

				bool Kill()
				{
					killCount--;
					if (killCount <= 0)
					{
						SafeDelete(heap);
						return true;
					}
					return false;

				};

				void ForceKill()
				{
					SafeDelete(heap);

				}
			};

			struct HandleStart 
			{
				D3D12_CPU_DESCRIPTOR_HANDLE viewCpuHandle;
				D3D12_GPU_DESCRIPTOR_HANDLE viewGpuHandle;
				D3D12_CPU_DESCRIPTOR_HANDLE samplerCpuHandle;
				D3D12_GPU_DESCRIPTOR_HANDLE samplerGpuHandle;

			};

		private:

			std::shared_ptr<D3D12Device> _device;
			RaytracingDescriptorHeap* _currentHeap;
			std::list<KillPendingHeap> _heapsBeforKill;
			Uint32 _globalIndex;
			Uint32 _localIndex;
		public:

			RaytracingDescriptorManager();
			~RaytracingDescriptorManager() {
				Discard();
			}

			bool Initialize(std::shared_ptr<D3D12Device>& device, DXRDescriptorManagerInitializeInfo info);

			void BeginNewFrame();

			bool ResizeMaterialCount(Uint32 count);

			void SetHeapToCommandList(std::shared_ptr<CommandList>& list);

			HandleStart InclementGlobalHandleStart();
			HandleStart InclementLocalHandleStart();

			Uint32 GetViewDescSize() { return _currentHeap->GetViewDescSize(); }
			Uint32 GetSamplerDescSize() { return _currentHeap->GetSamplerDescSize(); }

			void Discard();

		private:
		};

	} // namespace　DescriptorSystem


}
