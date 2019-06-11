#pragma once
#include <d3d12.h>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace Hashira {

	class PipelineStateObject;
	class RootSignature;
	class D3D12Device;

	class ShaderObjectManager
	{
	private:
		
		//!PSOキャリア
		std::map <std::string, std::shared_ptr<Hashira::PipelineStateObject>> _psolibrary;
		
		//!RSキャリア
		std::map <std::string, std::shared_ptr<Hashira::RootSignature>> _rootSignaturelibrary;
	
	public:
	private:
	public:

		ShaderObjectManager();
		
		~ShaderObjectManager();

		/**
		* @fn
		* @brief パイプラインステートを作成
		* @param[in] psoName パイプラインステートの名前
		* @param[in] gps グラフィクスパイプラインステートのデスクリプション
		* @param[in] rootSignature ルートシグネチャ
		* @return 成功でS_OK
		*/
		HRESULT CreatePSO(std::string psoName, D3D12_GRAPHICS_PIPELINE_STATE_DESC& gps, ID3DBlob* signature = nullptr);
		
		/**
		* @fn
		* @brief パイプラインステートを作成
		* @param[in] device デバイス
		* @param[in] psoName パイプラインステートの名前
		* @param[in] gps グラフィクスパイプラインステートのデスクリプション
		* @param[in] rootSignature ルートシグネチャ
		* @return 成功でS_OK
		*/
		HRESULT CreatePSO(std::shared_ptr<D3D12Device>& device ,std::string psoName, D3D12_GRAPHICS_PIPELINE_STATE_DESC& gps, ID3DBlob* signature = nullptr);

		/**
		* @fn
		* @brief パイプラインステートを作成
		* @param[in] psoName パイプラインステートの名前
		* @param[in] cps コンピュートパイプラインステートのデスクリプション
		* @param[in] rootSignature ルートシグネチャ
		* @return 成功でS_OK
		*/
		HRESULT CreatePSO(std::string psoName, D3D12_COMPUTE_PIPELINE_STATE_DESC& cps, ID3DBlob* signature = nullptr);
		
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
		HRESULT CreateRootSignature(std::string rsName,D3D12_ROOT_SIGNATURE_DESC * desc);

		/**
		* @fn
		* @brief パイプラインステートのセット
		* @param[in] psoName パイプラインステート名
		* @param[in] pso パイプラインステートの参照
		*/
		void SetPSO(std::string psoName, std::shared_ptr<Hashira::PipelineStateObject> pso);
		
		/**
		* @fn
		* @brief ルートシグネチャのセット
		* @param[in] rsName ルートシグネチャ名
		* @param[in] rootSignature ルートシグネチャの参照
		*/
		void SetRootSignature(std::string rsName, std::shared_ptr<RootSignature> rootSignature);

		/**
		* @fn
		* @brief パイプラインスステートのセット
		* @param[in] psoName パイプラインスステート名
		* @return パイプラインスステート参照
		*/
		std::weak_ptr<Hashira::PipelineStateObject> GetPSO(std::string psoName);
		
		/**
		* @fn
		* @brief ルートシグネチャのセット
		* @param[in] rsName ルートシグネチャ名
		* @return ルートシグネチャ参照
		*/
		std::weak_ptr<Hashira::RootSignature> GetRootSignature(std::string rsName);

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

		/**
		* @fn
		* @brief すべてのキャリアーが保持しているデータの破棄
		*/
		void Discard();

	};
}