#pragma once
#include "Engine/Source/PipelineState/PipelineStateObject.h"
#include "Engine/Source/Signature/RootSignature.h"
#include "Engine/Source/Signature/CommandSignature.h"
#include <vector>
#include <memory>

namespace Hashira {
	class PipelineState
	{

	public:

	private:
		
		//!�p�C�v���C���X�e�[�g�I�u�W�F�N�g
		std::shared_ptr<PipelineStateObject> _pso;
	
		//!���[�g�V�O�l�`��
		std::shared_ptr<RootSignature> _rs;
		

	public:

		PipelineState();

		~PipelineState();
		
		/**
		* @fn
		* @brief PSO�̎擾
		* @return  PSO�ւ̎Q��
		*/
		std::weak_ptr<PipelineStateObject> GetPSO();

		/**
		* @fn
		* @brief RS�̎擾
		* @return  RS�ւ̎Q��
		*/
		std::weak_ptr<RootSignature> GetRS();

		/**
		* @fn
		* @brief PSO�̃Z�b�g
		*/
		void SetPSO(std::weak_ptr<PipelineStateObject> pso);

		/**
		* @fn
		* @brief RS�̎擾
		*/
		void SetRS(std::weak_ptr<RootSignature> rs);

	private:

	};

}