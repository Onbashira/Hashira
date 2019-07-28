#pragma once
#include"Engine/Source/Utility/D3D12Common.h"
#include <string>

namespace Hashira {

	class D3D12Device;
	class CommandList;

	class Descriptor;
	//�q�[�v
	class DescriptorHeap
	{
	public:

	private:

		//�q�[�v�{��
		ID3D12DescriptorHeap* _heap;

		//�f�X�N���v�^���X�g
		Descriptor* _descriptors;

		//�g���Ă��Ȃ��f�X�N���v�^���X�g
		Descriptor* _unusedDescriptorsList;

		//�C�j�V�����C�Y�����ۂɐݒ肵���f�X�N
		D3D12_DESCRIPTOR_HEAP_DESC _heapDesc;

		//�C���N�������g�T�C�Y
		Uint32 _descriptorSize;

		//�ǂꂭ�炢�̎g�p�f�X�N���v�^���������Ă��邩
		Uint32 _takeNum;

	public:

		DescriptorHeap();

		~DescriptorHeap();

		//������
		HRESULT Initialize(std::shared_ptr<D3D12Device>& device, const D3D12_DESCRIPTOR_HEAP_DESC& desc);

		//�������ʒu�ɉ����r���[���ݒ肳��ĂȂ���Ԃ̃f�X�N���v�^�̕ԋp
		Descriptor* CreateDescriptor();

		//�f�X�N���v�^�P�ʂł̉��
		void ReleaseDescriptor(Descriptor* p);

		//Getter
		ID3D12DescriptorHeap* GetHeap();

		//�S�f�[�^�j��
		void Discard();

	private:

	};

	class DescriptorAllocator;

	//�f�X�N���v�^���
	struct DescriptorInfo
	{

		DescriptorAllocator* allocator = nullptr;
		D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle;
		D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle;
		Uint32 index = Uint32(-1);

		bool IsValid() {
			return allocator == nullptr;
		}

		//���
		void Free();

	};

	//�f�X�N���v�^�A���P�[�^
	class DescriptorAllocator
	{
	public:

	private:
		//�r���I����p��Mutex
		std::mutex _mutex;
		//�q�[�v�{��
		ID3D12DescriptorHeap* _heap;
		//�q�[�v�f�X�N
		D3D12_DESCRIPTOR_HEAP_DESC _heapDesc;
		//�����ʒu
		D3D12_CPU_DESCRIPTOR_HANDLE _cpuHandleStart;
		D3D12_GPU_DESCRIPTOR_HANDLE _gpuHandleStart;
		//�g�p�t���O
		Uint8* _usedFlags;
		//�f�X�N���v�^�C���N�������g�T�C�Y
		Uint32 _descSize;
		//�A���P�[�g�����T�C�Y
		Uint32 _allocateCount;
		//���݂̃V�[�N�|�W�V����
		Uint32 _currentSeekPos;

	public:
		DescriptorAllocator();

		~DescriptorAllocator();
		
		//�C�j�V�����C�Y
		HRESULT Initialize(std::shared_ptr<D3D12Device>& device, D3D12_DESCRIPTOR_HEAP_DESC& desc);
		
		//�j��
		void Discard();

		//�A���P�[�V��������
		DescriptorInfo Allocate();

		void FreeDescriptor(DescriptorInfo info);


	private:
	};

	class GlobalDescriptorHeap;

	//�X�^�b�N
	class DescriptorStack
	{
		friend class GlobalDescriptorHeap;

	public:

	private:
		//�q�[�v�̊e�A�h���X�̃X�^�[�g�ʒu
		D3D12_CPU_DESCRIPTOR_HANDLE _cpuHandleStartPos;
		D3D12_GPU_DESCRIPTOR_HANDLE _gpuHandleStartPos;
		//�f�X�N�T�C�Y
		Uint32 _descSize;
		//�X�^�b�N�ő吔
		Uint32 _stackMax;
		//�X�^�b�N�ʒu
		Uint32 _stackPosition;

	public:

		DescriptorStack();

		~DescriptorStack();

		//�X�^�b�N�̊m��
		bool Allocate(Uint32 count, D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle, D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle);

		void Reset()
		{
			_stackPosition = 0;
		}

	private:

	};

	//�X�^�b�N���X�g
	class DescriptorStackList
	{
	public:

	private:
		//�e�O���[�o���q�[�v
		GlobalDescriptorHeap* _parentHeap;
		
