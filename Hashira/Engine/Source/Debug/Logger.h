#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <comdef.h>
#include "Engine/Source/Utility/NonCopyable.h"

namespace Hashira {
	//!ログレベル
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
		* @brief ロギング
		* @param[in] level ログレベル
		* @param[in] format ログ文
		*/
		virtual void		Log(LOG_LEVEL level, std::string format) = 0;

		/**
		* @fn
		* @brief ロギング
		* @param[in] level ログレベル
		* @param[in] format ログ文
		*/
		virtual void		LogW(LOG_LEVEL level, std::wstring format) = 0;

		/**
		* @fn
		* @brief フィルター以下のレベルのロギングを抑制するためフィルターの設定
		* @param[in] level 
		*/
		virtual void		SetFilter(LOG_LEVEL level) = 0;

		/**
		* @fn
		* @brief フィルターのフェッチ
		* @return フィルタ
		*/
		virtual LOG_LEVEL	GetFilter() = 0;

	};

	class  SystemLogger : public ILogger, private NonCopyable {
	private:

		//!フィルタ
		LOG_LEVEL _LogFilter;
		
		//！デバッグファイル
		std::ofstream _debugFile;

	public:

	private:

		SystemLogger();
		
		void CreateDebugFile();

	public:

		~SystemLogger();

		/**
		* @fn
		* @brief ファイルへマッピング
		* @param[in] fileName ファイル名
		*/
		void		Map(std::string fileName);
		
		/**
		* @fn
		* @brief マップ解除
		*/
		void		UnMap();
		
		/**
		* @fn
		* @brief ロギング
		* @param[in] level ログレベル
		* @param[in] hr リザルト
		*/
		void		Log(LOG_LEVEL level, HRESULT hr);
		/**
		* @fn
		* @brief ロギング
		* @param[in] level ログレベル
		* @param[in] format ログ文
		*/
		void		Log(LOG_LEVEL level, std::string format);
		
		/**
		* @fn
		* @brief ロギング
		* @param[in] level ログレベル
		* @param[in] format ログ文
		*/
		void		Log(LOG_LEVEL level, const char* format, ...);
		
		/**
		* @fn
		* @brief ロギング
		* @param[in] level ログレベル
		* @param[in] format ログ文
		*/
		void		LogW(LOG_LEVEL level, std::wstring format);
		
		/**
		* @fn
		* @brief ロギング
		* @param[in] level ログレベル
		* @param[in] format ログ文
		*/
		void		LogW(LOG_LEVEL level, const wchar_t* format, ...);
		
		/**
		* @fn
		* @brief フィルター以下のレベルのロギングを抑制するためフィルターの設定
		* @param[in] level
		*/
		void		SetFilter(LOG_LEVEL level);
		
		/**
		* @fn
		* @brief フィルターのフェッチ
		* @return フィルタ
		*/
		LOG_LEVEL	GetFilter();
		
		/**
		* @fn
		* @brief 出力パスの設定
		* @param[in] filePath
		*/
		void		SetDebugFilePath(std::string filePath);
		
		/**
		* @fn
		* @brief インスタンスの取得
		* @return インスタンス
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