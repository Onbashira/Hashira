#include "CommandList.h"
#include "Engine/Source/Debug/Logger.h"
#include "Engine/Source/CommandList/CommandListManager.h"
#include "Engine/Source/Utility/Utility.h"
#include "Engine/Source/CoreSystem/Framework.h"
#include "Engine/Source/PipelineState/PipelineStateObject.h"
#include "Engine/Source/DescriptorHeap/DescriptorHeap.h"
#include "Engine/Source/Signature/RootSignature.h"
#include "Engine/Source/Signature/CommandSignature.h"
#include "Engine/Source/Device/D3D12Device.h"
#include "Engine/Source/CommandAllocator/CommandAllocator.h"
#include "Engine/Source/Resource/Resource.h"

Hashira::CommandList::CommandList() :
	_commandList(),
	_commandListName("UnNamed_CommandList")
{

}


Hashira::CommandList::~CommandList()
{
	Discard();
}


HRESULT Hashira::CommandList::Initialize(std::weak_ptr<D3D12Device> device, unsigned int nodeMask, D3D12_COMMAND_LIST_TYPE listType, std::shared_ptr<CommandAllocator>& commandAllocator)
{
	HRESULT result;
	if (commandAllocator->GetAllocator().Get() == nullptr) {
		return E_ACCESSDENIED;
	}
	_listType = listType;
	result = device.lock()->GetDevice()->CreateCommandList(nodeMask, _listType, commandAllocator->GetAllocator().Get(), nullptr, IID_PPV_ARGS(&_commandList));
	if (result != S_OK) {
		return result;
	}
	return result;
}

HRESULT Hashira::CommandList::SetResourceBarrie(ID3D12Resource * resource, D3D12_RESOURCE_STATES beforeState, D3D12_RESOURCE_STATES afterState)
{
	D3D12_RESOURCE_BARRIER resource_barrier{};

	resource_barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	resource_barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	resource_barrier.Transition.pResource = resource;
	resource_barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	resource_barrier.Transition.StateBefore = beforeState;
	resource_barrier.Transition.StateAfter = afterState;

	_commandList->ResourceBarrier(1, &resource_barrier);
	return S_OK;
}

HRESULT Hashira::CommandList::SetResourceBarrie(Resource * resource, D3D12_RESOURCE_STATES beforeState, D3D12_RESOURCE_STATES afterState)
{
	D3D12_RESOURCE_BARRIER resource_barrier{};

	resource_barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	resource_barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	resource_barrier.Transition.pResource = resource->GetResource().Get();
	resource_barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	resource_barrier.Transition.StateBefore = beforeState;
	resource_barrier.Transition.StateAfter = afterState;

	_commandList->ResourceBarrier(1, &resource_barrier);
	return S_OK;
}

Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2>& Hashira::CommandList::GetCommandList()
{
	return this->_commandList;
}

HRESULT Hashira::CommandList::ResetCommandList(std::shared_ptr<CommandAllocator>& allocator, ID3D12PipelineState * pInitialState)
{
	auto hr = _commandList->Reset(allocator->GetAllocator().Get(), pInitialState);
	return hr;
}

HRESULT Hashira::CommandList::CloseCommandList()
{
	auto hr = _commandList->Close();
	return hr;
}

void Hashira::CommandList::Discard()
{

	if (_commandList.Get() != nullptr) {

		this->_commandList.Reset();
		DEBUG_LOG(std::string("CommandList : " + _commandListName + " is Reset"));

	}

}

void Hashira::CommandList::SetName(std::string objectName)
{
	_commandListName = objectName + "List";
	this->_commandList->SetName(Util::StringToWString(_commandListName).c_str());
}

void Hashira::CommandList::BeginEvent(unsigned int metadata, const void * pData, unsigned int size)
{
	this->_commandList->BeginEvent(metadata, pData, size);
}

void Hashira::CommandList::BeginQuery(ID3D12QueryHeap * pQueryHeap, D3D12_QUERY_TYPE Type, unsigned int Index)
{
	this->_commandList->BeginQuery(pQueryHeap, Type, Index);

}

void Hashira::CommandList::ClearDepthStencilView(D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView, D3D12_CLEAR_FLAGS ClearFlags, float Depth, UINT8 Stencil, unsigned int NumRects, const D3D12_RECT * pRects)
{
	this->_commandList->ClearDepthStencilView(DepthStencilView, ClearFlags, Depth, Stencil, NumRects, pRects);
}

void Hashira::CommandList::ClearRenderTargetView(D3D12_CPU_DESCRIPTOR_HANDLE RenderTargetView, const float ColorRGBA[4], unsigned int NumRects, const D3D12_RECT * pRects)
{
	this->_commandList->ClearRenderTargetView(RenderTargetView, ColorRGBA, NumRects, pRects);
}