		//�X�^�b�N�z��
		std::vector<DescriptorStack> _stacks;
		//�X�^�b�N�̃C���f�b�N�X
		Uint32 _stackIndex;
	public:

		DescriptorStackList();

		~DescriptorStackList();

		//������
		bool Initialize(GlobalDescriptorHeap* parentHeap);
		//�J�E���g�����̃������������f�X�N���v�^�X�^�b�N���A���P�[�g����
		void Allocate(Uint32 count, D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle, D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle);

		void Reset();

	private:

		bool AddStack();

	};

	//���q�[�v
	class GlobalDescriptorHeap
	{
	public:

	private:
		//�r������pMutex
		std::mutex _mutex;
		//�q�[�v
		ID3D12DescriptorHeap* _heap;
		//�쐬���Ɏg�p�����f�X�N���v�V����
		D3D12_DESCRIPTOR_HEAP_DESC _heapDesc;
		D3D12_CPU_DESCRIPTOR_HANDLE _cpuHandleStartPos;
		D3D12_GPU_DESCRIPTOR_HANDLE _gpuHandleStartPos;
		Uint32 _descSize;
		//���݂̃A���P�[�g�����f�X�N���v�^��
		Uint32 _allocateCount;

	public:

		//�q�[�v������
		bool Initialize(std::shared_ptr<D3D12Device>& dev, const D3D12_DESCRIPTOR_HEAP_DESC& desc);

		//�X�^�b�N��Count*DescSize�̃����������蓖�Ă�
		bool AllocateStack(DescriptorStack& stack, Uint32 count);
		//�q�[�v�擾
		ID3D12DescriptorHeap* GetHeap()
		{
			return _heap;
		};

		void Discard();

	private:

	};

	//�T���v���p�q�[�v
	class SamplerDescriptorHeap
	{
	public:

	private:
		//�q�[�v
		ID3D12DescriptorHeap* _heap;
		D3D12_CPU_DESCRIPTOR_HANDLE _cpuHandleStartPos;
		D3D12_GPU_DESCRIPTOR_HANDLE _gpuHandleStartPos;
		//�f�X�N�T�C�Y
		Uint32 _descSize;
		//�A���P�[�g��
		Uint32 _allocateCount;

	public:

		//�q�[�v�C�j�V�����C�Y
		bool Initialize(std::shared_ptr<D3D12Device>& device);

		//�J�E���g���A���P�[�g
		bool Allocate(Uint32 count, D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle, D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle);

		ID3D12DescriptorHeap* GetHeap()
		{
			return _heap;
		}

		void Discard();
	private:

	};

	//�T���v���[�L���b�V��
	class SamplerDescriptorCache
	{

	public:

	private:
		//�}�b�s���O�p
		struct MapItem
		{
			//�T���v���[�q�[�v
			SamplerDescriptorHeap* heap;
			D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle;
			D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle;
		};
		//�e�f�o�C�X
		std::shared_ptr<D3D12Device> _device;
		//�q�[�v���X�g
		std::vector<std::unique_ptr<SamplerDescriptorHeap>> _heapList;
		//�O�̃q�[�v
		SamplerDescriptorHeap* _last;
		//���݂̃q�[�v
		SamplerDescriptorHeap* _current;
		//�L���b�V���}�b�v
		std::map<Uint32, MapItem> _descCache;

	public:

		bool Initialize(std::shared_ptr<D3D12Device>& dev);

		bool AllocateAndCopy(Uint32 count, D3D12_CPU_DESCRIPTOR_HANDLE* cpuHandle, D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle);

		ID3D12DescriptorHeap* GetHeap()
		{
			assert( _last != nullptr);
			return _last->GetHeap();
		};

		void Discard();

	private:

		bool AddHeap();

	};

	//DXR�q�[�v�C�j�V�����C�Y�\����
	struct DXRDescriptorHeapInitializeInfo {
		Uint32 bufferCount;
		Uint32 asCount;
		Uint32 globalCBVCount;
		Uint32 globalSRVCount;
		Uint32 globalUAVCount;
		Uint32 globalSamplerCount;
		Uint32 materialCount;
	};

	//DXR�p�q�[�v
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
			return _info.globalCBVCount;
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
			return _info.globalCBVCount + _info.globalSRVCount + _info.globalUAVCount;
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

	//DXR�p�q�[�v�}�l�[�W��
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



}
