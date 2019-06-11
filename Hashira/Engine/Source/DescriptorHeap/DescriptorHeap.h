#pragma once
#include"Engine/Source/Utility/D3D12Common.h"
#include <string>

namespace Hashira {

	class D3D12Device;

	class DescriptorHeap
	{

	public:

	private:

		//！CPUアドレス開始地点
		D3D12_CPU_DESCRIPTOR_HANDLE						_cpuHandle;

		//！CPUアドレス開始地点
		D3D12_GPU_DESCRIPTOR_HANDLE						_gpuHandle;

		//!タイプ
		UINT											_type;

		//！インクリメントサイズ
		UINT											_incrementSize;

		//！ヒープデスクリプション
		D3D12_DESCRIPTOR_HEAP_DESC						_heapDesc;

		//!ヒープ
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>	_heap;

		//！ヒープ名
		std::string										_heapName;

	public:
		DescriptorHeap();

		~DescriptorHeap();

		/**
		* @fn
		* @brief CPUハンドルのフェッチ
		* @param[in] index ビューインデックス
		* @return CPUハンドル
		*/
		D3D12_CPU_DESCRIPTOR_HANDLE						GetCPUHandle(UINT indexCount)const;

		/**
		* @fn
		* @brief GPUハンドルのフェッチ
		* @param[in] index ビューインデックス
		* @return GPUハンドル
		*/
		D3D12_GPU_DESCRIPTOR_HANDLE						GetGPUHandle(UINT indexCount)const;

		/**
		* @fn
		* @brief インクリメントサイズのフェッチ
		* @return ヒープインクリメントサイズ
		*/
		UINT											GetIncrementSize()const;

		/**
		* @fn
		* @brief ヒープのフェッチ
		* @return ヒープの参照
		*/
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>&	GetHeap();

		/**
		* @fn
		* @brief ヒープポインタのフェッチ
		* @return ヒープのポインタ
		*/
		ID3D12DescriptorHeap* 							GetPtr()const;

		/**
		* @fn
		* @brief 情報のフェッチ
		* @return 情報
		*/
		const D3D12_DESCRIPTOR_HEAP_DESC&				GetDesc()const;

		/**
		* @fn
		* @brief ヒープポインタのフェッチ
		* @return ヒープのポインタ
		*/
		ID3D12DescriptorHeap*							operator ->();

		/**
		* @fn
		* @brief ヒープの作成
		* @param[in] pDesc デスクリプションへのポインタ
		* @return リザルト　S_OKで成功
		*/
		HRESULT											Initialize(D3D12_DESCRIPTOR_HEAP_DESC* pDesc);

		/**
		* @fn
		* @brief ヒープの作成
		* @param[in] device 作成に使用するデバイス
		* @param[in] pDesc デスクリプションへのポインタ
		* @return リザルト　S_OKで成功
		*/
		HRESULT											Initialize(std::shared_ptr<D3D12Device>& device,D3D12_DESCRIPTOR_HEAP_DESC* pDesc);

		/**
		* @fn
		* @brief ヒープの作成
		* @param[in] type ヒープのタイプ
		* @param[in] numDescriptors デスクリプタの数
		* @param[in] nodeMask ノードマスク
		* @return リザルト　S_OKで成功
		*/
		HRESULT											Initialize(D3D12_DESCRIPTOR_HEAP_TYPE type, unsigned int numDescriptors, unsigned int nodeMask = 0);

		/**
		* @fn
		* @brief ヒープの作成
		* @param[in] device 作成に使用するデバイス
		* @param[in] type ヒープのタイプ
		* @param[in] numDescriptors デスクリプタの数
		* @param[in] nodeMask ノードマスク
		* @return リザルト　S_OKで成功
		*/
		HRESULT											Initialize(std::shared_ptr<D3D12Device>& device, D3D12_DESCRIPTOR_HEAP_TYPE type, unsigned int numDescriptors, unsigned int nodeMask = 0);

		/**
		* @fn
		* @brief ヒープの名前の設定
		* @param[in] name 名前
		* @return リザルト　S_OKで成功
		*/
		void											SetName(std::string name);

		/**
		* @fn
		* @brief ヒープの破棄
		*/
		void											Discard();

	private:
	};

}
