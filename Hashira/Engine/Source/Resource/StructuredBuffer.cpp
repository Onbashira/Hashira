#include "StructuredBuffer.h"
#include "Engine/Source/CoreSystem/Framework.h"


Hashira::StructuredBuffer::StructuredBuffer()
{
}


Hashira::StructuredBuffer::~StructuredBuffer()
{
	this->Discard();
}

HRESULT Hashira::StructuredBuffer::CreateHeap(unsigned numElements, unsigned int nodeMask)
{
	auto hr = this->_heap.Initialize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, numElements, nodeMask);
	_heap.SetName("UnorderedAccessValue_Heap");
	return hr;
}

HRESULT Hashira::StructuredBuffer::Initialize(unsigned int elementSize, unsigned int numElements, void* pBufferData)
{
	if (elementSize == 0 || numElements == 0) {
		return E_FAIL;
	}
#pragma region Initilaize UnorderdResource and StagingResource


	{
		D3D12_HEAP_PROPERTIES defaultHeapProp = {};
		D3D12_HEAP_PROPERTIES readBackHeapProp = {};

		D3D12_RESOURCE_DESC defaultResourceDesc = {};
		D3D12_RESOURCE_DESC readBackResourceDesc = {};
		{
			defaultHeapProp.Type = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_CUSTOM;
			defaultHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
			defaultHeapProp.VisibleNodeMask = 1;
			defaultHeapProp.CreationNodeMask = 1;
			defaultHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_L0;
		}

		{

			readBackHeapProp.Type = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_READBACK;
			readBackHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
			readBackHeapProp.VisibleNodeMask = 1;
			readBackHeapProp.CreationNodeMask = 1;
			readBackHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		}

		{
			defaultResourceDesc.Alignment = 0;
			defaultResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_BUFFER;
			defaultResourceDesc.Width = numElements * elementSize;
			defaultResourceDesc.Height = 1;
			defaultResourceDesc.DepthOrArraySize = 1;
			defaultResourceDesc.MipLevels = 1;
			defaultResourceDesc.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
			defaultResourceDesc.Layout = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
			defaultResourceDesc.SampleDesc.Count = 1;
			defaultResourceDesc.SampleDesc.Quality = 0;
			defaultResourceDesc.Flags = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;

		}
		{
			readBackResourceDesc.Alignment = 0;
			readBackResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_BUFFER;
			readBackResourceDesc.Width = numElements * elementSize;
			readBackResourceDesc.Height = 1;
			readBackResourceDesc.DepthOrArraySize = 1;
			readBackResourceDesc.MipLevels = 1;
			readBackResourceDesc.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
			readBackResourceDesc.Layout = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
			readBackResourceDesc.SampleDesc.Count = 1;
			readBackResourceDesc.SampleDesc.Quality = 0;
			readBackResourceDesc.Flags = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;
		}

		//ƒŠƒ\[ƒXì¬
		{
			Resource::Initialize(defaultHeapProp, D3D12_HEAP_FLAG_NONE, defaultResourceDesc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
#ifdef _DEBUG
			SetName("UAVResource");
#endif // _DEBUG

		}
	}
#pragma endregion
	//ReadRangeSetUp
	{
		_readRange.Begin = 0;
		_readRange.End = 1;
	}
	//this->WriteToBuffer(numElements, elementSize, pBufferData);

	if (pBufferData != nullptr) {
		this->Map(0, &this->_readRange);
		this->Update(pBufferData, numElements*elementSize, 0);
		this->Unmap(0, &this->_readRange);
	}

	CHECK_RESULT(CreateHeap(HEAP_OFFSET::HEAP_OFFSET_MAX));
	CHECK_RESULT(CreateDescriptors(elementSize, numElements));

	return S_OK;
}

HRESULT Hashira::StructuredBuffer::CreateDescriptors(unsigned int elementSize, unsigned int numElements)
{
	//UAVStructuredView
	{
		D3D12_BUFFER_UAV uavBuffer;
		uavBuffer.FirstElement = 0;
		uavBuffer.NumElements = numElements;
		uavBuffer.Flags = D3D12_BUFFER_UAV_FLAGS::D3D12_BUFFER_UAV_FLAG_NONE;
		uavBuffer.CounterOffsetInBytes = 0;
		uavBuffer.StructureByteStride = elementSize;


		_unorderedAccessViewDesc.ViewDimension = D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_BUFFER;
		_unorderedAccessViewDesc.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
		_unorderedAccessViewDesc.Buffer = uavBuffer;

		this->CreateView(&_unorderedAccessViewDesc, _heap.GetCPUHandle(HEAP_OFFSET::UAV_DESCRIPTOR_OFFSET));
	}
	//SRVRawView
	{
		_shaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
		_shaderResourceViewDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		_shaderResourceViewDesc.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
		_shaderResourceViewDesc.Buffer.FirstElement = 0;
		_shaderResourceViewDesc.Buffer.NumElements = numElements;
		_shaderResourceViewDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAGS::D3D12_BUFFER_SRV_FLAG_NONE;
		_shaderResourceViewDesc.Buffer.StructureByteStride = elementSize;
		this->CreateView(&_shaderResourceViewDesc, _heap.GetCPUHandle(HEAP_OFFSET::SRV_DESCRIPTOR_OFFSET));
	}
	return S_OK;

}

D3D12_CPU_DESCRIPTOR_HANDLE Hashira::StructuredBuffer::GetSRVCPUHandle()
{
	return _heap.GetCPUHandle(HEAP_OFFSET::SRV_DESCRIPTOR_OFFSET);
}

D3D12_CPU_DESCRIPTOR_HANDLE Hashira::StructuredBuffer::GetUAVCPUHandle()
{
	return _heap.GetCPUHandle(HEAP_OFFSET::UAV_DESCRIPTOR_OFFSET);
}

D3D12_GPU_DESCRIPTOR_HANDLE Hashira::StructuredBuffer::GetSRVGPUHandle()
{
	return _heap.GetGPUHandle(HEAP_OFFSET::SRV_DESCRIPTOR_OFFSET);
}

D3D12_GPU_DESCRIPTOR_HANDLE Hashira::StructuredBuffer::GetUAVGPUHandle()
{
	return _heap.GetGPUHandle(HEAP_OFFSET::UAV_DESCRIPTOR_OFFSET);
}

HRESULT Hashira::StructuredBuffer::CreateView(D3D12_UNORDERED_ACCESS_VIEW_DESC * uavDesc, D3D12_CPU_DESCRIPTOR_HANDLE cpuDescriptorHandle, Resource * counterResource)
{
	
	Framework::GetInstance().GetDevice()->GetDevice()->CreateUnorderedAccessView(_resource.Get(), counterResource == nullptr ?  nullptr: counterResource->GetResource().Get(), uavDesc, cpuDescriptorHandle);
	return S_OK;
}

HRESULT Hashira::StructuredBuffer::CreateView(D3D12_SHADER_RESOURCE_VIEW_DESC * srvDesc, D3D12_CPU_DESCRIPTOR_HANDLE cpuDescriptorHandle)
{
	Framework::GetInstance().GetDevice()->GetDevice()->CreateShaderResourceView(_resource.Get(), srvDesc, cpuDescriptorHandle);
	return S_OK;
}

void Hashira::StructuredBuffer::WriteToBuffer(unsigned int numElements, unsigned int elementSize, void * pBufferData)
{
	D3D12_RANGE range = { 0,1 };

	Map(0, &range);
	Update(pBufferData, numElements * elementSize, 0);
	Unmap(0, nullptr);
}

void Hashira::StructuredBuffer::Discard()
{
	_heap.Discard();
}

Hashira::DescriptorHeap * Hashira::StructuredBuffer::GetHeap()
{
	return &_heap;
}

D3D12_UNORDERED_ACCESS_VIEW_DESC Hashira::StructuredBuffer::GetUAVDesc()
{
	return _unorderedAccessViewDesc;
}

D3D12_SHADER_RESOURCE_VIEW_DESC Hashira::StructuredBuffer::GetSRVDesc()
{
	return _shaderResourceViewDesc;
}
