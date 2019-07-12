#pragma once
#include "BasicTypes.h"
#include <Windows.h>
#include <string>
#include <iostream>
#include <vector>
#include <d3d12.h>


namespace Hashira {

	template <typename T>
	void SafeDelete(T& p) {
		if (p != nullptr) {

			delete p;
			p = nullptr;
		}
	}

	template <typename T>
	void SafeRelease(T& p) {
		if (p != nullptr) {

			p->Release();
			p = nullptr;
		}
	}

	template <typename T>
	void SafeDeleteArray(T& p) {
		if (p != nullptr) {

			delete[] p;
			p = nullptr;
		}
	}


	constexpr Uint32 Fnv1aPrime32 = 16777619;
	constexpr Uint32 Fnv1aSeed32 = 0x811c9dc5;
	constexpr Uint64 Fnv1aPrime64 = 1099511628211L;
	constexpr Uint64 Fnv1aSeed64 = 0xcbf29ce484222325;

	inline Uint32 CalcFnv1a32(Uint8 oneByte, Uint32 hash = Fnv1aSeed32)
	{
		return (oneByte ^ hash) * Fnv1aPrime32;
	}
	inline Uint32 CalcFnv1a32(const void* data, size_t numBytes, Uint32 hash = Fnv1aSeed32)
	{
		assert(data);
		const Uint8 * ptr = reinterpret_cast<const Uint8*>(data);
		while (numBytes--)
			hash = CalcFnv1a32(*ptr++, hash);
		return hash;
	}
	inline Uint64 CalcFnv1a64(Uint8 oneByte, Uint64 hash = Fnv1aSeed64)
	{
		return (oneByte ^ hash) * Fnv1aPrime32;
	}
	inline Uint64 CalcFnv1a64(const void* data, size_t numBytes, Uint64 hash = Fnv1aSeed64)
	{
		assert(data);
		const Uint8 * ptr = reinterpret_cast<const Uint8*>(data);
		while (numBytes--)
			hash = CalcFnv1a64(*ptr++, hash);
		return hash;
	}

}
namespace Hashira::Util {

	//256バイトアライメント
	inline size_t Alignment256Bytes(size_t size) {
		if (size == 256) {
			return 256;
		}
		size_t ret = (size&(~0xff)) + 0x0100;
		return ret;
	};

	template <typename T>
	T inline Align(T num, unsigned int ali) {
		return ((num + ali - 1) / ali) * ali;
	}

	template <typename T>
	T inline Align(T num, Uint64 ali) {
		return ((num + ali - 1) / ali) * ali;
	}

	template <typename T>
	bool IsPowerOfTwo(T val)
	{
		return val > 0 && (val & (val - 1)) == 0;
	}

	inline unsigned int ConstantBufferAlign(unsigned int byteSize) {
		if (byteSize == 256) {
			return 256;
		}
		return ((byteSize + 255) & ~255);
	}

	inline unsigned int  AlignForUavCounter(UINT bufferSize)
	{
		const UINT alignment = 4096;
		return (bufferSize + (alignment - 1)) & ~(alignment - 1);
	}

	template<typename T>inline unsigned int ConvertByteToBit()
	{
		return (sizeof(T) * 8);
	}

	inline unsigned int ConvertBitToByte(unsigned int bitCount)
	{
		return (bitCount / 8);
	}

	inline std::string GetRelativePath(const std::string& path)
	{
		size_t relative = path.rfind('/') + 1;
		std::string rtn;
		rtn.resize(relative);
		std::copy(path.begin(), path.begin() + relative, rtn.begin());
		return rtn;
	}

	template <typename T>inline
	std::string ConvertNumToString(T num, const unsigned int maxDigit)
	{
		std::string rtn;
		rtn.resize(maxDigit);
		unsigned int extractDigit = 10;

		for (unsigned int i = 0; i < maxDigit; ++i)
		{
			T currentDigit = num % extractDigit;
			rtn[maxDigit - 1 - i] = currentDigit + '0';
			num /= extractDigit;
		}
		return rtn;
	}

	inline std::string ConvertNumberToString(unsigned int num)
	{
		const unsigned int MAX_DIGIT = 10;
		return ConvertNumToString(num, MAX_DIGIT);
	}

	inline std::string ConvertNumberToString(unsigned char num)
	{
		const unsigned int MAX_DIGIT = 3;
		return ConvertNumToString(num, MAX_DIGIT);
	}

