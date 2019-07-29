#include "SwapChain.h"
#include "Engine/Source/CoreSystem/Framework.h"
#include "Engine/Source/Factory/Factory.h"
#include "Engine/Source/CommandList/CommandList.h"
#include "Engine/Source/CommandQueue/CommandQueue.h"
#include "Engine/Source/Window/Window.h"
#include "Engine/Source/Buffer/Buffer.h"


constexpr float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

Hashira::SwapChain::SwapChain()
{
}


Hashira::SwapChain::~SwapChain()
{
	Discard();
}


HRESULT Hashira::SwapChain::CreateSwapChain(CommandQueue & commandQueue, std::shared_ptr<D3D12Device>& device, Factory & factory, Window & window, UINT windowWidth, UINT windowHeight, unsigned int bufferNum, DXGI_FORMAT targetFormat)
{

	_width = windowWidth;
	_height = windowHeight;

	DXGI_SWAP_CHAIN_DESC1	swapChainDesc = {};
	swapChainDesc.BufferCount = bufferNum;
	swapChainDesc.Width = windowWidth;
	swapChainDesc.Height = windowHeight;
	swapChainDesc.Format = targetFormat;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.SampleDesc.Count = 1;

	Microsoft::WRL::ComPtr<IDXGISwapChain1>	swapChain;
	if (FAILED(factory.GetFactory()->CreateSwapChainForHwnd(commandQueue.GetQueue().Get(), window.GetWindowHandle(), &swapChainDesc, nullptr, nullptr, &swapChain)))
		return FALSE;
	// フルスクリーンのサポートなし
	if (FAILED(factory.GetFactory()->MakeWindowAssociation(window.GetWindowHandle(), DXGI_MWA_NO_ALT_ENTER)))
		return FALSE;

	if (FAILED(swapChain.As(&_swapChain)))
		return FALSE;

	_currentIndex = _swapChain->GetCurrentBackBufferIndex();

	return S_OK;
}

HRESULT Hashira::SwapChain::CreateRenderTargets(std::shared_ptr<D3D12Device>& device, unsigned int bufferNum)
{
	this->_bufferNum = Min(SwapChain::FrameBufferMax, Max(SwapChain::FrameBufferMax, bufferNum));;
	D3D12_DESCRIPTOR_HEAP_DESC desc{};
	desc.NumDescriptors = _bufferNum;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	_rtResource.resize(_bufferNum);

	_localRtHeap = std::make_unique<DescriptorAllocator>();
	_allocatedDesc.clear();
	_allocatedDesc.resize(_bufferNum);
	//レンダーターゲットビュー用のヒープの作成
	{
		if (FAILED(_localRtHeap->Initialize(device, desc)))
			return E_FAIL;
	}
	//レンダーターゲットの作成
	{
		for (UINT i = 0; i < _bufferNum; i++) {
			_rtResource[i] = Buffer::CreateShared();
			//ディスプレイバッファの取得
			if (FAILED(_swapChain->GetBuffer(i, IID_PPV_ARGS(this->_rtResource[i]->GetResource().GetAddressOf()))))
			{
				Util::Comment(L"バックバッファの作成に失敗しました　エラーログを見てね");

				return E_FAIL;
			}
			//レンダーターゲットビューの作成
			_allocatedDesc[i] = (_localRtHeap->Allocate());
			device->GetDevice()->CreateRenderTargetView(_rtResource[i]->GetResource().Get(), nullptr, _allocatedDesc[i].cpuHandle);
			
			if (_allocatedDesc[i].IsValid()) {
				return E_FAIL;
			}
			
			_rtResource[i]->SetResourceState(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COMMON);
			
			std::stringstream ss;
			ss << "RenderTargetResource " << i;
			_rtResource[i]->SetName(ss.str());
			

		}
	}
	return S_OK;
}

HRESULT Hashira::SwapChain::Initialize(CommandQueue & commandQueue, std::shared_ptr<D3D12Device>& device, Factory & factory, Window & window, UINT windowWidth, UINT windowHeight, unsigned int bufferNum, DXGI_FORMAT targetFormat)
{
	auto hr = CreateSwapChain(commandQueue, device, factory, window, windowWidth, windowHeight, bufferNum, targetFormat);
	CHECK_RESULT(hr);
	hr = CreateRenderTargets(device, bufferNum);
	CHECK_RESULT(hr);
	return hr;
}

unsigned int Hashira::SwapChain::GetBufferNum()
{
	return _bufferNum;
}

unsigned int Hashira::SwapChain::GetCurrentBuckBuffer()
{
	return _currentIndex;
}

std::unique_ptr<Hashira::DescriptorAllocator>& Hashira::SwapChain::GetLocalRtDescriptortAlolocator()
{
	return _localRtHeap;

}

