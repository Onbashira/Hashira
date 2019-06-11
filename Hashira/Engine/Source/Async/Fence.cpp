#include "Fence.h"
#include "Engine/Source/CommandQueue/CommandQueue.h"
#include "Engine/Source/CoreSystem/Framework.h"

Hashira::Fence::Fence() :_fence(),_fenceValue(), _fenceEvent(nullptr)
{
}


Hashira::Fence::~Fence()
{
	Discard();
}


HRESULT Hashira::Fence::Initialize(UINT64 initialFenceValue, D3D12_FENCE_FLAGS flags)
{
	auto hresult = Hashira::Framework::GetInstance().GetDevice()->GetDevice()->CreateFence(initialFenceValue,flags,IID_PPV_ARGS(&_fence));
	if (FAILED(hresult)) {
		return hresult;
	}
	_fenceValue = initialFenceValue;
	_fenceEvent = CreateEventEx(nullptr, FALSE, FALSE, EVENT_ALL_ACCESS);
	if (_fenceEvent == nullptr) {
		return ERROR_INVALID_HANDLE;
	}
	return hresult;
}

void Hashira::Fence::Discard()
{
	CloseHandle(_fenceEvent);
	_fenceEvent = nullptr;
	_fenceValue = 0;
	_fence.Reset();
}

Microsoft::WRL::ComPtr<ID3D12Fence> Hashira::Fence::GetFence()
{
	return _fence;
}

UINT64 Hashira::Fence::GetFenceValue()
{
	return _fenceValue;
}

void * Hashira::Fence::GetFenceEvent()
{
	return _fenceEvent;
}

HRESULT Hashira::Fence::Wait(CommandQueue * commandQueue)
{
	const UINT64 fence = _fenceValue;
	auto hr = commandQueue->GetQueue()->Signal(_fence.Get(), fence);
	if (FAILED(hr))
		return hr;
	_fenceValue++;

	// Š®—¹‚ð‘Ò‹@.
	if (_fence->GetCompletedValue() < fence)
	{
		hr = _fence->SetEventOnCompletion(fence, _fenceEvent);
		if (FAILED(hr))
			return hr;
		WaitForSingleObject(_fenceEvent, INFINITE);
	}
	return S_OK;
}
