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

		//!���΃p�X
		std::string _relativePath;

		//!�C���N���[�h������
		std::string _nowString;


	
	public:

		HLSLIncluder() {};

		~HLSLIncluder() {};
		
		HRESULT STDMETHODCALLTYPE	Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes);
		
		HRESULT	STDMETHODCALLTYPE	Close(LPCVOID pData);

		/**
		* @fn
		* @brief ���΃p�X�̐ݒ�
		* @param[in] relativePath ���΃p�X
		*/
		void SetRelativePath(std::string relativePath);
		
		/**
		* @fn
		* @brief �R���X�g���N�^
		* @param[in] relativePath ���΃p�X
		*/
		HLSLIncluder(std::string relativePath);

	private:

	};
}

