#pragma once
#include "Engine/Source/Utility/D3D12Common.h"
#include <unordered_map>



//シェーダーにパスに対応した
namespace Hashira {
	class ShaderHelper
	{
	public:
		enum class SHADER_TYPE {
			SHADER_TYPE_VERTEX,
			SHADER_TYPE_PIXEL,
			SHADER_TYPE_GEOMETRY,
			SHADER_TYPE_DOMAIN,
			SHADER_TYPE_HULL,
			SHADER_TYPE_COMPUTE
		};
	private:

		//!シェーダーマップ
		std::unordered_map<SHADER_TYPE, Microsoft::WRL::ComPtr<ID3DBlob>> _shaderMap;

		//!シェーダーマクロ
		std::vector<D3D_SHADER_MACRO> _shaderMacro;

	public:

		ShaderHelper();

		~ShaderHelper();

		/**
		* @fn
		* @brief シェーダーバイトコードの取得
		* @param[in] type シェーダータイプ
		* @return バイトコード
		*/
		Microsoft::WRL::ComPtr<ID3DBlob>& GetShader(ShaderHelper::SHADER_TYPE type);

		/**
		* @fn
		* @brief シェーダーBLOBの取得
		* @param[in] type シェーダータイプ
		* @return バイトコード
		*/

		/**
		* @fn
		* @brief マクロの追加
		* @param[in] name マクロネーム
		* @param[in] definition 定義
		*/
		void AddShaderMacro(std::string name, std::string definition);

		/**
		* @fn
		* @brief マクロの削除
		* @param[in] name マクロネーム
		*/
		void EraseShaderMacro(std::string name);

		/**
		* @fn
		* @brief マクロのフェッチ
		* @param[in] name マクロネーム
		* @return マクロ配列
		*/
		const std::vector<D3D_SHADER_MACRO>& GetShaderMacro()const;

		/**
		* @fn
		* @brief シェーダーコンパイル
		* @param[in] type タイプ
		* @param[in] shaderPath プロジェクト相対のシェーダーパス
		* @param[in] functionName 関数名
		* @param[in] shaderMode シェーダーマクロ
		* @param[in] includePath インクルードパス
		* @return リザルト
		*/
		HRESULT CompileShader(SHADER_TYPE type, std::string shaderPath, std::string functionName, std::string  shaderMode);

	private:

	};
}

