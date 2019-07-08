#include "DescriptorHeap.h"
#include "Engine/Source/Debug/Logger.h"
#include "Engine/Source/Utility/Utility.h"
#include "Engine/Source/Device/D3D12Device.h"
#include "Engine/Source/DescriptorHeap/Descriptor.h"
#include "Engine/Source/CommandList/CommandList.h"
#include "Engine/Source/Rendering/SwapChain.h"

Hashira::DescriptorHeap::DescriptorHeap()
{
}

Hashira::DescriptorHeap::~DescriptorHeap()
{
	Discard();
}

HRESULT Hashira::DescriptorHeap::Initialize(std::shared_ptr<D3D12Device>& device, const D3D12_DESCRIPTOR_HEAP_DESC & desc)
{
	auto hr = device->GetDevice()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&_heap));
	if (FAILED(hr)) {
#ifdef _DEBUG
		HRESULT_LOG(hr);
#endif // DEBUG
		return hr;
	}

	//配列確保
	_descriptors = new Descriptor[desc.NumDescriptors + 2];
	//使用していないデスクリプタのリストに配列[1]を設定
	_unusedDescriptorsList = _descriptors + 1;
	//使用していないデスクリプタの前、次のポインタに配列[0]を設定
	_unusedDescriptorsList->_prev = _unusedDescriptorsList->_next = _unusedDescriptorsList;

	_heapDesc = desc;
	_descriptorSize = device->GetDevice()->GetDescriptorHandleIncrementSize(desc.Type);

	Descriptor* p = _unusedDescriptorsList +1;

	D3D12_CPU_DESCRIPTOR_HANDLE hCpu = _heap->GetCPUDescriptorHandleForHeapStart();
	D3D12_GPU_DESCRIPTOR_HANDLE hGpu = _heap->GetGPUDescriptorHandleForHeapStart();

	for (int i = 0; i < desc.NumDescriptors; i++,p++ ,hCpu.ptr += _descriptorSize , hGpu.ptr += _descriptorSize) {
		
		//親ヒープ及びハンドル、インデックスを設定
		p->_parentHeap = this;
		p->_cpuHandle = hCpu;
		p->_gpuHandle = hGpu;
		p->_index = i;

		//使っていないデスクリプタリストの次の要素を代入
		Descriptor* next = _unusedDescriptorsList->_next;
		//
		_unusedDescriptorsList->_next = next->_prev = p;

		p->_prev = _unusedDescriptorsList;
		p->_next = next;

	}

	_takeNum = 0;


	return hr;
}

Hashira::Descriptor * Hashira::DescriptorHeap::CreateDescriptor()
{

	if (_unusedDescriptorsList->_next = _unusedDescriptorsList)
	{
		return nullptr;

	}

	Descriptor* ret = _unusedDescriptorsList->_next;

	ret->_prev->_next = ret->_next;

	ret->_next->_prev = ret->_prev;
	ret->_next = ret->_prev = ret;

	_takeNum++;

	return ret;
}

void Hashira::DescriptorHeap::ReleaseDescriptor(Descriptor * p)
{
	assert((_descriptors <= p) && (p <= _descriptors + _heapDesc.NumDescriptors + 2));

	Descriptor* next = _unusedDescriptorsList->_next;
	p->_prev = _unusedDescriptorsList;
	p->_next = next;
	--_takeNum;

}

Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& Hashira::DescriptorHeap::GetHeap()
{
	return  _heap;
}

void Hashira::DescriptorHeap::Discard()
{
	delete[] _descriptors;

	SafeRelease(this->_heap);
}

Hashira::DescriptorAllocator::DescriptorAllocator()
{
}

Hashira::DescriptorAllocator::~DescriptorAllocator()
{

	this->Discard();
}

