#pragma once
#include "Engine/Source/Utility/D3D12Common.h"

/**
 * @file Fenc
 * @brief Fenceを管理するクラス
 * @author 小林開人
 * @date
 */

namespace Hashira {

	class CommandQueue;

	class Fence
	{
	public:

	private:

		//!Fence本体
		Microsoft::WRL::ComPtr<ID3D12Fence> _fence;

		//!Fenceバリュー
		UINT64								_fenceValue;

		//!イベント
		void*								_fenceEvent;

	public:

		Fence();

		~Fence();

		/**
		* @fn
		* @brief フェンスを初期化する
		* @param[in] initialFenceValue
		* @param[in] flags 
		* @return S_OKで成功、それ以外で失敗
		*/
		HRESULT								Initialize(UINT64 initialFenceValue, D3D12_FENCE_FLAGS flags);

		/**
		 * @fn
		 * @brief データの破棄
		 */
		void								Discard();

		/**
		 * @fn
		 * @brief フェンスの取得
		 * @return フェンス本体
		 */
		Microsoft::WRL::ComPtr<ID3D12Fence>	GetFence();

		/**
		 * @fn
		 * @brief フェンスの値を取得する
		 * @return 戻り値の説明
		 */
		UINT64								GetFenceValue();

		/**
		 * @fn
		 * @brief 割り当てられたフェンスイベントの取得
		 * @return フェンスイベント
		 */
		void*								GetFenceEvent();

		/**
		 * @fn
		 * @brief フェンスを張って待機
		 * @param[in] commandQueue キュー 
		 * @return S_OKで成功、それ以外で失敗
		 */
		HRESULT								Wait(CommandQueue* commandQueue);

	private:

	};
}

