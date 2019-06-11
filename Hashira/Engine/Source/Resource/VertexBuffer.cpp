#include "VertexBuffer.h"
#include "Engine/Source/CoreSystem/Framework.h"


Hashira::VertexBuffer::VertexBuffer()  :
	_view({}), _size(0), _stride(0)
{
	_view.BufferLocation = 0;
	_view.SizeInBytes = 0;
	_view.StrideInBytes = 0;
}

Hashira::VertexBuffer::VertexBuffer(const VertexBuffer & other)
{
	*this =	other;
}

Hashira::VertexBuffer::VertexBuffer(VertexBuffer && value)
{
	*this = std::move(value);
}

Hashira::VertexBuffer & Hashira::VertexBuffer::operator=(const VertexBuffer & value)
{
	Resource::operator=(value);
	this->_view = value._view;
	this->_stride = value._stride;
	this->_size = value._size;
	return *this;
}

Hashira::VertexBuffer & Hashira::VertexBuffer::operator=(VertexBuffer && value)
{
	Resource::operator=(std::move(value));
	this->_view =	value._view;
	this->_stride = value._stride;
	this->_size =	value._size;

	value._view = D3D12_VERTEX_BUFFER_VIEW();
	value._stride = 0;
	value._size = 0;

	return *this;
}

Hashira::VertexBuffer::~VertexBuffer()
{
	Discard();
}


HRESULT Hashira::VertexBuffer::Initialize(ULONG64 size, UINT stride, const void * pVertices)
{
	if (pVertices == nullptr || size == 0 || stride == 0) {
		return E_FAIL;
	}
	_size = size;
	_stride = stride;

	D3D12_HEAP_PROPERTIES prop = {};
	prop.Type = D3D12_HEAP_TYPE_UPLOAD;
	prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	prop.CreationNodeMask = 1;
	prop.VisibleNodeMask = 1;

	D3D12_RESOURCE_DESC desc = {};
	desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Alignment = 0;
	desc.Width = _size;
	desc.Height = 1;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 1;
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	desc.Flags = D3D12_RESOURCE_FLAG_NONE;

	auto hr = Framework::GetInstance().GetDevice()->GetDevice()->CreateCommittedResource(&prop, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(this->GetResource().GetAddressOf()));

	if (FAILED(hr)) {
		return E_FAIL;
	}


	hr = Map(0, nullptr);
	if (FAILED(hr)) {
		return E_FAIL;
	}
	Update(pVertices, size_t(_size), 0);
	Unmap(0, nullptr);

	_view.BufferLocation = GetResource()->GetGPUVirtualAddress();
	_view.SizeInBytes = static_cast<UINT>(_size);
	_view.StrideInBytes = _stride;


	return S_OK;

}

void Hashira::VertexBuffer::Discard()
{
	_view.BufferLocation = 0;
	_view.SizeInBytes = 0;
	_view.StrideInBytes = 0;

}

D3D12_VERTEX_BUFFER_VIEW& Hashira::VertexBuffer::GetView()
{
	return this->_view;
}