HRESULT Hashira::DescriptorAllocator::Initialize(std::shared_ptr<D3D12Device>& device, D3D12_DESCRIPTOR_HEAP_DESC & desc)
{

	auto hr = device->GetDevice()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&_heap));
	if (FAILED(hr))
	{
		return hr;
	}

	//デスクリプタ分の仕様フラグ配列を確保
	_usedFlags = new Uint8[desc.NumDescriptors];
	memset(_usedFlags, 0, sizeof(Uint8) * desc.NumDescriptors);

	this->_cpuHandleStart = _heap->GetCPUDescriptorHandleForHeapStart();
	this->_gpuHandleStart = _heap->GetGPUDescriptorHandleForHeapStart();

	_heapDesc = desc;
	_descSize = device->GetDevice()->GetDescriptorHandleIncrementSize(desc.Type);

	_allocateCount = 0;
	_currentSeekPos = 0;

	return hr;
}

void Hashira::DescriptorAllocator::Discard()
{
	assert(_allocateCount == 0);

	SafeRelease(_heap);
	SafeDeleteArray(_usedFlags);

}

Hashira::DescriptorInfo Hashira::DescriptorAllocator::Allocate()
{
	DescriptorInfo ret;

	std::lock_guard < std::mutex>lock(_mutex);

	if (_allocateCount == _heapDesc.NumDescriptors)
	{
		return ret;
	}

	auto cp = _currentSeekPos;
	for (Uint32 i = 0; i < _heapDesc.NumDescriptors; i++, cp++)
	{
		cp = cp % _heapDesc.NumDescriptors;
		if (!this->_usedFlags[cp])
		{
			_usedFlags[cp] = 1;
			ret.allocator = this;
			ret.cpuHandle = _cpuHandleStart;
			ret.gpuHandle = _gpuHandleStart;
			//オフセット値でスタートポジションからずらす
			ret.cpuHandle.ptr += _descSize * cp;
			ret.gpuHandle.ptr += _descSize * cp;
			ret.index = cp;

			//現在のSeek位置をずらす
			_currentSeekPos = cp + 1;

			_allocateCount++;

			break;
		}

	}
	return ret;
}

void Hashira::DescriptorAllocator::FreeDescriptor(DescriptorInfo info)
{
	assert(info.allocator == this);
	assert(_usedFlags[info.index] != 0);

	std::lock_guard < std::mutex> lock(_mutex);
	_usedFlags[info.index] = 0;
	_allocateCount--;

}

Hashira::DescriptorStack::DescriptorStack()
{
}

Hashira::DescriptorStack::~DescriptorStack()
{

	void Discard();
}

bool Hashira::DescriptorStack::Allocate(Uint32 count, D3D12_CPU_DESCRIPTOR_HANDLE & cpuHandle, D3D12_GPU_DESCRIPTOR_HANDLE & gpuHandle)
{

	if (_stackPosition + count > this->_stackMax) {
		return false;
	}


	Uint32 prev = _stackPosition;
	_stackPosition += count;

	cpuHandle = _cpuHandleStartPos;
	cpuHandle.ptr += prev * _descSize;
	gpuHandle = _gpuHandleStartPos;
	gpuHandle.ptr += prev * _descSize;
	
	return true;
}

Hashira::DescriptorStackList::DescriptorStackList()
{
}

Hashira::DescriptorStackList::~DescriptorStackList()
{
}

bool Hashira::DescriptorStackList::Initialize(GlobalDescriptorHeap * parentHeap)
{
	_parentHeap = parentHeap;
	_stackIndex = 0;

	return AddStack();
}

void Hashira::DescriptorStackList::Allocate(Uint32 count, D3D12_CPU_DESCRIPTOR_HANDLE & cpuHandle, D3D12_GPU_DESCRIPTOR_HANDLE & gpuHandle)
{
	if (_stacks[_stackIndex].Allocate(count, cpuHandle, gpuHandle))
		return;
	if (!AddStack())
		assert(!"[ERROR] Stack Empty");

	_stackIndex++;


	if (!_stacks[_stackIndex].Allocate(count, cpuHandle, gpuHandle))
		assert(!"[ERROR] Stack Empty");
}