void Hashira::CommandList::ClearState(std::shared_ptr<PipelineStateObject> pPipelineState)
{
	this->_commandList->ClearState(pPipelineState->GetPSO().Get());
}

void Hashira::CommandList::ClearUnorderedAccessViewFloat(D3D12_GPU_DESCRIPTOR_HANDLE ViewGPUHandleInCurrentHeap, D3D12_CPU_DESCRIPTOR_HANDLE ViewCPUHandle, Resource * pResource, const float Values[4], unsigned int NumRects, const D3D12_RECT * pRects)
{
	this->_commandList->ClearUnorderedAccessViewFloat(ViewGPUHandleInCurrentHeap, ViewCPUHandle, pResource->GetResource().Get(), Values, NumRects, pRects);
}

void Hashira::CommandList::ClearUnorderedAccessViewUint(D3D12_GPU_DESCRIPTOR_HANDLE ViewGPUHandleInCurrentHeap, D3D12_CPU_DESCRIPTOR_HANDLE ViewCPUHandle, Resource * pResource, const unsigned int Values[4], unsigned int NumRects, const D3D12_RECT * pRects)
{
	this->_commandList->ClearUnorderedAccessViewUint(ViewGPUHandleInCurrentHeap, ViewCPUHandle, pResource->GetResource().Get(), Values, NumRects, pRects);
}

void Hashira::CommandList::CopyBufferRegion(Resource * pDstBuffer, UINT64 DstOffset, Resource * pSrcBuffer, UINT64 SrcOffset, UINT64 NumBytes)
{
	this->_commandList->CopyBufferRegion(pDstBuffer->GetResource().Get(), DstOffset, pSrcBuffer->GetResource().Get(), SrcOffset, NumBytes);
}

void Hashira::CommandList::CopyResource(Resource * pDstResource, Resource * pSrcResource)
{
	this->_commandList->CopyResource(pDstResource->GetResource().Get(), pSrcResource->GetResource().Get());
}

void Hashira::CommandList::CopyTextureRegion(const D3D12_TEXTURE_COPY_LOCATION * pDst, unsigned int DstX, unsigned int DstY, unsigned int DstZ, const D3D12_TEXTURE_COPY_LOCATION * pSrc, const D3D12_BOX * pSrcBox)
{
	this->_commandList->CopyTextureRegion(pDst, DstX, DstY, DstZ, pSrc, pSrcBox);

}

void Hashira::CommandList::CopyTiles(Resource * pTiledResource, const D3D12_TILED_RESOURCE_COORDINATE * pTileRegionStartCoordinate, const D3D12_TILE_REGION_SIZE * pTileRegionSize, Resource * pBuffer, UINT64 BufferStartOffsetInBytes, D3D12_TILE_COPY_FLAGS Flags)
{
	this->_commandList->CopyTiles(pTiledResource->GetResource().Get(), pTileRegionStartCoordinate, pTileRegionSize, pBuffer->GetResource().Get(), BufferStartOffsetInBytes, Flags);

}

void Hashira::CommandList::DiscardResource(Resource * pResource, const D3D12_DISCARD_REGION * pRegion)
{
	this->_commandList->DiscardResource(pResource->GetResource().Get(), pRegion);

}

void Hashira::CommandList::Dispatch(unsigned int ThreadGroupCountX, unsigned int ThreadGroupCountY, unsigned int ThreadGroupCountZ)
{
	this->_commandList->Dispatch(ThreadGroupCountX, ThreadGroupCountY, ThreadGroupCountZ);

}

void Hashira::CommandList::DrawIndexedInstanced(unsigned int IndexCountPerInstance, unsigned int InstanceCount, unsigned int StartIndexLocation, int BaseVertexLocation, unsigned int StartInstanceLocation)
{
	this->_commandList->DrawIndexedInstanced(IndexCountPerInstance, InstanceCount, StartIndexLocation, BaseVertexLocation, StartInstanceLocation);

}

void Hashira::CommandList::DrawInstanced(unsigned int VertexCountPerInstance, unsigned int InstanceCount, unsigned int StartVertexLocation, unsigned int StartInstanceLocation)
{
	this->_commandList->DrawInstanced(VertexCountPerInstance, InstanceCount, StartVertexLocation, StartInstanceLocation);

}

void Hashira::CommandList::EndEvent()
{
	this->_commandList->EndEvent();

}

void Hashira::CommandList::EndQuery(ID3D12QueryHeap * pQueryHeap, D3D12_QUERY_TYPE Type, unsigned int Index)
{
	this->_commandList->EndQuery(pQueryHeap, Type, Index);
}

