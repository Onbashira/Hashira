#pragma once
#include "Engine/Source/Utility/D3D12Common.h"
#include "Resource.h"

namespace Hashira {
	class IndexBuffer : public Resource
	{
		D3D12_INDEX_BUFFER_VIEW _view;

		ULONG64 _size;

		UINT64 _elementSize;

	public:

		IndexBuffer();

		~IndexBuffer();

		virtual HRESULT Initialize(ULONG64 size, DXGI_FORMAT format, const void * pIndices);

		virtual HRESULT Initialize(ULONG64 size, size_t elementSize, const void * pIndices);

		void Discard();

		UINT64 GetElementSize() { return _elementSize; };

		size_t GetElementCount() { return (static_cast<size_t>(_size) / static_cast<size_t>(_elementSize)); };

		D3D12_INDEX_BUFFER_VIEW GetView()const;

	private:



	};
}