void Hashira::DescriptorStackList::Reset()
{
	for (auto && stack : _stacks)
	{
		stack.Reset();
	}
	_stackIndex = 0;
}

bool Hashira::DescriptorStackList::AddStack()
{
	constexpr int StackNum = 2000;
	DescriptorStack stack;
	_stacks.push_back(stack);
	auto&& s = _stacks.back();
	return _parentHeap->AllocateStack(s, StackNum);
}

bool Hashira::GlobalDescriptorHeap::Initialize(std::shared_ptr<D3D12Device>& device, const D3D12_DESCRIPTOR_HEAP_DESC & desc)
{

	auto hr = device->GetDevice()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&_heap));
	if(FAILED(hr))
	{
		return hr;
	}

	_cpuHandleStartPos = _heap->GetCPUDescriptorHandleForHeapStart();
	_gpuHandleStartPos = _heap->GetGPUDescriptorHandleForHeapStart();

	_heapDesc = desc;
	_descSize = device->GetDevice()->GetDescriptorHandleIncrementSize(desc.Type);
	_allocateCount = 0;

	return hr;
}

bool Hashira::GlobalDescriptorHeap::AllocateStack(DescriptorStack & stack, Uint32 count)
{

	std::lock_guard<std::mutex> lock(_mutex);

	if (_allocateCount + count > _heapDesc.NumDescriptors) 
	{
		return false;
	}

	//スタックに自身が持つヒープのメモリアドレスの開始地点を割り当てる

	stack._cpuHandleStartPos = _cpuHandleStartPos;
	stack._cpuHandleStartPos.ptr += _descSize* _allocateCount;
	stack._gpuHandleStartPos = _gpuHandleStartPos;
	stack._gpuHandleStartPos.ptr += _descSize * _allocateCount;
	stack._descSize = _descSize;
	stack._stackMax = count;
	stack._stackPosition = 0;

	_allocateCount += count;
	
	return true;
}

void Hashira::GlobalDescriptorHeap::Discard()
{
	SafeRelease(_heap);

}

bool Hashira::SamplerDescriptorHeap::Initialize(std::shared_ptr<D3D12Device>& device)
{
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
	desc.NodeMask = 1;
	desc.NumDescriptors = SamplerMaxCount;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

	auto hr = device->GetDevice()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&_heap));
	if (FAILED(hr))
	{
		return false;
	}

	_cpuHandleStartPos = _heap->GetCPUDescriptorHandleForHeapStart();
	_gpuHandleStartPos = _heap->GetGPUDescriptorHandleForHeapStart();

	_descSize = device->GetDevice()->GetDescriptorHandleIncrementSize(desc.Type);
	_allocateCount = 0;

	return false;
}

bool Hashira::SamplerDescriptorHeap::Allocate(Uint32 count, D3D12_CPU_DESCRIPTOR_HANDLE & cpuHandle, D3D12_GPU_DESCRIPTOR_HANDLE & gpuHandle)
{

	if (_allocateCount + count > SamplerMaxCount)
	{
		return false;
	}

	cpuHandle = _cpuHandleStartPos;
	cpuHandle.ptr += _descSize * _allocateCount;
	gpuHandle = _gpuHandleStartPos;
	gpuHandle.ptr += _descSize * _allocateCount;
	_descSize = _descSize;
	_allocateCount += count;
	return true;
}

void Hashira::SamplerDescriptorHeap::Discard()
{

	SafeRelease(_heap);
}

bool Hashira::SamplerDescriptorCache::Initialize(std::shared_ptr<D3D12Device>& device)
{
	this->_device = device;
	
	return AddHeap();
}

