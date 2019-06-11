#pragma once
#include <d3d12.h>
#include <memory>
#include <string>
#include <map>
#include <vector>



namespace Hashira {

	class CommandList;
	class PipelineStateObject;
	class GraphicsContextManager;
	class DepthStencil;
	class RenderContext;
	class RenderingDevice;

	class CommandListManager
	{

		friend class GraphicsContextManager;

	public:

	private:

		//!コマンドリストのキャリア
		std::map<std::string, std::shared_ptr<CommandList>> _library;

	public:

		CommandListManager();

		~CommandListManager();

		/**
		* @fn
		* @brief リスト作成
		* @param[in] commandListName リストの名前
		* @param[in] nodeMask ノードマスク
		* @param[in] listType リストのタイプ
		* @return リザルト　S_OKで成功
		*/
		HRESULT Create(std::string commandListName, std::shared_ptr<RenderingDevice>& device,std::shared_ptr<RenderContext>& renderContext, unsigned int nodeMask, D3D12_COMMAND_LIST_TYPE listType);

		/**
		* @fn
		* @brief リスト作成
		* @param[in] commandListName セットするリストの名前
		* @param[in] commandList リスト本体
		* @return リザルト　S_OKで成功
		*/
		void Set(std::string commandListName, std::shared_ptr<CommandList> commandList);

		/**
		* @fn
		* @brief リストフェッチ
		* @param[in] commandListName リストの名前
		* @return リストへの参照
		*/
		std::shared_ptr<CommandList> Get(std::string commandListName);

		/**
		* @fn
		* @brief リスト削除
		* @param[in] commandListName リストの名前
		*/
		void Erase(std::string commandListName);

		/**
		* @fn
		* @brief 保持データの削除
		*/
		void Discard();

	private:

	};
};