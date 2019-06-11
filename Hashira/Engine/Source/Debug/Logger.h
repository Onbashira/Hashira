#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <comdef.h>
#include "Engine/Source/Utility/NonCopyable.h"

namespace Hashira {
	//!���O���x��
	enum class LOG_LEVEL {
		Details,
		Info,
		Debug,
		Warning,
		Error
	};

	class  ILogger {
	private:

	public:

	private:

	public:

		virtual ~ILogger() {};

		/**
		* @fn
		* @brief ���M���O
		* @param[in] level ���O���x��
		* @param[in] format ���O��
		*/
		virtual void		Log(LOG_LEVEL level, std::string format) = 0;

		/**
		* @fn
		* @brief ���M���O
		* @param[in] level ���O���x��
		* @param[in] format ���O��
		*/
		virtual void		LogW(LOG_LEVEL level, std::wstring format) = 0;

		/**
		* @fn
		* @brief �t�B���^�[�ȉ��̃��x���̃��M���O��}�����邽�߃t�B���^�[�̐ݒ�
		* @param[in] level 
		*/
		virtual void		SetFilter(LOG_LEVEL level) = 0;

		/**
		* @fn
		* @brief �t�B���^�[�̃t�F�b�`
		* @return �t�B���^
		*/
		virtual LOG_LEVEL	GetFilter() = 0;

	};

	class  SystemLogger : public ILogger, private NonCopyable {
	private:

		//!�t�B���^
		LOG_LEVEL _LogFilter;
		
		//�I�f�o�b�O�t�@�C��
		std::ofstream _debugFile;

	public:

	private:

		SystemLogger();
		
		void CreateDebugFile();

	public:

		~SystemLogger();

		/**
		* @fn
		* @brief �t�@�C���փ}�b�s���O
		* @param[in] fileName �t�@�C����
		*/
		void		Map(std::string fileName);
		
		/**
		* @fn
		* @brief �}�b�v����
		*/
		void		UnMap();
		
		/**
		* @fn
		* @brief ���M���O
		* @param[in] level ���O���x��
		* @param[in] hr ���U���g
		*/
		void		Log(LOG_LEVEL level, HRESULT hr);
		/**
		* @fn
		* @brief ���M���O
		* @param[in] level ���O���x��
		* @param[in] format ���O��
		*/
		void		Log(LOG_LEVEL level, std::string format);
		
		/**
		* @fn
		* @brief ���M���O
		* @param[in] level ���O���x��
		* @param[in] format ���O��
		*/
		void		Log(LOG_LEVEL level, const char* format, ...);
		
		/**
		* @fn
		* @brief ���M���O
		* @param[in] level ���O���x��
		* @param[in] format ���O��
		*/
		void		LogW(LOG_LEVEL level, std::wstring format);
		
		/**
		* @fn
		* @brief ���M���O
		* @param[in] level ���O���x��
		* @param[in] format ���O��
		*/
		void		LogW(LOG_LEVEL level, const wchar_t* format, ...);
		
		/**
		* @fn
		* @brief �t�B���^�[�ȉ��̃��x���̃��M���O��}�����邽�߃t�B���^�[�̐ݒ�
		* @param[in] level
		*/
		void		SetFilter(LOG_LEVEL level);
		
		/**
		* @fn
		* @brief �t�B���^�[�̃t�F�b�`
		* @return �t�B���^
		*/
		LOG_LEVEL	GetFilter();
		
		/**
		* @fn
		* @brief �o�̓p�X�̐ݒ�
		* @param[in] filePath
		*/
		void		SetDebugFilePath(std::string filePath);
		
		/**
		* @fn
		* @brief �C���X�^���X�̎擾
		* @return �C���X�^���X
		*/
		static SystemLogger& GetInstance();
	};

#define HRESULT_LOG(hr)		Hashira::SystemLogger::GetInstance().Log(Hashira::LOG_LEVEL::Details,	"[ FILE : %s, LINE : %d] { %s }\n", __FILE__, __LINE__, reinterpret_cast<const char*>(_com_error(hr).ErrorMessage()))
#define	DEBUG_LOG(str)		Hashira::SystemLogger::GetInstance().Log(Hashira::LOG_LEVEL::Debug,		"[ FILE : %s, LINE : %d] { %s }\n", __FILE__, __LINE__, str.c_str())
#define INFO_LOG(str)		Hashira::SystemLogger::GetInstance().Log(Hashira::LOG_LEVEL::Info ,		"[ FILE : %s, LINE : %d] { %s }\n", __FILE__, __LINE__, str.c_str())
#define DETAILS_LOG(str)	Hashira::SystemLogger::GetInstance().Log(Hashira::LOG_LEVEL::Details ,	"[ FILE : %s, LINE : %d] { %s }\n", __FILE__, __LINE__, str.c_str())
#define WARNING_LOG(str)	Hashira::SystemLogger::GetInstance().Log(Hashira::LOG_LEVEL::Warning ,	"[ FILE : %s, LINE : %d] { %s }\n", __FILE__, __LINE__, str.c_str())
#define ERROR_LOG(str)		Hashira::SystemLogger::GetInstance().Log(Hashira::LOG_LEVEL::Error ,		"[ FILE : %s, LINE : %d] { %s }\n", __FILE__, __LINE__, str.c_str())


#if defined(UNICODE) || defined(_UNICODE)
#else
#endif

}