bool Hashira::SamplerDescriptorCache::AllocateAndCopy(Uint32 count, D3D12_CPU_DESCRIPTOR_HANDLE* cpuHandle, D3D12_GPU_DESCRIPTOR_HANDLE & gpuHandle)
{

	//キャッシュの存在を確認
	auto hash = CalcFnv1a32(cpuHandle, sizeof(D3D12_CPU_DESCRIPTOR_HANDLE) * count);
	auto findItr = _descCache.find(hash);
	if (findItr != _descCache.end())
	{
		gpuHandle = findItr->second.gpuHandle;
		_last = findItr->second.heap;
		return true;
	}

	//新しくDescriptorを確保する
	D3D12_CPU_DESCRIPTOR_HANDLE cHandle;
	if (!_current->Allocate(count, cHandle, gpuHandle))
	{
		if (!AddHeap())
			return false;
		if (!_current->Allocate(count, cHandle, gpuHandle))
			return false;
	}

	_device->GetDevice()->CopyDescriptors(1, &cHandle, &count, count, cpuHandle, nullptr, D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);

	MapItem item;
	item.heap = _current;
	item.cpuHandle = cHandle;
	item.gpuHandle = gpuHandle;
	_descCache[hash] = item;

	_last = _current;
	return true;
}

void Hashira::SamplerDescriptorCache::Discard()
{
	_heapList.clear();
}

bool Hashira::SamplerDescriptorCache::AddHeap()
{

	auto p = new SamplerDescriptorHeap();

	if (!p->Initialize(_device))
		return false;

	_current = p;

	_heapList.push_back(std::unique_ptr<SamplerDescriptorHeap>(p));

	return true;
}

Hashira::RaytracingDescriptorHeap::RaytracingDescriptorHeap()
{
}

Hashira::RaytracingDescriptorHeap::~RaytracingDescriptorHeap()
{
	this->Discard();
}

bool Hashira::RaytracingDescriptorHeap::Initialize(std::shared_ptr<D3D12Device>& device, const DXRDescriptorHeapInitializeInfo & info)
{
	
	
	this->_info = info;

	_info.bufferCount = Max(info.bufferCount,Hashira::SwapChain::FrameBufferMax);

	this->_localCBVCount = CbvMaxCount - info.globalCBVCount;
	this->_localSRVCount = SrvMaxCount -info.asCount -info.globalSRVCount;
	this->_localUAVCount = UavMaxCount - info.globalUAVCount;
	this->_localSamplerCount = SamplerMaxCount - info.globalSamplerCount;

	Uint32 globalViewMax = GetGlobalViewCount();
	Uint32 globalSamplerMax = GetGlobalSamplerCount();
	Uint32 localViewMax = GetLocalViewCount();
	Uint32 localSamplerMax = GetLocalSamplerCount();

	Uint32 viewMax = Max<Uint32>(1024, globalViewMax * _info.bufferCount + localViewMax * Hashira::SwapChain::FrameBufferMax);
	Uint32 samplerMax = Max<Uint32>(1024, globalSamplerMax * _info.bufferCount + localSamplerMax * Hashira::SwapChain::FrameBufferMax);

	D3D12_DESCRIPTOR_HEAP_DESC desc{};
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	desc.NumDescriptors = viewMax;
	desc.NodeMask = 1;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

	auto hr = device->GetDevice()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&this->_viewHeap));

	if (FAILED(hr))
	{
		return false;
	}

	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
	desc.NumDescriptors = samplerMax;
	desc.NodeMask = 1;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

	hr = device->GetDevice()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&this->_samplerHeap));
	if (FAILED(hr))
	{
		return false;
	}

	_viewCpuHandleStart = _viewHeap->GetCPUDescriptorHandleForHeapStart();
	_viewGpuHandleStart = _viewHeap->GetGPUDescriptorHandleForHeapStart();
	_samplerCpuHandleStart = _samplerHeap->GetCPUDescriptorHandleForHeapStart();
	_samplerGpuHandleStart = _samplerHeap->GetGPUDescriptorHandleForHeapStart();

	_viewDescSize = device->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	_samplerDescSize = device->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);

	return true;
}