void Hashira::CommandList::ExecuteBundle(CommandList * bundle)
{
	this->_commandList->ExecuteBundle(bundle->_commandList.Get());

}

void Hashira::CommandList::ExecuteIndirect(ID3D12CommandSignature * pCommandSignature, unsigned int MaxCommandCount, Resource * pArgumentBuffer, UINT64 ArgumentBufferOffset, Resource * pCountBuffer, UINT64 CountBufferOffset)
{
	this->_commandList->ExecuteIndirect(pCommandSignature, MaxCommandCount, pArgumentBuffer->GetResource().Get(), ArgumentBufferOffset,
		pCountBuffer->GetResource().Get(), CountBufferOffset);

}

void Hashira::CommandList::IASetIndexBuffer(const D3D12_INDEX_BUFFER_VIEW * pView)
{
	this->_commandList->IASetIndexBuffer(pView);
}

void Hashira::CommandList::IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY PrimitiveTopology)
{
	this->_commandList->IASetPrimitiveTopology(PrimitiveTopology);

}

void Hashira::CommandList::IASetVertexBuffers(unsigned int StartSlot, unsigned int NumViews, const D3D12_VERTEX_BUFFER_VIEW * pViews)
{
	this->_commandList->IASetVertexBuffers(StartSlot, NumViews, pViews);

}

void Hashira::CommandList::OMSetBlendFactor(const float BlendFactor[4])
{
	this->_commandList->OMSetBlendFactor(BlendFactor);

}

void Hashira::CommandList::OMSetRenderTargets(unsigned int NumRenderTargetDescriptors, const D3D12_CPU_DESCRIPTOR_HANDLE * pRenderTargetDescriptors, BOOL RTsSingleHandleToDescriptorRange, const D3D12_CPU_DESCRIPTOR_HANDLE * pDepthStencilDescriptor)
{
	this->_commandList->OMSetRenderTargets(NumRenderTargetDescriptors, pRenderTargetDescriptors, RTsSingleHandleToDescriptorRange, pDepthStencilDescriptor);
}

void Hashira::CommandList::OMSetStencilRef(unsigned int StencilRef)
{
	this->_commandList->OMSetStencilRef(StencilRef);
}

void Hashira::CommandList::ResolveQueryData(ID3D12QueryHeap * pQueryHeap, D3D12_QUERY_TYPE Type, unsigned int StartIndex, unsigned int NumQueries, Resource * pDestinationBuffer, UINT64 AlignedDestinationBufferOffset)
{
	this->_commandList->ResolveQueryData(pQueryHeap, Type, StartIndex, NumQueries, pDestinationBuffer->GetResource().Get(), AlignedDestinationBufferOffset);

}

void Hashira::CommandList::ResolveSubresource(Resource * pDstResource, unsigned int DstSubresource, Resource * pSrcResource, unsigned int SrcSubresource, DXGI_FORMAT Format)
{
	this->_commandList->ResolveSubresource(pDstResource->GetResource().Get(), DstSubresource, pSrcResource->GetResource().Get(), SrcSubresource, Format);

}

void Hashira::CommandList::ResourceBarrier(unsigned int NumBarriers, const D3D12_RESOURCE_BARRIER * pBarriers)
{
	this->_commandList->ResourceBarrier(NumBarriers, pBarriers);

}

void Hashira::CommandList::RSSetScissorRects(unsigned int NumRects, const D3D12_RECT * pRects)
{
	this->_commandList->RSSetScissorRects(NumRects, pRects);

}

void Hashira::CommandList::RSSetViewports(unsigned int NumViewports, const D3D12_VIEWPORT * pViewports)
{
	this->_commandList->RSSetViewports(NumViewports, pViewports);

}

void Hashira::CommandList::SetComputeRoot32BitConstant(unsigned int RootParameterIndex, unsigned int SrcData, unsigned int DestOffsetIn32BitValues)
{
	this->_commandList->SetComputeRoot32BitConstant(RootParameterIndex, SrcData, DestOffsetIn32BitValues);

}

void Hashira::CommandList::SetComputeRoot32BitConstants(unsigned int RootParameterIndex, unsigned int Num32BitValuesToSet, const void * pSrcData, unsigned int DestOffsetIn32BitValues)
{
	this->_commandList->SetComputeRoot32BitConstants(RootParameterIndex, Num32BitValuesToSet, pSrcData, DestOffsetIn32BitValues);

}

