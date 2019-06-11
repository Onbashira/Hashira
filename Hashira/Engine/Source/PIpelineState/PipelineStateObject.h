#pragma once
#include"Engine/Source/Utility/D3D12Common.h"
#include <memory>
#include <string>

namespace Hashira {

	class RootSignature;
	class D3D12Device;
	class PipelineStateObject
	{
	public:

	protected:

	private:
		//!パイプラインステート
		Microsoft::WRL::ComPtr<ID3D12PipelineState> _pipelineState;

		//!ルートシグネチャ
		std::weak_ptr<RootSignature>				_rootSignature;

		//各名前の頭につく名前
		std::string									_name;

	public:

		PipelineStateObject();

		virtual ~PipelineStateObject();

		/**
		* @fn
		* @brief パイプラインステートの作成
		* @param[in] name PSOの名前
		* @param[in] gpsDesc PSOのデスクリプション
		* @param[in] shader hlslに記述したルートシグネチャがバインドされているシェーダー
		* @return リザルト
		*/
		virtual HRESULT								Initialize(std::string name, D3D12_GRAPHICS_PIPELINE_STATE_DESC & gpsDesc, ID3DBlob* shader);

		/**
		* @fn
		* @brief パイプラインステートの作成
		* @param[in] device 作成に使うデバイスの参照
		* @param[in] name PSOの名前
		* @param[in] gpsDesc PSOのデスクリプション
		* @param[in] shader hlslに記述したルートシグネチャがバインドされているシェーダー
		* @return リザルト
		*/
		virtual HRESULT								Initialize(std::shared_ptr<D3D12Device>& device, std::string name, D3D12_GRAPHICS_PIPELINE_STATE_DESC & gpsDesc, ID3DBlob* shader);

		/**
		* @fn
		* @brief パイプラインステートの作成
		* @param[in] name PSOの名前
		* @param[in] cpsDesc PSOのデスクリプション
		* @param[in] shader hlslに記述したルートシグネチャがバインドされているシェーダー
		* @return リザルト
		*/
		virtual HRESULT								Initialize(std::string name, D3D12_COMPUTE_PIPELINE_STATE_DESC &  cpsDesc, ID3DBlob* shader);

		/**
		* @fn
		* @brief パイプラインステートの作成
		* @param[in] device 作成に使うデバイスの参照
		* @param[in] name PSOの名前
		* @param[in] cpsDesc PSOのデスクリプション
		* @param[in] shader hlslに記述したルートシグネチャがバインドされているシェーダー
		* @return リザルト
		*/
		virtual HRESULT								Initialize(std::shared_ptr<D3D12Device>& device,std::string name, D3D12_COMPUTE_PIPELINE_STATE_DESC &  cpsDesc, ID3DBlob* shader);

		/**
		* @fn
		* @brief パイプラインステートの作成
		* @param[in] name PSOの名前
		* @param[in] gpsDesc PSOのデスクリプション
		* @param[in] rootSignature ルートシグネチャ
		* @return リザルト
		*/
		virtual HRESULT								Initialize(std::string name, D3D12_GRAPHICS_PIPELINE_STATE_DESC & gpsDesc, std::weak_ptr<RootSignature> rootSignature);

		/**
		* @fn
		* @brief パイプラインステートの作成
		* @param[in] device 作成に使うデバイスの参照
		* @param[in] name PSOの名前
		* @param[in] gpsDesc PSOのデスクリプション
		* @param[in] rootSignature ルートシグネチャ
		* @return リザルト
		*/
		virtual HRESULT								Initialize(std::shared_ptr<D3D12Device>& device, std::string name, D3D12_GRAPHICS_PIPELINE_STATE_DESC & gpsDesc, std::weak_ptr<RootSignature> rootSignature);

		/**
		* @fn
		* @brief パイプラインステートの作成
		* @param[in] name PSOの名前
		* @param[in] cpsDesc PSOのデスクリプション
		* @param[in] rootSignature ルートシグネチャ
		* @return リザルト
		*/
		virtual HRESULT								Initialize(std::string name, D3D12_COMPUTE_PIPELINE_STATE_DESC &  cpsDesc, std::weak_ptr<RootSignature> rootSignature);

		/**
		* @fn
		* @brief パイプラインステートの作成
		* @param[in] device 作成に使うデバイスの参照
		* @param[in] name PSOの名前
		* @param[in] cpsDesc PSOのデスクリプション
		* @param[in] rootSignature ルートシグネチャ
		* @return リザルト
		*/
		virtual HRESULT								Initialize(std::shared_ptr<D3D12Device>& device,std::string name, D3D12_COMPUTE_PIPELINE_STATE_DESC &  cpsDesc, std::weak_ptr<RootSignature> rootSignature);

		/**
		* @fn
		* @brief 名前の設定
		* @param[in] name PSOの名前
		*/
		void										SetName(std::string name);

		/**
		* @fn
		* @brief ルートシグネチャへの弱参照のフェッチ
		* @return バインドされているルートシグネチャへの弱参照
		*/
		std::weak_ptr<RootSignature>				GetRootSignature();

		/**
		* @fn
		* @brief PSOのフェッチ
		* @return PSOの参照
		*/
		Microsoft::WRL::ComPtr<ID3D12PipelineState>& GetPSO();

		/**
		* @fn
		* @brief データ破棄
		*/
		virtual void Discard();

	private:

	};
}
