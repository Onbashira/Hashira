#include "Buffer.h"
#include "Engine/Source/Utility/D3D12Common.h"
#include "Engine/Source/Utility/Utility.h"
#include "Engine/Source/Debug/Logger.h"
#include "Engine/Source/CommandList/CommandList.h"
#include "Engine/Source/CoreSystem/Framework.h"


Hashira::Buffer::Buffer() :
	_resource(),
	_pDst(nullptr),
	_clearValue({}),
	_currentResourceState(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COMMON),
	_isAllowUAV(),
	_isDynamic(),
	_resDesc(),
	_heapProp(),
	_name("UnNamedResource ")
{
}

Hashira::Buffer::Buffer(const D3D12_HEAP_PROPERTIES& heapProps, const D3D12_HEAP_FLAGS& flags, const D3D12_RESOURCE_DESC& resourceDesc, const D3D12_RESOURCE_STATES& state, D3D12_CLEAR_VALUE* clearValue) :
	_resource(),
	_pDst(nullptr),
	_clearValue({}),
	_currentResourceState(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COMMON),
	_isAllowUAV(),
	_isDynamic(),
	_resDesc(),
	_heapProp(),
	_name("UnNamedResource ")

{
	HRESULT ret = Initialize(heapProps, flags, resourceDesc, state, clearValue);
	assert(ret == S_OK);
}

Hashira::Buffer::Buffer(const Buffer& other)
{
	*this = other;
}

Hashira::Buffer::Buffer(Buffer&& value)
{
	*this = std::move(value);
}


Hashira::Buffer& Hashira::Buffer::operator=(const Buffer& value)
{
	this->Discard();
	this->_clearValue = value._clearValue;
	this->_currentResourceState = value._currentResourceState;
	this->_name = value._name;
	this->_pDst = value._pDst;
	this->_resource.Attach(value._resource.Get());
	this->_heapProp = value._heapProp;
	this->_isDynamic = value._isDynamic;
	this->_isAllowUAV = value._isAllowUAV;

	return *this;
}

Hashira::Buffer& Hashira::Buffer::operator=(Buffer&& value)
{
	*this = value;
	value.Discard();
	value._clearValue = D3D12_CLEAR_VALUE();
	value._currentResourceState = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ;
	value._name = "MOVED RESOURCE";
	value._pDst = nullptr;
	value._resource.Detach();
	value._heapProp = D3D12_HEAP_PROPERTIES();
	value._isDynamic = false;
	value._isAllowUAV = false;
	return *this;
}

Hashira::Buffer::~Buffer()
{
	Discard();
}

std::unique_ptr<Hashira::Buffer> Hashira::Buffer::CreateUnique()
{
	return std::unique_ptr<Hashira::Buffer>(new Hashira::Buffer());
}

std::shared_ptr<Hashira::Buffer> Hashira::Buffer::CreateShared()
{
	return std::shared_ptr<Hashira::Buffer>(new Hashira::Buffer());
}

HRESULT Hashira::Buffer::Initialize(const D3D12_HEAP_PROPERTIES& heapProps, const  D3D12_HEAP_FLAGS& flags, const  D3D12_RESOURCE_DESC& resourceDesc, const D3D12_RESOURCE_STATES& state, D3D12_CLEAR_VALUE* clearValue)
{

	_currentResourceState = state;
	if (clearValue != nullptr) {
		_clearValue = *clearValue;
	}

	if (_resource.Get() != nullptr) {
		Discard();
	}

	auto hr = Framework::GetInstance().GetDevice()->GetDevice()->CreateCommittedResource(&heapProps, flags, &resourceDesc, state, clearValue, IID_PPV_ARGS(&this->_resource));

	if (FAILED(hr)) {
		SystemLogger::GetInstance().Log(LOG_LEVEL::Error, hr);

		return E_FAIL;
	}
	this->_resDesc = resourceDesc;
	this->_heapProp = heapProps;
	this->_clearValue = *clearValue;
	this->_currentResourceState = state;
	this->_isDynamic = (_heapProp.Type == D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD) ? true :
		(_heapProp.Type == D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_CUSTOM
			&& _heapProp.MemoryPoolPreference == D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_L0) ? true : false;
	this->_isAllowUAV = (resourceDesc.Flags == D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS) ? true : false;
	return S_OK;
}

HRESULT Hashira::Buffer::Initialize(std::shared_ptr<D3D12Device>& device, const D3D12_HEAP_PROPERTIES& heapProps, const D3D12_HEAP_FLAGS& flags, const D3D12_RESOURCE_DESC& resourceDesc, const D3D12_RESOURCE_STATES& state, D3D12_CLEAR_VALUE* clearValue)
{
	if (clearValue != nullptr) {
		_clearValue = *clearValue;
	}

	if (_resource.Get() != nullptr) {
		Discard();
	}

	auto hr = device->GetDevice()->CreateCommittedResource(&heapProps, flags, &resourceDesc, state, clearValue, IID_PPV_ARGS(&this->_resource));

	if (FAILED(hr)) {
		SystemLogger::GetInstance().Log(LOG_LEVEL::Error, hr);

		return E_FAIL;
	}

	this->_resDesc = resourceDesc;
	this->_heapProp = heapProps;
	this->_clearValue = *clearValue;
	this->_currentResourceState = state;
	this->_isDynamic = (_heapProp.Type == D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD)? true : 
		(_heapProp.Type == D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_CUSTOM
			&&_heapProp.MemoryPoolPreference == D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_L0) ? true:false;
	this->_isAllowUAV = (resourceDesc.Flags == D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS) ? true : false;
	return S_OK;
}

