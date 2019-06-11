#pragma once
#include <memory>
#include <vector>


namespace Hashira {
	class GamePad;

	class GamePadManager
	{
	public:

	private:
		//!�V�X�e�����Ō��߂�ڑ��������Q�[���p�b�h�ő吔
		int _maxConectionCount;
		
		//!���݂̐ڑ����ꂽ�Q�[���p�b�h
		
		std::vector<std::shared_ptr<GamePad>> _gamePads;
	public:

		/**
		* @fn
		* @brief �e�p�b�h���A�b�v�f�[�g����
		*/
		void Update();
		
		/**
		* @fn
		* @brief ID����Q�[���p�b�h���擾����
		* @param[in] padID �p�b�h��ID
		* @return �p�b�h�ւ̎Q��
		*/
		std::shared_ptr<GamePad> GetPad(int padID = 0);

		/**
		* @fn
		* @brief �p�b�h�S�̂̏�����
		* @param[in] macConectionCount �p�b�h�̐�
		*/
		void Initialize(int macConectionCount = 4);

		/**
		* @fn
		* @brief �p�b�h�S�̂̔j��
		*/
		void Discard();

		GamePadManager();

		~GamePadManager();

	private:

		/**
		* @fn
		* @brief �p�b�h�S�̂̍X�V
		*/
		void AllSetUp();

	};
}
