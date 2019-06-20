#pragma once
#include "Resource.h"

namespace Hashira {
	class L0Buffer :
		public Resource
	{
	public:

	private:

	public:

		L0Buffer();

		virtual ~L0Buffer();

		HRESULT Initialize(std::shared_ptr<D3D12Device>& device, size_t bufferSize, const D3D12_RESOURCE_STATES& state, D3D12_CLEAR_VALUE* clearValue = nullptr);

	};

}