#pragma once
#include "Engine/Source/Utility/D3D12Common.h"

namespace Hashira {
	class GeometryState
	{
	public:

	private:

		//!���_�o�b�t�@�r���[
		std::vector<D3D12_VERTEX_BUFFER_VIEW> _vertexBuffers;
		//!�C���f�b�N�X�o�b�t�@�r���[
		D3D12_INDEX_BUFFER_VIEW  _indexBuffer;

	public:

		GeometryState();

		~GeometryState();

		/**
		* @fn
		* @brief ���_�o�b�t�@�r���[�z��̎擾
		* @return  ���_�o�b�t�@�r���[�ւ̎Q��
		*/
		const std::vector<D3D12_VERTEX_BUFFER_VIEW>*  GetVertexBufferView();

		/**
		* @fn
		* @brief �C���f�b�N�X�o�b�t�@�r���[�̎擾
		* @return  �C���f�b�N�X�o�b�t�@�r���[�ւ̎Q��
		*/
		const D3D12_INDEX_BUFFER_VIEW* GetIndexBufferView();

		/**
		* @fn
		* @brief ���_�o�b�t�@�r���[�̃Z�b�g
		* @oaram[in] vertexBuffer�@VBV
		*/
		void AddVertexBufferView(D3D12_VERTEX_BUFFER_VIEW&& vertexBufferView);

		/**
		* @fn
		* @brief ���_�o�b�t�@�r���[�̃Z�b�g
		* @oaram[in] vertexBuffer�@VBV
		*/
		void AddVertexBufferView(const D3D12_VERTEX_BUFFER_VIEW& vertexBufferView);

		/**
		* @fn
		* @brief �C���f�b�N�X�o�b�t�@�r���[�̃Z�b�g
		* @oaram[in] indexBuffer�@IBV
		*/
		void SetIndexBufferView(D3D12_INDEX_BUFFER_VIEW& indexBufferView);

	private:

	};
}