void Hashira::RaytracingDescriptorHeap::GetGlobalViewHandleStart(Uint32 frameIndex, D3D12_CPU_DESCRIPTOR_HANDLE & cpu, D3D12_GPU_DESCRIPTOR_HANDLE & gpu)
{
	assert(frameIndex < _info.bufferCount);
	Uint32 viewCount = GetGlobalViewCount();
	cpu = _viewCpuHandleStart;
	cpu.ptr += viewCount * frameIndex * _viewDescSize;
	gpu = _viewGpuHandleStart;
	gpu.ptr += viewCount * frameIndex* _viewDescSize;

}

void Hashira::RaytracingDescriptorHeap::GetGlobalSamplerHandleStart(Uint32 frameIndex, D3D12_CPU_DESCRIPTOR_HANDLE & cpu, D3D12_GPU_DESCRIPTOR_HANDLE & gpu)
{
	assert(frameIndex < _info.bufferCount);
	Uint32 samplerCount = GetGlobalSamplerCount();
	cpu = _samplerCpuHandleStart;
	cpu.ptr += samplerCount * frameIndex * _samplerDescSize;
	gpu = _viewGpuHandleStart;
	gpu.ptr += samplerCount * frameIndex* _samplerDescSize;
}

void Hashira::RaytracingDescriptorHeap::GetLocalViewHandleStart(Uint32 frameIndex, D3D12_CPU_DESCRIPTOR_HANDLE & cpu, D3D12_GPU_DESCRIPTOR_HANDLE & gpu)
{
	assert(frameIndex < _info.bufferCount);
	Uint32 globalCount = GetGlobalViewCount() * _info.bufferCount;
	Uint32 localMax = (_viewDescMaxCount - globalCount) / _info.bufferCount;
	cpu = _viewCpuHandleStart;
	cpu.ptr += (globalCount  + localMax * frameIndex) * _viewDescSize;
	gpu = _viewGpuHandleStart;
	gpu.ptr += (globalCount + localMax * frameIndex) * _viewDescSize;
}

void Hashira::RaytracingDescriptorHeap::GetLocalSamplerHandleStart(Uint32 frameIndex, D3D12_CPU_DESCRIPTOR_HANDLE & cpu, D3D12_GPU_DESCRIPTOR_HANDLE & gpu)
{
	assert(frameIndex < _info.bufferCount);
	Uint32 globalCount = GetGlobalViewCount() * _info.bufferCount;
	Uint32 localMax = (_viewDescMaxCount - globalCount) / _info.bufferCount;
	cpu = _samplerCpuHandleStart;
	cpu.ptr += (globalCount + localMax * frameIndex) * _samplerDescSize;
	gpu = _viewGpuHandleStart;
	gpu.ptr += (globalCount + localMax * frameIndex) * _samplerDescSize;
}

bool Hashira::RaytracingDescriptorHeap::CanResizeMaterialCount(Uint32 materialCount)
{
	Uint32 localViewMax = GetLocalViewCount() * materialCount;
	Uint32 localSamplerMax = GetLocalSamplerCount() * materialCount;

	Uint32 globalMax = GetGlobalSamplerCount() * _info.bufferCount;
	Uint32 curViewMax = (_viewDescMaxCount - globalMax) / _info.bufferCount;
	Uint32 curSamplerMax = (_samplerDescMaxCount - globalMax) / _info.bufferCount;

	if (localViewMax > curViewMax || localSamplerMax > curSamplerMax) {
		return false;
	}

	_info.materialCount = materialCount;

	return true;
}

void Hashira::RaytracingDescriptorHeap::Discard()
{

	SafeRelease(_viewHeap);
	SafeRelease(_samplerHeap);

}

Hashira::RaytracingDescriptorManager::KillPendingHeap::KillPendingHeap(RaytracingDescriptorHeap * heap) :
	heap(heap) , killCount(Hashira::SwapChain::FrameBufferMax)
{
}

Hashira::RaytracingDescriptorManager::RaytracingDescriptorManager()
{
}

