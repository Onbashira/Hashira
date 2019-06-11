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
		
		//!パイプラインステートオブジェクト
		std::shared_ptr<PipelineStateObject> _pso;
	
		//!ロートシグネチャ
		std::shared_ptr<RootSignature> _rs;
		

	public:

		PipelineState();

		~PipelineState();
		
		/**
		* @fn
		* @brief PSOの取得
		* @return  PSOへの参照
		*/
		std::weak_ptr<PipelineStateObject> GetPSO();

		/**
		* @fn
		* @brief RSの取得
		* @return  RSへの参照
		*/
		std::weak_ptr<RootSignature> GetRS();

		/**
		* @fn
		* @brief PSOのセット
		*/
		void SetPSO(std::weak_ptr<PipelineStateObject> pso);

		/**
		* @fn
		* @brief RSの取得
		*/
		void SetRS(std::weak_ptr<RootSignature> rs);

	private:

	};

}