#pragma once
#include "Engine/Source/Utility/D3D12Common.h"

/**
 * @file Fenc
 * @brief Fence���Ǘ�����N���X
 * @author ���ъJ�l
 * @date
 */

namespace Hashira {

	class CommandQueue;

	class Fence
	{
	public:

	private:

		//!Fence�{��
		Microsoft::WRL::ComPtr<ID3D12Fence> _fence;

		//!Fence�o�����[
		UINT64								_fenceValue;

		//!�C�x���g
		void*								_fenceEvent;

	public:

		Fence();

		~Fence();

		/**
		* @fn
		* @brief �t�F���X������������
		* @param[in] initialFenceValue
		* @param[in] flags 
		* @return S_OK�Ő����A����ȊO�Ŏ��s
		*/
		HRESULT								Initialize(UINT64 initialFenceValue, D3D12_FENCE_FLAGS flags);

		/**
		 * @fn
		 * @brief �f�[�^�̔j��
		 */
		void								Discard();

		/**
		 * @fn
		 * @brief �t�F���X�̎擾
		 * @return �t�F���X�{��
		 */
		Microsoft::WRL::ComPtr<ID3D12Fence>	GetFence();

		/**
		 * @fn
		 * @brief �t�F���X�̒l���擾����
		 * @return �߂�l�̐���
		 */
		UINT64								GetFenceValue();

		/**
		 * @fn
		 * @brief ���蓖�Ă�ꂽ�t�F���X�C�x���g�̎擾
		 * @return �t�F���X�C�x���g
		 */
		void*								GetFenceEvent();

		/**
		 * @fn
		 * @brief �t�F���X�𒣂��đҋ@
		 * @param[in] commandQueue �L���[ 
		 * @return S_OK�Ő����A����ȊO�Ŏ��s
		 */
		HRESULT								Wait(CommandQueue* commandQueue);

	private:

	};
}