void Hashira::CommandList::SetComputeRootConstantBufferView(unsigned int RootParameterIndex, D3D12_GPU_VIRTUAL_ADDRESS BufferLocation)
{
	this->_commandList->SetComputeRootConstantBufferView(RootParameterIndex, BufferLocation);

}

void Hashira::CommandList::SetComputeRootDescriptorTable(unsigned int RootParameterIndex, D3D12_GPU_DESCRIPTOR_HANDLE BaseDescriptor)
{
	this->_commandList->SetComputeRootDescriptorTable(RootParameterIndex, BaseDescriptor);

}

void Hashira::CommandList::SetComputeRootShaderResourceView(unsigned int RootParameterIndex, D3D12_GPU_VIRTUAL_ADDRESS BufferLocation)
{
	this->_commandList->SetComputeRootShaderResourceView(RootParameterIndex, BufferLocation);

}

void Hashira::CommandList::SetComputeRootSignature(std::shared_ptr<RootSignature> pRootSignature)
{
	this->_commandList->SetComputeRootSignature(pRootSignature->GetSignature().Get());

}

void Hashira::CommandList::SetComputeRootUnorderedAccessView(unsigned int RootParameterIndex, D3D12_GPU_VIRTUAL_ADDRESS BufferLocation)
{
	this->_commandList->SetComputeRootUnorderedAccessView(RootParameterIndex, BufferLocation);

}

void Hashira::CommandList::SetDescriptorHeaps(unsigned int NumDescriptorHeaps, DescriptorHeap * pDescriptorHeaps)
{
	this->_commandList->SetDescriptorHeaps(NumDescriptorHeaps, &pDescriptorHeaps->GetHeap());

}

void Hashira::CommandList::SetGraphicsRoot32BitConstant(unsigned int RootParameterIndex, unsigned int SrcData, unsigned int DestOffsetIn32BitValues)
{
	this->_commandList->SetGraphicsRoot32BitConstant(RootParameterIndex, SrcData, DestOffsetIn32BitValues);

}

void Hashira::CommandList::SetGraphicsRoot32BitConstants(unsigned int RootParameterIndex, unsigned int Num32BitValuesToSet, const void * pSrcData, unsigned int DestOffsetIn32BitValues)
{
	this->_commandList->SetGraphicsRoot32BitConstants(RootParameterIndex, Num32BitValuesToSet, pSrcData, DestOffsetIn32BitValues);

}

void Hashira::CommandList::SetGraphicsRootConstantBufferView(unsigned int RootParameterIndex, D3D12_GPU_VIRTUAL_ADDRESS BufferLocation)
{
	this->_commandList->SetGraphicsRootConstantBufferView(RootParameterIndex, BufferLocation);

}

void Hashira::CommandList::SetGraphicsRootDescriptorTable(unsigned int RootParameterIndex, D3D12_GPU_DESCRIPTOR_HANDLE BaseDescriptor)
{
	this->_commandList->SetGraphicsRootDescriptorTable(RootParameterIndex, BaseDescriptor);

}

void Hashira::CommandList::SetGraphicsRootShaderResourceView(unsigned int RootParameterIndex, D3D12_GPU_VIRTUAL_ADDRESS BufferLocation)
{
	this->_commandList->SetGraphicsRootShaderResourceView(RootParameterIndex, BufferLocation);

}

void Hashira::CommandList::SetGraphicsRootSignature(std::shared_ptr<RootSignature> pRootSignature)
{
	this->_commandList->SetGraphicsRootSignature(pRootSignature->GetSignature().Get());

}

void Hashira::CommandList::SetGraphicsRootUnorderedAccessView(unsigned int RootParameterIndex, D3D12_GPU_VIRTUAL_ADDRESS BufferLocation)
{
	this->_commandList->SetGraphicsRootUnorderedAccessView(RootParameterIndex, BufferLocation);

}

void Hashira::CommandList::SetMarker(unsigned int Metadata, const void * pData, unsigned int Size)
{
	this->_commandList->SetMarker(Metadata, pData, Size);
}

void Hashira::CommandList::SetPipelineState(std::shared_ptr<PipelineStateObject> pPso)
{
	this->_commandList->SetPipelineState(pPso->GetPSO().Get());

}

void Hashira::CommandList::SetPredication(Resource * pBuffer, UINT64 AlignedBufferOffset, D3D12_PREDICATION_OP Operation)
{
	this->_commandList->SetPredication(pBuffer->GetResource().Get(), AlignedBufferOffset, Operation);

}

void Hashira::CommandList::SOSetTargets(unsigned int StartSlot, unsigned int NumViews, const D3D12_STREAM_OUTPUT_BUFFER_VIEW * pViews)
{
	this->_commandList->SOSetTargets(StartSlot, NumViews, pViews);

}