	inline unsigned int ConvertStringToUINT(std::string num)
	{
		const unsigned int digit = static_cast<unsigned int>(num.size());

		unsigned int rtn = 0;

		unsigned int extractDigit = 10;
		for (unsigned int i = 0; i < digit; ++i)
		{
			rtn += (num[i - digit - 1] - '0') * static_cast<unsigned int>(powf(static_cast<float>(extractDigit), static_cast<float>(i)));
		}

		return rtn;
	}

	//引数の文字列から拡張子抽出
	inline std::string ExtensionExtruction(std::string str)
	{
		size_t dotPos = str.rfind('.');
		size_t cutNum = str.length() - dotPos;
		std::string ext = str.substr(dotPos + 1, cutNum);
		return ext;
	}

	//std::wstring -> std::string
	inline std::string WStringToString(std::wstring wstr) {
		// wstring → SJIS
		int iBufferSize = WideCharToMultiByte(CP_OEMCP, 0, wstr.c_str()
			, -1, (char *)NULL, 0, NULL, NULL);

		// バッファの取得
		char* cpMultiByte = new char[iBufferSize];

		// wstring → SJIS
		WideCharToMultiByte(CP_OEMCP, 0, wstr.c_str(), -1, cpMultiByte
			, iBufferSize, NULL, NULL);

		// stringの生成
		std::string oRet(cpMultiByte, cpMultiByte + iBufferSize - 1);

		// バッファの破棄
		delete[] cpMultiByte;

		// 変換結果を返す
		return(oRet);

	};

	//std::string -> std::wstring
	inline std::wstring StringToWString(const std::string& refSrc)
	{
		std::vector<wchar_t> buffer(MultiByteToWideChar(CP_ACP, 0, refSrc.c_str(), -1, nullptr, 0));
		MultiByteToWideChar(CP_ACP, 0, refSrc.c_str(), -1, &buffer.front(), static_cast<int>(buffer.size()));
		return std::wstring(buffer.begin(), buffer.end());
	}

	//現在モデルパスからTextureパスまでの相対パスを取得
	inline std::string GetRelativeTexturePath(std::string & modelPath, std::string texturePath)
	{
		size_t pathIndex1 = modelPath.rfind('/');

		std::string textureTempPath = modelPath.substr(0, pathIndex1);
		textureTempPath += '/';
		textureTempPath += texturePath;

		return textureTempPath;
	}
	//レガシーなFopen・Fclose・Fread
	namespace FileStream {

		//ファイルを開く
		inline HRESULT FileOpen(std::string path, std::string mode, FILE** stream) {
			errno_t error = 0;
			error = fopen_s(stream, path.c_str(), mode.c_str());
			if (error != 0) {
				char* errorStr = {};
				strerror_s(errorStr, 256, error);
				if (errorStr != nullptr) {
					printf_s("print error string by strerror  : %s\n", errorStr);
				}
				return E_FAIL;
			}
			return S_OK;
		};

		//ファイルを閉じる
		inline void FileClose(FILE* stream) {
			fclose(stream);
		};

		//ファイルを読み込む
		template < typename T > inline HRESULT FileRead(void* buffer, size_t elementNum, FILE* stream) {
			size_t refCount = fread_s(buffer, sizeof(&buffer), sizeof(T), elementNum, stream);
			if (refCount < 0)
				return E_FAIL;
			return S_OK;
		};

		//ファイルを読み込む
		template < typename T > inline HRESULT FileRead(void* buffer, size_t bufferSize, size_t elementNum, FILE* stream) {
			size_t refCount = fread_s(buffer, bufferSize, sizeof(T), elementNum, stream);
			if (refCount < 0)
				return E_FAIL;
			return S_OK;
		};

		template <class T = void*> size_t FileRead(void* buffer, FILE* stream, size_t size = 0, size_t readSize = 1) {

			size_t inSize = sizeof(T);
			size_t returnValue = -1;
			FILE&  streamRef = *stream;

			if (size == 0) {
				returnValue = std::fread(buffer, sizeof(T), readSize, &streamRef);
			}
			else {
				returnValue = std::fread(buffer, size, readSize, &streamRef);
			}
			return returnValue;
		}


	}

	inline float frand() {
#if 0
		unsigned res = (rand() >> 9) | 0x3f800000;
		return(*(float*)&res) - 1.0f;
#else
		return (float)rand() / RAND_MAX;
#endif
	}

	template<typename T1, typename T2 = WCHAR * >
	void Comment(const T1& t1, const T2& t2 = L"")
	{
		MessageBox(NULL, t1, t2, MB_OK);
	}

}


