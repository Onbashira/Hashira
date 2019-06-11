#pragma once
#include"Engine/Source/Utility/D3D12Common.h"
#include"Engine/Source/Utility//NonCopyable.h"
#include"Engine/Source/CommandList/CommandListManager.h"
#include"Engine/Source/ShaderObject/ShaderObjectManager.h"
#include <vector>



namespace Hashira {
	class CommandQueue;
	class DepthStencil;
	class Fence;
	class PipelineStateObject;


	class GraphicsContextManager : private NonCopyable
	{
	private:

		//!コマンドリストマネージャ
		CommandListManager			_commandListLibrary;

		//!シェーダーオブジェクトマネージャ
		ShaderObjectManager			_shaderObjectLibrary;

		////!キューマネージャ
		//CommandQueueManager         _queueManager;

	public:
	private:

		GraphicsContextManager();

	public:

		~GraphicsContextManager();

		/**
		* @fn
		* @brief コマンドリストマネージャ内のコマンドリストをすべて閉じる
		*/
		void CloseAllCommandLists();

		/**
		* @fn
		* @brief コマンドリスト名から、そのコマンドリストを検索し、閉じる
		* @param[in] commandListName コマンドリスト名前
		*/
		void CloseCommandList(std::string commandListName);

		/**
		* @fn
		* @brief コマンドリストマネージャ内のコマンドリストをすべてリセットする
		*/
		void ResetAllCommandList();

		/**
		* @fn
		* @brief コマンドリスト名から、そのコマンドリストを検索し、閉じる
		* @param[in] commandListName コマンドリスト名前
		*/
		void ResetCommandList(std::string commandListName,std::shared_ptr<RenderContext>& renderContext);

		/**
		* @fn
		* @brief 非同期でコマンドリストを実行する
		* @param[in] queue コマンドキューへのポインタ
		* @param[in] commandListName コマンドリスト名前
		* @param[in] fence フェンス
		* @return 成功でS_OK
		*/
		HRESULT ExecutionCommandListAsync(CommandQueue* queue, std::vector<std::string> commandListNames, Fence* fence = nullptr);

		/**
		* @fn
		* @brief すべてのコマンドリストを実行する
		* @param[in] queue コマンドキューへのポインタ
		* @param[in] fence フェンス
		* @return 成功でS_OK
		*/
		HRESULT ExecutionAllCommandLists(CommandQueue* queue, Fence* fence = nullptr);

		/**
		* @fn
		* @brief コマンドリストを作成する
		* @param[in] commandListName コマンドリストの名前
		* @param[in] device 作成用デバイス
		* @param[in] renderContext コンテキスト
		* @param[in] nodeMask ノードマスク
		* @param[in] listType リストタイプ
		* @return 成功でS_OK
		*/
		HRESULT CreateCommandList(std::string commandListName, std::shared_ptr<RenderingDevice>& device, std::shared_ptr<RenderContext>& renderContext, unsigned int nodeMask, D3D12_COMMAND_LIST_TYPE listType);

		/**
		* @fn
		* @brief パイプラインステートを作成
		* @param[in] psoName パイプラインステートの名前
		* @param[in] gps グラフィクスパイプラインステートのデスクリプション
		* @param[in] rootSignature ルートシグネチャ
		* @return 成功でS_OK
		*/
		HRESULT CreatePSO(std::string psoName, D3D12_GRAPHICS_PIPELINE_STATE_DESC& gps, ID3DBlob* rootSignature = nullptr);

		/**
		* @fn
		* @brief パイプラインステートを作成
		* @param[in] device デバイス
		* @param[in] psoName パイプラインステートの名前
		* @param[in] gps グラフィクスパイプラインステートのデスクリプション
		* @param[in] rootSignature ルートシグネチャ
		* @return 成功でS_OK
		*/
		HRESULT CreatePSO(std::shared_ptr<D3D12Device>& device,std::string psoName, D3D12_GRAPHICS_PIPELINE_STATE_DESC& gps, ID3DBlob * rootSignature = nullptr);

		/**
		* @fn
		* @brief パイプラインステートを作成
		* @param[in] psoName パイプラインステートの名前
		* @param[in] cps コンピュートパイプラインステートのデスクリプション
		* @param[in] rootSignature ルートシグネチャ
		* @return 成功でS_OK
		*/
		HRESULT CreatePSO(std::string psoName, D3D12_COMPUTE_PIPELINE_STATE_DESC& gps, ID3DBlob* rootSignature = nullptr);

