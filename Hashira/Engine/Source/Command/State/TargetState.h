#pragma once
#include "Engine/Source/Utility/D3D12Common.h"
#include <vector>
namespace Hashira {
	class TargetState
	{
	public:
		//!�����_�����O�Ɏg��RT�̃n���h��
		std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> _rtvCPUHandles;

		//!�����_�����O�Ɏg���f�v�X�X�e���V���̃n���h��
		D3D12_CPU_DESCRIPTOR_HANDLE _dsvCPUHandle;
		
		//!�����_�����O�Ɏg���r���[�|�[�g
		std::vector<D3D12_VIEWPORT> _viewports;

		//!�����_�����O�Ɏg���؂蔲����`
		std::vector<D3D12_RECT> _scissorRects;

	private:

	public:

		TargetState();

		~TargetState();

		/**
		* @fn
		* @brief RTV�n���h���̒ǉ�
		* @param[in] handle RTV�ւ̃n���h��
		*/
		void AddRenderTargetHandle(D3D12_CPU_DESCRIPTOR_HANDLE& handle);

		/**
		* @fn
		* @brief DSV�n���h���̐ݒ�
		* @param[in] handle �n���h��
		*/
		void SetDepthStencilHandle(D3D12_CPU_DESCRIPTOR_HANDLE& handle);

		/**
		* @fn
		* @brief �r���[�|�[�g�̒ǉ�
		* @param[in] viewport �r���[�|�[�g
		*/
		void AddViewport(D3D12_VIEWPORT&& viewport);

		/**
		* @fn
		* @brief �؂蔲����`�̒ǉ�
		* @param[in] scissor �؂蔲����`
		*/
		void AddScissorRects(D3D12_RECT&& scissor);

	private:
	};

}