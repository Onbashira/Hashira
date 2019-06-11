#pragma once
#include "Engine/Source/Utility/D3D12Common.h"
#include "Engine/Source/DescriptorHeap/DescriptorHeap.h"
#include "Resource.h"

namespace Hashira {

	class CommandList;
	class CommandQueue;

	//UAVはDXGI_FORMATはUNKNWON固定
	class UnorderedAccessValue : public Hashira::Resource
	{
	public:
		enum BUFFER_MODE {
			UAV_BUFFER_MODE_APPEND,
			UAV_BUFFER_MODE_CONSUME

		};

	private:
		//Heapのオフセット値
		enum HEAP_OFFSET
		{
			UAV_DESCRIPTOR_OFFSET = 0,
			SRV_DESCRIPTOR_OFFSET = 1,
			HEAP_OFFSET_MAX,
		};
		//ステージング用のメモリ
		std::unique_ptr<Resource> _stagingResource;
		//GPUMemにアップロードするためのメモリ　（リードバック用のメモリは継承元のリソース
		std::unique_ptr<Resource> _uploadResource;

		D3D12_RANGE _readRange;

		DescriptorHeap _heap;

		D3D12_UNORDERED_ACCESS_VIEW_DESC _unorderedAccessViewDesc;

		D3D12_SHADER_RESOURCE_VIEW_DESC _shaderResourceViewDesc;

	public:

	private:

		HRESULT CreateHeap(unsigned numElements, unsigned int nodeMask = 0);

	public:

		UnorderedAccessValue();

		virtual~UnorderedAccessValue();
		//リードバックバッファとアップロードバッファを作成。ついでにGPUに配置
		HRESULT Initialize(unsigned int elementSize, unsigned int numElements, void* pBufferData);
		//デスクリプタの作成
		HRESULT CreateDescriptors(unsigned int elementSize, unsigned int numElements);

		HRESULT CreateView(D3D12_UNORDERED_ACCESS_VIEW_DESC* uavDesc, D3D12_CPU_DESCRIPTOR_HANDLE cpuDescriptorHandle);

		HRESULT CreateView(D3D12_SHADER_RESOURCE_VIEW_DESC* srvDesc, D3D12_CPU_DESCRIPTOR_HANDLE cpuDescriptorHandle);
		//コマンドリストを指定してやること。非同期即時復帰でリソースのGPU配置操作を実行
		void AsyncWriteToBuffer(std::weak_ptr<Hashira::CommandList> list, unsigned int numElements, unsigned int elementSize, void* pBufferData, Hashira::CommandQueue* queue = nullptr);
		//コマンドリストを指定してやること。非同期即時復帰でGPU上の情報をリードバック操作を実行
		void AsyncReadBack(std::weak_ptr<Hashira::CommandList> list, Hashira::CommandQueue* queue = nullptr);
		//
		D3D12_CPU_DESCRIPTOR_HANDLE GetSRVCPUHandle();
		//
		D3D12_CPU_DESCRIPTOR_HANDLE GetUAVCPUHandle();
		//
		D3D12_GPU_DESCRIPTOR_HANDLE GetSRVGPUHandle();
		//
		D3D12_GPU_DESCRIPTOR_HANDLE GetUAVGPUHandle();

		void Discard();

		DescriptorHeap* GetHeap();

		D3D12_UNORDERED_ACCESS_VIEW_DESC GetUAVDesc();

		D3D12_SHADER_RESOURCE_VIEW_DESC GetSRVDesc();

	};
}