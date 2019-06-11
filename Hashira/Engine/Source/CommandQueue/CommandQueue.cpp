#include "CommandQueue.h"
#include "Engine/Source/CoreSystem/Framework.h"
#include "Engine/Source/Utility/Utility.h"
#include "Engine/Source/Debug/Logger.h"

Hashira::CommandQueue::CommandQueue() : _3DQueue()
{
}


Hashira::CommandQueue::~CommandQueue()
{
	Discard();
}

HRESULT Hashira::CommandQueue::Initialize(D3D12_COMMAND_QUEUE_DESC& desc)
{
	CHECK_RESULT(Hashira::Framework::GetInstance().GetDevice()->GetDevice()->CreateCommandQueue(&desc, IID_PPV_ARGS(&_3DQueue)));

	return S_OK;
}

HRESULT Hashira::CommandQueue::Initialize(std::weak_ptr<D3D12Device> device, D3D12_COMMAND_QUEUE_DESC & desc)
{
	desc.Type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT;
	CHECK_RESULT(device.lock()->GetDevice()->CreateCommandQueue(&desc, IID_PPV_ARGS(&_3DQueue)));
	desc.Type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_COPY;
	CHECK_RESULT(device.lock()->GetDevice()->CreateCommandQueue(&desc, IID_PPV_ARGS(&_copyQueue)));
	desc.Type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_COMPUTE;
	CHECK_RESULT(device.lock()->GetDevice()->CreateCommandQueue(&desc, IID_PPV_ARGS(&_computeQueue)));
	return S_OK;
}

Microsoft::WRL::ComPtr<ID3D12CommandQueue>& Hashira::CommandQueue::GetQueue()
{
	return _3DQueue;
}

Microsoft::WRL::ComPtr<ID3D12CommandQueue>& Hashira::CommandQueue::GetCopyQueue()
{
	return _copyQueue;
}

Microsoft::WRL::ComPtr<ID3D12CommandQueue>& Hashira::CommandQueue::GetComputeQueue()
{
	return _computeQueue;
}


UINT64 Hashira::CommandQueue::GetTimestampFrequency()
{
	if (_3DQueue == nullptr) {
		return 1;
	}

	UINT64 ret;
	auto hr = _3DQueue->GetTimestampFrequency(&ret);
	if (FAILED(hr)) {
		return 1;
	}

	return ret;
}

D3D12_COMMAND_QUEUE_DESC Hashira::CommandQueue::GetDesc()
{
	return _3DQueue->GetDesc();
}

void Hashira::CommandQueue::ExecuteCommandLists(std::vector<std::shared_ptr<CommandList>>& lists)
{
	Execute(lists, _3DQueue);
	SystemLogger::GetInstance().Log(Hashira::LOG_LEVEL::Details, std::string("Execute 3DQueue\n"));


}

void Hashira::CommandQueue::ExecuteCopyCommands(std::vector<std::shared_ptr<CommandList>>& lists)
{
	Execute(lists, _copyQueue);
	//DETAILS_LOG(String("Execute CopyQueue"));

}

void Hashira::CommandQueue::ExecuteComputeCommands(std::vector<std::shared_ptr<CommandList>>& lists)
{
	Execute(lists, _computeQueue);
	//DETAILS_LOG(String("Execute ComputeQueue"));

}

void Hashira::CommandQueue::SetName(std::string name)
{
	this->_3DQueue->SetName(Util::StringToWString(name + "_3DQueue").c_str());
	this->_copyQueue->SetName(Util::StringToWString(name + "_CopyQueue").c_str());
	this->_computeQueue->SetName(Util::StringToWString(name + "_ComputeQueue").c_str());

}

void Hashira::CommandQueue::Discard()
{
	if (_3DQueue.Get() != nullptr) {
		_3DQueue.Reset();
		_copyQueue.Reset();
		_computeQueue.Reset();

		DEBUG_LOG(std::string(_name + " is Reset"));
	}
}

void Hashira::CommandQueue::Execute(std::vector<std::shared_ptr<CommandList>>& lists, Microsoft::WRL::ComPtr<ID3D12CommandQueue>& queue)
{
	std::vector < ID3D12CommandList* > rawLists(lists.size());
	auto ptr = rawLists.data();
	for (auto& list : lists) {
		*ptr = list->GetCommandList().Get();
		ptr++;
	}
	queue->ExecuteCommandLists(static_cast<unsigned int>(lists.size()), rawLists.data());
}