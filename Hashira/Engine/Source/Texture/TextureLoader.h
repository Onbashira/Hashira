#pragma once
#include "Engine/Source/DescriptorHeap/DescriptorHeap.h"
#include <vector>
#include <string>
#include "Engine/ThirdParty/DirectXTex/include/DirectXTex.h"

namespace Hashira {

	class ShaderResource;
	class CommandList;
	class CommandQueue;
	class D3D12Device;
	class TextureObject;
	class RenderingDevice;
	class RenderContext;
	
	class TextureLoader
	{
	
		friend class TextureManager;

	public:

	private:

	public:

		static TextureLoader& GetInstance() {
			static TextureLoader instance;
			return instance;
		};
			~TextureLoader();

		/**
		* @fn
		* @brief ���f���̃e�N�X�`���̃��[�h
		* @param[in] commandList ���X�g�̎Q��
		* @param[in] renderContext �����_�����O�Ǘ��I�u�W�F�N�g�̎Q��
		* @param[in] filePath �t�@�C���p�X
		*/
		std::shared_ptr<TextureObject> LoadTextureResource(std::shared_ptr<RenderContext>& renderContex, std::string filePath);

	private:

		TextureLoader();

		/**
		* @fn
		* @brief UpdateSubresource��p�����o�b�t�@�ւ̏�������
		* @param[in] commandList ���X�g�̎Q��
		* @param[in] commandQueue �L���[�̎Q��
		* @param[in] resource �V�F�[�_�[���\�[�X�ւ̎�Q��
		* @param[out] subResource �T�u���\�[�X
		* @param[in] path �t�@�C���p�X
		* @return ���U���g
		*/
		HRESULT UpdateSubResource(std::shared_ptr<CommandList> list, std::shared_ptr<RenderContext>& renderContex, std::weak_ptr<ShaderResource> resource, D3D12_SUBRESOURCE_DATA& subResource, std::string path);


		/**
		* @fn
		* @brief WriteToSubResource��p�����o�b�t�@�ւ̏�������
		* @param[in] commandList ���X�g�̎Q��
		* @param[in] commandQueue �L���[�̎Q��
		* @param[in] resource �V�F�[�_�[���\�[�X�ւ̎�Q��
		* @param[out] subResource �T�u���\�[�X
		* @param[in] paths �t�@�C���p�X
		* @return ���U���g
		*/
		HRESULT WriteToSubResource(std::shared_ptr<CommandList> list, std::shared_ptr<RenderContext>& renderContex, std::weak_ptr<ShaderResource> resource, D3D12_SUBRESOURCE_DATA& subResource, std::string path);

		/**
		* @fn
		* @brief File�̓ǂݍ���
		* @param[out] metaData ���^�f�[�^�ւ̎Q��
		* @param[out] scratchImage �X�N���b�`�f�[�^�ւ̎Q��
		* @param[in] path �t�@�C���p�X
		* @return ���U���g
		*/
		HRESULT LoadFile(DirectX::TexMetadata& metaData, DirectX::ScratchImage& scratchImage, std::string& path);

		/**
		* @fn
		* @brief WICFile�̓ǂݍ���
		* @param[out] metaData ���^�f�[�^�ւ̎Q��
		* @param[out] scratchImage �X�N���b�`�f�[�^�ւ̎Q��
		* @param[in] path �t�@�C���p�X
		* @return ���U���g
		*/
		HRESULT LoadWICFile(DirectX::TexMetadata& metaData, DirectX::ScratchImage& scratchImage, std::string& path);

		/**
		* @fn
		* @brief DDSFile�̓ǂݍ���
		* @param[out] metaData ���^�f�[�^�ւ̎Q��
		* @param[out] scratchImage �X�N���b�`�f�[�^�ւ̎Q��
		* @param[in] path �t�@�C���p�X
		* @return ���U���g
		*/
		HRESULT LoadDDSFile(DirectX::TexMetadata& metaData, DirectX::ScratchImage& scratchImage, std::string& path);

		/**
		* @fn
		* @brief TGAFile�̓ǂݍ���
		* @param[out] metaData ���^�f�[�^�ւ̎Q��
		* @param[out] scratchImage �X�N���b�`�f�[�^�ւ̎Q��
		* @param[in] path �t�@�C���p�X
		* @return ���U���g
		*/
		HRESULT LoadTGAFile(DirectX::TexMetadata& metaData, DirectX::ScratchImage& scratchImage, std::string& path);

		/**
		* @fn
		* @brief �t�H�[�}�b�g����K���}�␳���������Ă��邩�ǂ����̌���������
		* @param[in] format format
		* @return ���U���g �^�ŃK���}�l����
		*/
		bool IsUseGamma(DXGI_FORMAT format);

	};

}