bool Hashira::RaytracingDescriptorManager::Initialize(std::shared_ptr<D3D12Device>& device, DXRDescriptorManagerInitializeInfo info)
{
	_device = device;

	_currentHeap = new RaytracingDescriptorHeap();

	DXRDescriptorHeapInitializeInfo hInfo{};
	hInfo.asCount = info.asCount;
	hInfo.bufferCount = info.renderCount * Hashira::SwapChain::FrameBufferMax;
	hInfo.globalCBVCount = info.globalCBVCount;
	hInfo.globalSRVCount = info.globalSRVCount;
	hInfo.globalUAVCount = info.globalUAVCount;
	hInfo.globalSamplerCount = info.globalSamplerCount;
	hInfo.materialCount = info.materialCount;

	if (!_currentHeap->Initialize(device, hInfo))
	{
		delete _currentHeap;
		return false;

	}

	_globalIndex = _localIndex = 0;

	return true;
}

void Hashira::RaytracingDescriptorManager::BeginNewFrame()
{
	for (auto itr = _heapsBeforKill.begin(); itr != _heapsBeforKill.end(); itr++) {

		if (itr->Kill()) 
		{
			itr = _heapsBeforKill.erase(itr);
		}
	}
}

bool Hashira::RaytracingDescriptorManager::ResizeMaterialCount(Uint32 count)
{
	if (_currentHeap->CanResizeMaterialCount(count)) {
		return true;
	}

	auto prevHeap = _currentHeap;

	_currentHeap = new RaytracingDescriptorHeap();

	DXRDescriptorHeapInitializeInfo hInfo{};
	hInfo.asCount = prevHeap->GetASCount();
	hInfo.bufferCount = prevHeap->GetBufferCount();
	hInfo.globalCBVCount = prevHeap->GetGlobalCBVCount;
	hInfo.globalSRVCount = prevHeap->GetGlobalSRVCount;
	hInfo.globalUAVCount = prevHeap->GetGlobalUAVCount;
	hInfo.globalSamplerCount = prevHeap->GetGlobalSamplerCount;
	hInfo.materialCount = count;

	if (!_currentHeap->Initialize(_device , hInfo)) {

		delete _currentHeap;
		_currentHeap = prevHeap;
		return false;
	}

	_heapsBeforKill.push_back(KillPendingHeap(prevHeap));

	return true;
}

void Hashira::RaytracingDescriptorManager::SetHeapToCommandList(std::shared_ptr<CommandList>& list)
{
	
	ID3D12DescriptorHeap* heaps[]= 
	{
		_currentHeap->GetViewHeap().Get(),
		_currentHeap->GetSamplerHeap().Get(),
	};

	list->GetCommandList()->SetDescriptorHeaps(ARRAYSIZE(heaps), heaps);

}

Hashira::RaytracingDescriptorManager::HandleStart 
Hashira::RaytracingDescriptorManager::InclementGlobalHandleStart()
{
	HandleStart ret;
	_currentHeap->GetGlobalViewHandleStart(_globalIndex, ret.viewCpuHandle, ret.viewGpuHandle);
	_currentHeap->GetGlobalSamplerHandleStart(_globalIndex, ret.samplerCpuHandle, ret.samplerGpuHandle);
	_globalIndex = (_globalIndex + 1) % _currentHeap->GetBufferCount();

	return ret;
}

Hashira::RaytracingDescriptorManager::HandleStart 
Hashira::RaytracingDescriptorManager::InclementLocalHandleStart()
{
	HandleStart ret;
	_currentHeap->GetLocalViewHandleStart(_localIndex, ret.viewCpuHandle, ret.viewGpuHandle);
	_currentHeap->GetLocalSamplerHandleStart(_localIndex, ret.samplerCpuHandle, ret.samplerGpuHandle);
	_globalIndex = (_globalIndex + 1) % Hashira::SwapChain::FrameBufferMax;

	return ret;
}

void Hashira::RaytracingDescriptorManager::Discard()
{	
	for (auto itr = _heapsBeforKill.begin(); itr != _heapsBeforKill.end(); itr++) {
		itr->ForceKill();
	}
	_heapsBeforKill.clear();
	SafeDelete(_currentHeap);
}
