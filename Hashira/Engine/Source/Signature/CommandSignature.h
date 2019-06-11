#pragma once
#include <string>
#include "Engine/Source/Utility/D3D12Common.h"

namespace Hashira {

	class RootSignature;
	class D3D12Device;

	class CommandSignature
	{
	public:

	private:

		//!コマンドシグネチャ
		Microsoft::WRL::ComPtr<ID3D12CommandSignature>	 _commandSignature;

		//!名前
		std::string										 _name;

	public:

		CommandSignature();

		~CommandSignature();

		/**
		* @fn
		* @brief コマンドシグネチャの作成
		* @param[in] desc パイプラインスステート名
		* @param[in] rs バインド先のルートシグネチャ
		* @return リザルト
		*/
		HRESULT	Initialize(const D3D12_COMMAND_SIGNATURE_DESC& desc, std::weak_ptr<Hashira::RootSignature> rs);

		/**
		* @fn
		* @brief コマンドシグネチャの作成
		* @param[in] device 作成に使用するデバイス
		* @param[in] desc パイプラインスステート名
		* @param[in] rs バインド先のルートシグネチャ
		* @return リザルト
		*/
		HRESULT	Initialize(std::shared_ptr<D3D12Device>& device,const D3D12_COMMAND_SIGNATURE_DESC& desc, std::weak_ptr<Hashira::RootSignature> rs);

		/**
		* @fn
		* @brief コマンドシグネチャの作成
		* @param[in] desc パイプラインスステート名
		* @return リザルト
		*/
		HRESULT	Initialize(const D3D12_COMMAND_SIGNATURE_DESC& desc);

		/**
		* @fn
		* @brief コマンドシグネチャの作成
		* @param[in] device 作成に使用するデバイス
		* @param[in] desc パイプラインスステート名
		* @return リザルト
		*/
		HRESULT	Initialize(std::shared_ptr<D3D12Device>& device,const D3D12_COMMAND_SIGNATURE_DESC& desc);

		/**
		* @fn
		* @brief コマンドシグネチャのフェッチ
		* @return コマンドシグネチャ
		*/
		Microsoft::WRL::ComPtr<ID3D12CommandSignature>& GetSignature();

		/**
		* @fn
		* @brief コマンドシグネチャのフェッチ
		* @param[in] name 名前
		*/
		void	SetName(std::string name);

		/**
		* @fn
		* @brief 破棄
		*/
		void	Discard();

	private:
	};
}
