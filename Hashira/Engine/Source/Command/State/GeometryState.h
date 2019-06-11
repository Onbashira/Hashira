#pragma once
#include "Engine/Source/Utility/D3D12Common.h"

namespace Hashira {
	class GeometryState
	{
	public:

	private:

		//!頂点バッファビュー
		std::vector<D3D12_VERTEX_BUFFER_VIEW> _vertexBuffers;
		//!インデックスバッファビュー
		D3D12_INDEX_BUFFER_VIEW  _indexBuffer;

	public:

		GeometryState();

		~GeometryState();

		/**
		* @fn
		* @brief 頂点バッファビュー配列の取得
		* @return  頂点バッファビューへの参照
		*/
		const std::vector<D3D12_VERTEX_BUFFER_VIEW>*  GetVertexBufferView();

		/**
		* @fn
		* @brief インデックスバッファビューの取得
		* @return  インデックスバッファビューへの参照
		*/
		const D3D12_INDEX_BUFFER_VIEW* GetIndexBufferView();

		/**
		* @fn
		* @brief 頂点バッファビューのセット
		* @oaram[in] vertexBuffer　VBV
		*/
		void AddVertexBufferView(D3D12_VERTEX_BUFFER_VIEW&& vertexBufferView);

		/**
		* @fn
		* @brief 頂点バッファビューのセット
		* @oaram[in] vertexBuffer　VBV
		*/
		void AddVertexBufferView(const D3D12_VERTEX_BUFFER_VIEW& vertexBufferView);

		/**
		* @fn
		* @brief インデックスバッファビューのセット
		* @oaram[in] indexBuffer　IBV
		*/
		void SetIndexBufferView(D3D12_INDEX_BUFFER_VIEW& indexBufferView);

	private:

	};
}