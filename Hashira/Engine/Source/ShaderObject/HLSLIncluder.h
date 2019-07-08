#pragma once
#include <string>
#include <d3dcompiler.h>

namespace Hashira {

	class HLSLIncluder : public ID3DInclude
	{
		friend class ShaderHelper;
		friend class Shader;

	public:

	private:

		//!相対パス
		std::string _relativePath;

		//!インクルード文字列
		std::string _nowString;


	
	public:

		HLSLIncluder() {};

		~HLSLIncluder() {};
		
		HRESULT STDMETHODCALLTYPE	Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes);
		
		HRESULT	STDMETHODCALLTYPE	Close(LPCVOID pData);

		/**
		* @fn
		* @brief 相対パスの設定
		* @param[in] relativePath 相対パス
		*/
		void SetRelativePath(std::string relativePath);
		
		/**
		* @fn
		* @brief コンストラクタ
		* @param[in] relativePath 相対パス
		*/
		HLSLIncluder(std::string relativePath);

	private:

	};
}