HRESULT Hashira::SwapChain::SetStatePresent(std::shared_ptr<CommandList> list)
{
	auto hr = _rtResource[_currentIndex]->ResourceTransition(list, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT);

	CHECK_RESULT(hr);
	return hr;
}

HRESULT Hashira::SwapChain::SetStateRenderTarget(std::shared_ptr<CommandList> list)
{
	auto hr = _rtResource[_currentIndex]->ResourceTransition(list, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET);
	CHECK_RESULT(hr);
	return hr;
}

HRESULT Hashira::SwapChain::SetStateCopyDest(std::shared_ptr<CommandList> list)
{
	auto hr = _rtResource[_currentIndex]->ResourceTransition(list, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST);
	CHECK_RESULT(hr);
	return hr;
}

HRESULT Hashira::SwapChain::SetStateGenericRead(std::shared_ptr<CommandList> list)
{
	auto hr = _rtResource[_currentIndex]->ResourceTransition(list, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ);
	CHECK_RESULT(hr);
	return hr;
}

HRESULT Hashira::SwapChain::CopyToRenderTarget(std::shared_ptr<CommandList> list, Buffer* pSrc)
{
	list->GetCommandList()->CopyResource(this->_rtResource[_currentIndex]->GetResource().Get(), pSrc->GetResource().Get());
	return S_OK;
}

void Hashira::SwapChain::SetRenderTarget(std::shared_ptr<CommandList> list, D3D12_CPU_DESCRIPTOR_HANDLE* depthHandle)
{
	list->GetCommandList()->OMSetRenderTargets(1, &_allocatedDesc[_currentIndex].cpuHandle, true, depthHandle);

}

void Hashira::SwapChain::ClearScreen(std::shared_ptr<CommandList> list)
{
	//リソースステートをRTにバリアを張る
	SetStateRenderTarget(list);
	//testCode CleacolorChange
	float tempColor[4] = { 0.5f,0.5f,0.5f,1.0f };

	list->GetCommandList()->ClearRenderTargetView(_allocatedDesc[_currentIndex].cpuHandle, tempColor, 0, nullptr);


}

void Hashira::SwapChain::ReSizeRenderTarget(std::shared_ptr<D3D12Device>& device, unsigned int width, unsigned int height, unsigned int backBufferNum)
{

	for (auto& ref : _rtResource) {
		ref->Discard();
	}

	if (width > 0 && height > 0) {
		_width = width;
		_height = height;
		_bufferNum = Min(SwapChain::FrameBufferMax,Max(SwapChain::FrameBufferMax,backBufferNum));
	}
	else
	{
		Util::Comment(L"スクリーンバッファのリサイズに失敗　サイズは0以上である必要があります");
		assert(0);
	}


	_swapChain->ResizeBuffers(
		_bufferNum,
		_width,
		_height,
		DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
	);
	this->_bufferNum = Min(SwapChain::FrameBufferMax, Max(SwapChain::FrameBufferMax, _bufferNum));;
	D3D12_DESCRIPTOR_HEAP_DESC desc{};
	desc.NumDescriptors = _bufferNum;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	
	_localRtHeap->Discard();
	_localRtHeap->Initialize(device, desc);
	_rtResource.resize(this->_bufferNum);

	//レンダーターゲットの作成
	{
		for (UINT i = 0; i < _bufferNum; i++) {
			//_rtResource[i] = Buffer::CreateShared();
			//ディスプレイバッファの取得
			if (FAILED(_swapChain->GetBuffer(i, IID_PPV_ARGS(this->_rtResource[i]->GetResource().GetAddressOf()))))
			{
				Util::Comment(L"バックバッファの作成に失敗しました　エラーログを見てね");
				assert(0);

			}
			//レンダーターゲットビューの作成
			_allocatedDesc.push_back(_localRtHeap->Allocate());
			auto&& descInfo = _allocatedDesc.back();
			device->GetDevice()->CreateRenderTargetView(_rtResource[i]->GetResource().Get(), nullptr, descInfo.cpuHandle);
			_rtResource[i]->SetResourceState(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT);
			_rtResource[i]->SetName(std::string("RenderTargetResource " + i));
		}
	}

}

void Hashira::SwapChain::FlipScreen()
{
	_currentIndex = _swapChain->GetCurrentBackBufferIndex();;
}

HRESULT Hashira::SwapChain::Present(unsigned int sysncInterval, unsigned int flags)
{
	auto hr = _swapChain->Present(sysncInterval, flags);
	CHECK_RESULT(hr);
	//FlipScreen();
	return hr;
}

void Hashira::SwapChain::Discard()
{
	_swapChain.Reset();
	for (auto& res : _rtResource) {
		res->Discard();
	}

	_localRtHeap->Discard();
}