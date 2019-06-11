#include "stdafx.h"
#include "GameHeap.h"
#include "Engine/Source/CoreSystem/Framework.h"
#include "Engine/Source/Resource/Resource.h"
#include "Engine/Source/DescriptorHeap/Descriptor.h"


std::shared_ptr<Hashira::Descriptor> Hashira::GameHeap::InvalidDescriptor = std::make_shared<Hashira::Descriptor>();

Hashira::GameHeap::GameHeap(std::shared_ptr<D3D12Device>& device,GameHeapDesc& desc) :
	_device(device), _cpuOffset(0), _rtvOffset(0), _dsvOffset(0), _samplerOffset(0)
{

	_heaps[HeapType::CPU] = std::make_shared<DescriptorHeap>();
	_heaps[HeapType::CPU]->Initialize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, desc.maxCPUHeapSize);

	_heaps[HeapType::RTV] = std::make_shared<DescriptorHeap>();
	_heaps[HeapType::RTV]->Initialize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV, desc.maxRTHeapSize);

	_heaps[HeapType::DSV] = std::make_shared<DescriptorHeap>();
	_heaps[HeapType::DSV]->Initialize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_DSV, desc.maxDSHeapSize);

	_heaps[HeapType::SAMP] = std::make_shared<DescriptorHeap>();
	_heaps[HeapType::SAMP]->Initialize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, desc.maxSampHeapSize);

	_allocatedDescMap[HeapType::CPU].resize(desc.maxCPUHeapSize);
	_allocatedDescMap[HeapType::RTV].resize(desc.maxRTHeapSize);
	_allocatedDescMap[HeapType::DSV].resize(desc.maxDSHeapSize);
	_allocatedDescMap[HeapType::SAMP].resize(desc.maxSampHeapSize);

	_allocatedViewMap[HeapType::CPU].resize(desc.maxCPUHeapSize);
	_allocatedViewMap[HeapType::RTV].resize(desc.maxRTHeapSize);
	_allocatedViewMap[HeapType::DSV].resize(desc.maxDSHeapSize);
	_allocatedViewMap[HeapType::SAMP].resize(desc.maxSampHeapSize);
}

Hashira::GameHeap::~GameHeap()
{
	Discard();
	_device.reset();
}

std::shared_ptr<Hashira::GameHeap> Hashira::GameHeap::CreateGameHeap(std::shared_ptr<D3D12Device>& device, GameHeapDesc* desc)
{

	auto ret = std::make_shared<GameHeap>(device, *desc);
	return ret;
}

HRESULT Hashira::GameHeap::ReInitialize(GameHeapDesc* desc)
{
	Discard();

	_heaps[HeapType::CPU] = std::make_shared<DescriptorHeap>();
	_heaps[HeapType::CPU]->Initialize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, desc->maxCPUHeapSize);

	_heaps[HeapType::RTV] = std::make_shared<DescriptorHeap>();
	_heaps[HeapType::RTV]->Initialize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV, desc->maxRTHeapSize);

	_heaps[HeapType::DSV] = std::make_shared<DescriptorHeap>();
	_heaps[HeapType::DSV]->Initialize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_DSV, desc->maxDSHeapSize);

	_heaps[HeapType::SAMP] = std::make_shared<DescriptorHeap>();
	_heaps[HeapType::SAMP]->Initialize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, desc->maxSampHeapSize);

	_allocatedDescMap[HeapType::CPU].resize(desc->maxCPUHeapSize);
	_allocatedDescMap[HeapType::RTV].resize(desc->maxRTHeapSize);
	_allocatedDescMap[HeapType::DSV].resize(desc->maxDSHeapSize);
	_allocatedDescMap[HeapType::SAMP].resize(desc->maxSampHeapSize);

	_allocatedViewMap[HeapType::CPU].resize(desc->maxCPUHeapSize);
	_allocatedViewMap[HeapType::RTV].resize(desc->maxRTHeapSize);
	_allocatedViewMap[HeapType::DSV].resize(desc->maxDSHeapSize);
	_allocatedViewMap[HeapType::SAMP].resize(desc->maxSampHeapSize);

	return S_OK;
}

