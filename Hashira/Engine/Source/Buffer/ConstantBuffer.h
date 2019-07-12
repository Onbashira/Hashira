#pragma once
#include "Engine/Source/Utility/D3D12Common.h"
#include "Buffer.h"

namespace Hashira {

	class DescriptorHeap;

	class ConstantBuffer : public Buffer
	{

	public:

	private:

		unsigned long long _initializeSize;

	public:

		ConstantBuffer();

		~ConstantBuffer();

		virtual HRESULT Initialize(ULONG64 size);

		HRESULT	CreateView(D3D12_CONSTANT_BUFFER_VIEW_DESC& cvDesc, D3D12_CPU_DESCRIPTOR_HANDLE cpuDescriptorHandle);

		unsigned long long GetInitializeSize();

	private:


	};
}