HRESULT Hashira::Buffer::Map(UINT subResource, D3D12_RANGE* readRange)
{
	if (_resource.Get() != nullptr) {

		auto hr = _resource->Map(subResource, readRange, reinterpret_cast<void**>(&_pDst));
		if (FAILED(hr)) {
			return E_FAIL;
		}
	}
	return S_OK;

}

void Hashira::Buffer::Unmap(UINT subResource, D3D12_RANGE* writtenRange)
{
	if (_resource.Get() != nullptr) {
		_resource->Unmap(subResource, writtenRange);
		_pDst = nullptr;
	}
}

UCHAR* Hashira::Buffer::GetMappedPointer()
{
	return _pDst;
}

void Hashira::Buffer::Discard()
{
	if (_resource.Get() != nullptr) {
		if (_pDst != nullptr) {
			Unmap(0, nullptr);
		}
		_resource.Reset();
		DEBUG_LOG(std::string("Buffer : " + _name + " is  Reset"));
	}
}

void Hashira::Buffer::Update(const void* pSrc, ULONG64 size, const size_t dstOffset)
{
	assert(pSrc != nullptr);
	assert(size != 0);
	std::memcpy(_pDst + dstOffset, pSrc, size_t(size));
}

void Hashira::Buffer::Read(void* pDstBuffer, ULONG64 readSize, const unsigned int dstOffsest)
{
	assert(pDstBuffer != nullptr);
	assert(readSize != 0);
	std::memcpy(pDstBuffer, _pDst + dstOffsest, size_t(readSize));
}

void Hashira::Buffer::Update(const void* pSrc, ULONG64 size, const UINT dstOffset, UINT subResource, D3D12_RANGE* readRange, D3D12_RANGE* writtenRange)
{
	Map(subResource, readRange);
	Update(pSrc, size, dstOffset);
	Unmap(subResource, writtenRange);
}

void Hashira::Buffer::Alignment256ByteUpdate(const void* pSrc, unsigned int elementSize, unsigned int elementCount)
{
	UINT offset = 0;
	for (unsigned int i = 0; i < elementCount; i++) {
		Update(static_cast<void*>((char*)(pSrc)+i * elementSize), Util::Alignment256Bytes(elementSize), offset);
		offset += static_cast<unsigned int>(Util::Alignment256Bytes(elementSize));
	}
}

Microsoft::WRL::ComPtr<ID3D12Resource>& Hashira::Buffer::GetResource()
{
	return _resource;
}

const std::tuple<D3D12_HEAP_PROPERTIES, D3D12_HEAP_FLAGS> Hashira::Buffer::GetHeapPropaties()
{
	D3D12_HEAP_PROPERTIES props;
	D3D12_HEAP_FLAGS flags;
	_resource->GetHeapProperties(&props, &flags);
	return { props, flags };
}

const D3D12_RESOURCE_DESC Hashira::Buffer::GetResourceDesc()
{
	return _resource->GetDesc();
}

const D3D12_RESOURCE_STATES& Hashira::Buffer::GetResourceState()
{
	return   _currentResourceState;
}

void Hashira::Buffer::SetResourceState(D3D12_RESOURCE_STATES state)
{
	_currentResourceState = state;
}

HRESULT Hashira::Buffer::ResourceTransition(CommandList* list, D3D12_RESOURCE_STATES nextState)
{
	D3D12_RESOURCE_BARRIER resource_barrier{};

	resource_barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	resource_barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	resource_barrier.Transition.pResource = this->_resource.Get();
	resource_barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	resource_barrier.Transition.StateBefore = _currentResourceState;
	resource_barrier.Transition.StateAfter = nextState;
	_currentResourceState = nextState;
	list->GetCommandList()->ResourceBarrier(1, &resource_barrier);

	return S_OK;

}

HRESULT Hashira::Buffer::ResourceTransition(std::weak_ptr<CommandList> list, D3D12_RESOURCE_STATES nextState)
{
	D3D12_RESOURCE_BARRIER resource_barrier{};

	resource_barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	resource_barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	resource_barrier.Transition.pResource = this->_resource.Get();
	resource_barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	resource_barrier.Transition.StateBefore = _currentResourceState;
	resource_barrier.Transition.StateAfter = nextState;
	_currentResourceState = nextState;
	list.lock()->GetCommandList()->ResourceBarrier(1, &resource_barrier);

	return S_OK;
}

D3D12_CLEAR_VALUE Hashira::Buffer::GetClearValue()
{
	return _clearValue;
}

void Hashira::Buffer::SetName(std::string name)
{
	_name = name;
	_resource->SetName(Util::StringToWString(_name).c_str());
}
