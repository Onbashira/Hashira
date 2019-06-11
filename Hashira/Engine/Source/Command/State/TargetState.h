#pragma once
#include "Engine/Source/Utility/D3D12Common.h"
#include <vector>
namespace Hashira {
	class TargetState
	{
	public:
		//!レンダリングに使うRTのハンドル
		std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> _rtvCPUHandles;

		//!レンダリングに使うデプスステンシルのハンドル
		D3D12_CPU_DESCRIPTOR_HANDLE _dsvCPUHandle;
		
		//!レンダリングに使うビューポート
		std::vector<D3D12_VIEWPORT> _viewports;

		//!レンダリングに使う切り抜き矩形
		std::vector<D3D12_RECT> _scissorRects;

	private:

	public:

		TargetState();

		~TargetState();

		/**
		* @fn
		* @brief RTVハンドルの追加
		* @param[in] handle RTVへのハンドル
		*/
		void AddRenderTargetHandle(D3D12_CPU_DESCRIPTOR_HANDLE& handle);

		/**
		* @fn
		* @brief DSVハンドルの設定
		* @param[in] handle ハンドル
		*/
		void SetDepthStencilHandle(D3D12_CPU_DESCRIPTOR_HANDLE& handle);

		/**
		* @fn
		* @brief ビューポートの追加
		* @param[in] viewport ビューポート
		*/
		void AddViewport(D3D12_VIEWPORT&& viewport);

		/**
		* @fn
		* @brief 切り抜き矩形の追加
		* @param[in] scissor 切り抜き矩形
		*/
		void AddScissorRects(D3D12_RECT&& scissor);

	private:
	};

}