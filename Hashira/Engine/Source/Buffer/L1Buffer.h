#pragma once
#include "Buffer.h"

namespace Hashira {
	class L1Buffer :
		public Buffer
	{
	public:

	private:

	public:

		L1Buffer();

		virtual ~L1Buffer();

		HRESULT Initialize(std::shared_ptr<D3D12Device>& device, size_t bufferSize, const D3D12_RESOURCE_STATES& state, D3D12_CLEAR_VALUE* clearValue = nullptr);
	

	};

}