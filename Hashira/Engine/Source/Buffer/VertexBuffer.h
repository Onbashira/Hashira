#pragma once
#include "Engine/Source/Utility/D3D12Common.h"
#include "Buffer.h"

namespace Hashira {

	class VertexBuffer : public Buffer
	{

	public:

	private:
		D3D12_VERTEX_BUFFER_VIEW _view;

		ULONG64 _size;

		UINT _stride;



	public:

		VertexBuffer();

		VertexBuffer(const VertexBuffer& other);

		VertexBuffer(VertexBuffer&& value);

		VertexBuffer& operator =(const VertexBuffer& value);

		VertexBuffer& operator =(VertexBuffer&& value);

		~VertexBuffer();

		virtual HRESULT Initialize(ULONG64 size, UINT stride, const void* pVertices);

		void Discard();

		D3D12_VERTEX_BUFFER_VIEW& GetView();

	private:


		//‰B‚·
		HRESULT	Initialize(const D3D12_HEAP_PROPERTIES& heapProps, const D3D12_HEAP_FLAGS& flags, const D3D12_RESOURCE_DESC& resourceDesc, const D3D12_RESOURCE_STATES& state, D3D12_CLEAR_VALUE* clearValue = nullptr)override final { return E_FAIL; };

	};
}