HRESULT Hashira::GameHeap::ReCreateHeap(const HeapType & heapType, unsigned int maxHeapSize)
{
	switch (heapType)
	{
	case Hashira::GameHeap::HeapType::CPU:
	{
		unsigned int size = _heaps[HeapType::CPU]->GetDesc().NumDescriptors;
		_heaps[HeapType::CPU].reset();
		_heaps[HeapType::CPU] = std::make_shared<DescriptorHeap>();
		_heaps[HeapType::CPU]->Initialize(_device, D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, size);
	}
	break;
	case Hashira::GameHeap::HeapType::RTV:
	{
		unsigned int size = _heaps[HeapType::RTV]->GetDesc().NumDescriptors;
		_heaps[HeapType::RTV].reset();
		_heaps[HeapType::RTV] = std::make_shared<DescriptorHeap>();
		_heaps[HeapType::RTV]->Initialize(_device, D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV, size);
	}
	break;
	case Hashira::GameHeap::HeapType::DSV:
	{
		unsigned int size = _heaps[HeapType::DSV]->GetDesc().NumDescriptors;
		_heaps[HeapType::DSV].reset();
		_heaps[HeapType::DSV] = std::make_shared<DescriptorHeap>();
		_heaps[HeapType::DSV]->Initialize(_device, D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_DSV, size);
	}
	break;
	case Hashira::GameHeap::HeapType::SAMP:
	{
		unsigned int size = _heaps[HeapType::SAMP]->GetDesc().NumDescriptors;
		_heaps[HeapType::SAMP].reset();
		_heaps[HeapType::SAMP] = std::make_shared<DescriptorHeap>();
		_heaps[HeapType::SAMP]->Initialize(_device, D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, size);
	}
	break;
	default:
		break;
	}
	return S_OK;
}

std::weak_ptr<Hashira::Descriptor> Hashira::GameHeap::GetDescriptorHandle(const Hashira::GameHeap::HeapType & heapType, unsigned int handleOffset)
{

	if (_allocatedViewMap[heapType].size() <= handleOffset ||
		_allocatedViewMap[heapType][handleOffset] == Descriptor::ViewType::EMPTY
		)
	{
		return InvalidDescriptor;
	}

	return this->_allocatedDescMap[heapType][handleOffset];
}

std::weak_ptr<Hashira::Descriptor> Hashira::GameHeap::CreateView(Hashira::GameHeap::HeapType heapType, Hashira::Descriptor::ViewType  viewType, void * viewDesc, unsigned int handleOffset, Resource* resource, Resource* counterResource)
{

	if (_allocatedViewMap[heapType].size() <= handleOffset || _allocatedViewMap[heapType][handleOffset] == Descriptor::ViewType::EMPTY) {
		return InvalidDescriptor;
	}

	auto cpuPtr = _heaps[heapType]->GetCPUHandle(handleOffset);

	switch (viewType)
	{
	case Hashira::Descriptor::ViewType::CBV:

	{
		_device->GetDevice()->CreateConstantBufferView((D3D12_CONSTANT_BUFFER_VIEW_DESC*)viewDesc, cpuPtr);
	}
	break;
	case Hashira::Descriptor::ViewType::SRV:
	{
		_device->GetDevice()->CreateShaderResourceView(resource->GetResource().Get(), (D3D12_SHADER_RESOURCE_VIEW_DESC*)viewDesc, cpuPtr);
	}
	break;
	case Hashira::Descriptor::ViewType::UAV:
	{
		_device->GetDevice()->CreateUnorderedAccessView(resource->GetResource().Get(), counterResource->GetResource().Get(), (D3D12_UNORDERED_ACCESS_VIEW_DESC*)viewDesc, cpuPtr);

	}
	break;
	case Hashira::Descriptor::ViewType::RTV:
	{
		_device->GetDevice()->CreateRenderTargetView(resource->GetResource().Get(), (D3D12_RENDER_TARGET_VIEW_DESC*)viewDesc, cpuPtr);
	}
	break;
	case Hashira::Descriptor::ViewType::DSV:
	{
		_device->GetDevice()->CreateDepthStencilView(resource->GetResource().Get(), (D3D12_DEPTH_STENCIL_VIEW_DESC*)viewDesc, cpuPtr);

	}
	break;
	case Hashira::Descriptor::ViewType::SAMP:
	{
		_device->GetDevice()->CreateSampler((D3D12_SAMPLER_DESC*)viewDesc, cpuPtr);
	}
	break;
	case Hashira::Descriptor::ViewType::EMPTY:
		//èàóùÇÃë±çs
	default:

	{
		return InvalidDescriptor;
	}
	break;
	}

	_allocatedViewMap[heapType][handleOffset] = viewType;
	std::shared_ptr<Descriptor> ptr = std::make_shared<Descriptor>(
		handleOffset,
		_allocatedViewMap[heapType][handleOffset],
		_heaps[heapType]->GetCPUHandle(handleOffset),
		_heaps[heapType]->GetGPUHandle(handleOffset)
		);

	_allocatedDescMap[heapType][handleOffset] = ptr;

	return ptr;

}

