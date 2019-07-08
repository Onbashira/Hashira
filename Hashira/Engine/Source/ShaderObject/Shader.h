#pragma once
#include "Engine/Source/Utility/D3D12Common.h"
#include <unordered_map>


//�V�F�[�_
namespace Hashira {
	class Shader
	{
	public:
			enum Type {
				SHADER_TYPE_VERTEX,
				SHADER_TYPE_PIXEL,
				SHADER_TYPE_GEOMETRY,
				SHADER_TYPE_DOMAIN,
				SHADER_TYPE_HULL,
				SHADER_TYPE_COMPUTE
			};
	private:

		//!�V�F�[�_�[�}�b�v
		std::unordered_map<Type, Microsoft::WRL::ComPtr<ID3DBlob>> _shaderMap;

		//!�V�F�[�_�[�}�N��
		std::vector<D3D_SHADER_MACRO> _shaderMacro;

	public:

		Shader();

		~Shader();

		/**
		* @fn
		* @brief �V�F�[�_�[�o�C�g�R�[�h�̎擾
		* @param[in] type �V�F�[�_�[�^�C�v
		* @return �o�C�g�R�[�h
		*/
		Microsoft::WRL::ComPtr<ID3DBlob>& GetShader(Type type);

		/**
		* @fn
		* @brief �V�F�[�_�[BLOB�̎擾
		* @param[in] type �V�F�[�_�[�^�C�v
		* @return �o�C�g�R�[�h
		*/

		/**
		* @fn
		* @brief �}�N���̒ǉ�
		* @param[in] name �}�N���l�[��
		* @param[in] definition ��`
		*/
		void AddShaderMacro(std::string name, std::string definition);

		/**
		* @fn
		* @brief �}�N���̍폜
		* @param[in] name �}�N���l�[��
		*/
		void EraseShaderMacro(std::string name);

		/**
		* @fn
		* @brief �}�N���̃t�F�b�`
		* @param[in] name �}�N���l�[��
		* @return �}�N���z��
		*/
		const std::vector<D3D_SHADER_MACRO>& GetShaderMacro()const;

		/**
		* @fn
		* @brief �V�F�[�_�[�R���p�C��
		* @param[in] type �^�C�v
		* @param[in] shaderPath �v���W�F�N�g���΂̃V�F�[�_�[�p�X
		* @param[in] functionName �֐���
		* @param[in] shaderMode �V�F�[�_�[�}�N��
		* @param[in] includePath �C���N���[�h�p�X
		* @return ���U���g
		*/
		HRESULT CompileShader(Type type, std::string shaderPath, std::string functionName, std::string  shaderMode);

	private:

	};
}