		/**
		* @fn
		* @brief パイプラインステートを作成
		* @param[in] device デバイス
		* @param[in] psoName パイプラインステートの名前
		* @param[in] cps コンピュートパイプラインステートのデスクリプション
		* @param[in] rootSignature ルートシグネチャ
		* @return 成功でS_OK
		*/
		HRESULT CreatePSO(std::shared_ptr<D3D12Device>& device, std::string psoName, D3D12_COMPUTE_PIPELINE_STATE_DESC& gps, ID3DBlob * rootSignature = nullptr);

		///**
		//* @fn
		//* @brief コマンドキュー作成
		//* @param[in] queueName キューの名前
		//* @param[in] desc キューのデスクリプション
		//* @return 成功でS_OK
		//*/
		//HRESULT CreateCommandQueue(std::string queueName, D3D12_COMMAND_QUEUE_DESC& desc);

		/**
		* @fn
		* @brief ルートシグネチャ作成
		* @param[in] rsName ルートシグネチャの名前
		* @param[in] signature ルートシグネチャのデータポインタ
		* @return 成功でS_OK
		*/
		HRESULT CreateRootSignature(std::string rsName, ID3DBlob* signature);

		/**
		* @fn
		* @brief ルートシグネチャ作成
		* @param[in] rsName ルートシグネチャの名前
		* @param[in] signature ルートシグネチャのデスクリプション
		* @return 成功でS_OK
		*/
		HRESULT CreateRootSignature(std::string rsName, D3D12_ROOT_SIGNATURE_DESC* signature);

		/**
		* @fn
		* @brief コマンドリストのセット
		* @param[in] commandListName コマンドリスト名
		* @param[in] commandList コマンドリストの参照
		*/
		void SetCommandList(std::string commandListName, std::shared_ptr<CommandList> commandList);

		/**
		* @fn
		* @brief パイプラインステートのセット
		* @param[in] psoName パイプラインステート名
		* @param[in] pso パイプラインステートの参照
		*/
		void SetPSO(std::string psoName, std::shared_ptr<PipelineStateObject> pso);

		/**
		* @fn
		* @brief ルートシグネチャのセット
		* @param[in] rsName ルートシグネチャ名
		* @param[in] rootSignature ルートシグネチャの参照
		*/
		void SetRootSignature(std::string rsName, std::shared_ptr<RootSignature> rootSignature);

		/**
		* @fn
		* @brief コマンドリストのセット
		* @param[in] commandListName コマンドリスト名
		* @return コマンドリストへの参照
		*/
		std::shared_ptr<CommandList> GetCommandList(std::string commandListName);

		/**
		* @fn
		* @brief パイプラインスステートのセット
		* @param[in] psoName パイプラインスステート名
		* @return パイプラインスステート参照
		*/
		std::shared_ptr<PipelineStateObject> GetPSO(std::string psoName);

		///**
		//* @fn
		//* @brief キューのセット
		//* @param[in] queueName キュー名
		//* @return キュー参照
		//*/
		//std::shared_ptr<CommandQueue> GetCommandQueue(std::string queueName);

		/**
		* @fn
		* @brief ルートシグネチャのセット
		* @param[in] rsName ルートシグネチャ名
		* @return ルートシグネチャ参照
		*/
		std::shared_ptr<RootSignature> GetRootSignature(std::string rsName);

		/**
		* @fn
		* @brief コマンドリストの削除
		* @param[in] commandListName ルートシグネチャ名
		*/
		void EraseCommandList(std::string commandListName);

		/**
		* @fn
		* @brief パイプラインスステートの削除
		* @param[in] psoName パイプラインスステート名
		*/
		void ErasePSO(std::string psoName);

		/**
		* @fn
		* @brief ルートシグネチャの削除
		* @param[in] psoName ルートシグネチャ名
		*/
		void EraseRootSignature(std::string psoName);

		///**
		//* @fn
		//* @brief キューの削除
		//* @param[in] queueName キュー名
		//*/
		//void EraseCommandQueue(std::string queueName);

		/**
		* @fn
		* @brief すべてのキャリアーが保持しているデータの破棄
		*/
		void Discard();

		static GraphicsContextManager& GetInstance();
	};
};