std::weak_ptr<Hashira::Descriptor> Hashira::GameHeap::CreateCBView(D3D12_CONSTANT_BUFFER_VIEW_DESC& desc)
{
	std::lock_guard mutex(_cpuMutex);

	auto cpuPtr = _heaps[HeapType::CPU]->GetCPUHandle(_cpuOffset);
	_device->GetDevice()->CreateConstantBufferView(&desc, cpuPtr);

	_allocatedViewMap[HeapType::CPU][_cpuOffset] = Descriptor::ViewType::CBV;
	std::shared_ptr<Descriptor> ptr = std::make_shared<Descriptor>(_cpuOffset, Descriptor::ViewType::CBV, cpuPtr, _heaps[HeapType::CPU]->GetGPUHandle(_cpuOffset));
	_allocatedDescMap[HeapType::CPU][_cpuOffset++] = ptr;

	return ptr;
}

std::weak_ptr<Hashira::Descriptor> Hashira::GameHeap::CreateSRView(D3D12_SHADER_RESOURCE_VIEW_DESC& desc, Resource * resource)
{
	std::lock_guard mutex(_cpuMutex);

	auto cpuPtr = _heaps[HeapType::CPU]->GetCPUHandle(_cpuOffset);
	_device->GetDevice()->CreateShaderResourceView(resource->GetResource().Get(), &desc, cpuPtr);

	_allocatedViewMap[HeapType::CPU][_cpuOffset] = Descriptor::ViewType::SRV;
	std::shared_ptr<Descriptor> ptr = std::make_shared<Descriptor>(_cpuOffset, Descriptor::ViewType::SRV, cpuPtr, _heaps[HeapType::CPU]->GetGPUHandle(_cpuOffset));
	_allocatedDescMap[HeapType::CPU][_cpuOffset++] = ptr;

	return ptr;
}

std::weak_ptr<Hashira::Descriptor> Hashira::GameHeap::CreateUAView(D3D12_UNORDERED_ACCESS_VIEW_DESC& desc, Resource * resource, Resource * counterResource)
{
	std::lock_guard mutex(_cpuMutex);

	auto cpuPtr = _heaps[HeapType::CPU]->GetCPUHandle(_cpuOffset);
	_device->GetDevice()->CreateUnorderedAccessView(resource->GetResource().Get(), counterResource->GetResource().Get(), &desc, cpuPtr);

	_allocatedViewMap[HeapType::CPU][_cpuOffset] = Descriptor::ViewType::UAV;
	std::shared_ptr<Descriptor> ptr = std::make_shared<Descriptor>(_cpuOffset, Descriptor::ViewType::UAV, cpuPtr, _heaps[HeapType::CPU]->GetGPUHandle(_cpuOffset));
	_allocatedDescMap[HeapType::CPU][_cpuOffset++] = ptr;

	return ptr;
}

std::weak_ptr<Hashira::Descriptor> Hashira::GameHeap::CreateRTView(D3D12_RENDER_TARGET_VIEW_DESC& desc, Resource * resource)
{
	std::lock_guard mutex(_rtvMutex);

	auto cpuPtr = _heaps[HeapType::RTV]->GetCPUHandle(_rtvOffset);
	_device->GetDevice()->CreateRenderTargetView(resource->GetResource().Get(), &desc, cpuPtr);

	_allocatedViewMap[HeapType::RTV][_rtvOffset] = Descriptor::ViewType::RTV;
	std::shared_ptr<Descriptor> ptr = std::make_shared<Descriptor>(_rtvOffset, Descriptor::ViewType::RTV, _heaps[HeapType::RTV]->GetCPUHandle(_rtvOffset), _heaps[HeapType::RTV]->GetGPUHandle(_rtvOffset));
	_allocatedDescMap[HeapType::RTV][_rtvOffset++] = ptr;

	return ptr;

}


