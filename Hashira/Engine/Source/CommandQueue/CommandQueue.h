#pragma once
#include "Engine/Source/Async/Fence.h"
#include <string>
namespace Hashira {

	class D3D12Device;
	class CommandList;

	class CommandQueue
	{


	public:
	private:

		//!3Dキュー
		Microsoft::WRL::ComPtr<ID3D12CommandQueue>	_3DQueue;

		//!コピーキュー
		Microsoft::WRL::ComPtr<ID3D12CommandQueue>	_copyQueue;

		//!コンピュートキュー
		Microsoft::WRL::ComPtr<ID3D12CommandQueue>	_computeQueue;
				
		//!キューの名前
		std::string _name;

	public:

		CommandQueue();

		~CommandQueue();

		/**
		* @fn
		* @brief キュー作成
		* @param[in] desc キューのデスクリプション
		* @return リザルト　S_OKで成功
		*/
		HRESULT Initialize(D3D12_COMMAND_QUEUE_DESC& desc);
		
		/**
		* @fn
		* @brief キュー作成
		* @param[in] device デバイス
		* @param[in] desc キューのデスクリプション
		* @return リザルト　S_OKで成功
		*/
		HRESULT Initialize(std::weak_ptr<D3D12Device> device,D3D12_COMMAND_QUEUE_DESC& desc);

		/**
		* @fn
		* @brief キューのフェッチ
		* @return キューへの参照
		*/
		Microsoft::WRL::ComPtr<ID3D12CommandQueue>& GetQueue();

		/**
		* @fn
		* @brief COPYキューのフェッチ
		* @return キューへの参照
		*/
		Microsoft::WRL::ComPtr<ID3D12CommandQueue>& GetCopyQueue();
		
		/**
		* @fn
		* @brief COMPUTEキューのフェッチ
		* @return キューへの参照
		*/
		Microsoft::WRL::ComPtr<ID3D12CommandQueue>& GetComputeQueue();

		/**
		* @fn
		* @brief タイムスタンプの取得
		* @return タイムスタンプ
		*/
		UINT64 GetTimestampFrequency();

		/**
		* @fn
		* @brief キューのデスクリプションの取得
		* @return デスクリプション
		*/
		D3D12_COMMAND_QUEUE_DESC GetDesc();

		/**
		* @fn
		* @brief キューのデスクリプションの取得
		* @param[in] lists　コマンドリストベクタへの参照
		* @return デスクリプション
		*/
		void ExecuteCommandLists(std::vector<std::shared_ptr<CommandList>>& lists);

		/**
		* @fn
		* @brief キューのデスクリプションの取得
		* @param[in] lists　コマンドリストベクタへの参照
		* @return デスクリプション
		*/
		void ExecuteCopyCommands(std::vector<std::shared_ptr<CommandList>>& lists);

		/**
		* @fn
		* @brief キューのデスクリプションの取得
		* @param[in] lists　コマンドリストベクタへの参照
		* @return デスクリプション
		*/
		void ExecuteComputeCommands(std::vector<std::shared_ptr<CommandList>>& lists);

		/**
		* @fn
		* @brief キューの名前の設定
		* @param[in] name 名前
		*/
		void SetName(std::string name);

		/**
		* @fn
		* @brief 破棄
		*/
		void Discard();

	private:

		void Execute(std::vector<std::shared_ptr<CommandList>>& lists, Microsoft::WRL::ComPtr<ID3D12CommandQueue>& queue);

	};
}

