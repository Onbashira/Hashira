#pragma once
#include <d3d12.h>
#include <memory>
#include <string>
#include <map>
#include <vector>



namespace Hashira {

	class CommandList;
	class PipelineStateObject;
	class GraphicsContextManager;
	class DepthStencil;
	class RenderContext;
	class RenderingDevice;

	class CommandListManager
	{

		friend class GraphicsContextManager;

	public:

	private:

		//!�R�}���h���X�g�̃L�����A
		std::map<std::string, std::shared_ptr<CommandList>> _library;

	public:

		CommandListManager();

		~CommandListManager();

		/**
		* @fn
		* @brief ���X�g�쐬
		* @param[in] commandListName ���X�g�̖��O
		* @param[in] nodeMask �m�[�h�}�X�N
		* @param[in] listType ���X�g�̃^�C�v
		* @return ���U���g�@S_OK�Ő���
		*/
		HRESULT Create(std::string commandListName, std::shared_ptr<RenderingDevice>& device,std::shared_ptr<RenderContext>& renderContext, unsigned int nodeMask, D3D12_COMMAND_LIST_TYPE listType);

		/**
		* @fn
		* @brief ���X�g�쐬
		* @param[in] commandListName �Z�b�g���郊�X�g�̖��O
		* @param[in] commandList ���X�g�{��
		* @return ���U���g�@S_OK�Ő���
		*/
		void Set(std::string commandListName, std::shared_ptr<CommandList> commandList);

		/**
		* @fn
		* @brief ���X�g�t�F�b�`
		* @param[in] commandListName ���X�g�̖��O
		* @return ���X�g�ւ̎Q��
		*/
		std::shared_ptr<CommandList> Get(std::string commandListName);

		/**
		* @fn
		* @brief ���X�g�폜
		* @param[in] commandListName ���X�g�̖��O
		*/
		void Erase(std::string commandListName);

		/**
		* @fn
		* @brief �ێ��f�[�^�̍폜
		*/
		void Discard();

	private:

	};
};