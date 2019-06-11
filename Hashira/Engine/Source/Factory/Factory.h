#pragma once
#include"Engine/Source/Utility/D3D12Common.h"


namespace Hashira {
	
	class D3D12Device;

	class Factory
	{

	public:

	private:
		//!ファクトリー
		Microsoft::WRL::ComPtr<IDXGIFactory4>	_factory;

	public:

		Factory();

		~Factory();

		/**
		* @fn
		* @brief ヒープのフェッチ
		* @param[in] createFlags オプションフラグ
		* @return S_OKで成功
		*/
		HRESULT	Initialize(UINT createFlags = 0);

		/**
		* @fn
		* @brief ファクトリのフェッチ
		* @return ファクトリの参照
		*/
		Microsoft::WRL::ComPtr<IDXGIFactory4>& GetFactory();

		/**
		* @fn
		* @brief ファクトリの破棄
		*/
		void Discard();

	private:

	};

}