std::weak_ptr<Hashira::Descriptor> Hashira::GameHeap::CreateDSView(D3D12_DEPTH_STENCIL_VIEW_DESC& desc, Resource * resource)
{
	std::lock_guard mutex(_dsvMutex);

	auto cpuPtr = _heaps[HeapType::DSV]->GetCPUHandle(_dsvOffset);
	_device->GetDevice()->CreateDepthStencilView(resource->GetResource().Get(), &desc, cpuPtr);

	_allocatedViewMap[HeapType::DSV][_dsvOffset] = Descriptor::ViewType::DSV;
	std::shared_ptr<Descriptor> ptr = std::make_shared<Descriptor>(_dsvOffset, Descriptor::ViewType::DSV, _heaps[HeapType::DSV]->GetCPUHandle(_dsvOffset), _heaps[HeapType::DSV]->GetGPUHandle(_dsvOffset));
	_allocatedDescMap[HeapType::DSV][_dsvOffset++] = ptr;

	return ptr;
}

std::weak_ptr<Hashira::Descriptor> Hashira::GameHeap::CreateSampView(D3D12_SAMPLER_DESC & desc)
{
	std::lock_guard mutex(_dsvMutex);

	auto cpuPtr = _heaps[HeapType::SAMP]->GetCPUHandle(_samplerOffset);
	_device->GetDevice()->CreateSampler(&desc, cpuPtr);

	_allocatedViewMap[HeapType::SAMP][_samplerOffset] = Descriptor::ViewType::SAMP;
	std::shared_ptr<Descriptor> ptr = std::make_shared<Descriptor>(_samplerOffset, Descriptor::ViewType::SAMP, _heaps[HeapType::SAMP]->GetCPUHandle(_samplerOffset), _heaps[HeapType::SAMP]->GetGPUHandle(_samplerOffset));
	_allocatedDescMap[HeapType::SAMP][_samplerOffset++] = ptr;

	return ptr;
}

void Hashira::GameHeap::SetGameHeap(std::shared_ptr<Hashira::CommandList> list)
{

	ID3D12DescriptorHeap* heaps[] = {
		this->_heaps[HeapType::CPU]->GetHeap().Get(),
		//this->_heaps[HeapType::SAMP]->GetHeap().Get()
	};
	list->GetCommandList()->SetDescriptorHeaps(_countof(heaps), heaps);


}

void Hashira::GameHeap::Discard()
{
	_heaps[HeapType::CPU]->Discard();
	_heaps[HeapType::RTV]->Discard();
	_heaps[HeapType::DSV]->Discard();
	_heaps[HeapType::SAMP]->Discard();

	this->_allocatedViewMap[HeapType::CPU].clear();
	this->_allocatedViewMap[HeapType::RTV].clear();
	this->_allocatedViewMap[HeapType::DSV].clear();
	this->_allocatedViewMap[HeapType::SAMP].clear();

	this->_allocatedViewMap[HeapType::CPU].shrink_to_fit();
	this->_allocatedViewMap[HeapType::RTV].shrink_to_fit();
	this->_allocatedViewMap[HeapType::DSV].shrink_to_fit();
	this->_allocatedViewMap[HeapType::SAMP].shrink_to_fit();

	_allocatedDescMap[HeapType::CPU].clear();
	_allocatedDescMap[HeapType::RTV].clear();
	_allocatedDescMap[HeapType::DSV].clear();
	_allocatedDescMap[HeapType::SAMP].clear();

	_allocatedDescMap[HeapType::CPU].shrink_to_fit();
	_allocatedDescMap[HeapType::RTV].shrink_to_fit();
	_allocatedDescMap[HeapType::DSV].shrink_to_fit();
	_allocatedDescMap[HeapType::SAMP].shrink_to_fit();

	_cpuOffset = 0;
	_rtvOffset = 0;
	_dsvOffset = 0;
	_samplerOffset = 0;

}
