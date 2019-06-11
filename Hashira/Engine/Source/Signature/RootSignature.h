#pragma once
#include "Engine/Source/Utility/D3D12Common.h"
#include <string>

namespace Hashira {

	class RootSignature
	{
	public:

	private:

		//!ルートシグネチャ
		Microsoft::WRL::ComPtr<ID3D12RootSignature> _rootSignature;

		//!名前
		std::string									_name;

	public:

		RootSignature();

		~RootSignature();

		/**
		* @fn
		* @brief ルートシグネチャの作成
		* @param[in] shader バインドされたシェーダー
		* @return リザルト
		*/
		HRESULT CreateFromShader(ID3DBlob* shader);

		/**
		* @fn
		* @brief ルートシグネチャの作成
		* @param[in] desc デスクリプション
		* @return リザルト
		*/
		HRESULT CreateFromDesc(D3D12_ROOT_SIGNATURE_DESC* desc);

		/**
		* @fn
		* @brief ルートシグネチャのフェッチ
		* @return ルートシグネチャへの参照
		*/
		Microsoft::WRL::ComPtr<ID3D12RootSignature>& GetSignature();

		/**
		* @fn
		* @brief 名前の設定
		* @param[in] name 名前
		*/
		void SetName(std::string name);

		/**
		* @fn
		* @brief　破棄
		*/
		void Discard();

	private:

	};
}