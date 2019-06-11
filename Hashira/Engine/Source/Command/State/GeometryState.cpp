#include "GeometryState.h"

Hashira::GeometryState::GeometryState() :
	_vertexBuffers(), _indexBuffer()
{

	_indexBuffer.BufferLocation = 0;
	_indexBuffer.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	_indexBuffer.SizeInBytes = 0;

}

Hashira::GeometryState::~GeometryState()
{

}

const std::vector<D3D12_VERTEX_BUFFER_VIEW>* Hashira::GeometryState::GetVertexBufferView()
{
	return &_vertexBuffers;
}

const D3D12_INDEX_BUFFER_VIEW * Hashira::GeometryState::GetIndexBufferView()
{
	return &_indexBuffer;
}

void Hashira::GeometryState::AddVertexBufferView(D3D12_VERTEX_BUFFER_VIEW && vertexBufferView)
{
	this->_vertexBuffers.push_back(std::move(vertexBufferView));

}

void Hashira::GeometryState::AddVertexBufferView(const D3D12_VERTEX_BUFFER_VIEW & vertexBufferView)
{
	this->_vertexBuffers.push_back(vertexBufferView);
}

void Hashira::GeometryState::SetIndexBufferView(D3D12_INDEX_BUFFER_VIEW & indexBufferView)
{
	this->_indexBuffer = indexBufferView;
}
