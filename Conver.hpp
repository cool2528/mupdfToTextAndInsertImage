#ifndef __CODING_CONVER__
#define __CODING_CONVER__
#include <string>
#include <windows.h>
namespace Coding_Conver
{
	class Conver
	{
	public:
		Conver() {};
		inline static std::string Utf8ToAnsi(const char* szBuf)
		{
			std::string resault;
			int widLen = 0;
			int MultiLen = 0;
			WCHAR* widPtr = NULL;
			CHAR* MulitPtr = NULL;
			widLen = ::MultiByteToWideChar(CP_UTF8, NULL, szBuf, -1, NULL, NULL);//获取转换后需要的空间
			widPtr = new WCHAR[widLen + 1];
			if (!widPtr)
				return resault;
			ZeroMemory(widPtr, (widLen + 1) * sizeof(WCHAR));
			if (!::MultiByteToWideChar(CP_UTF8, NULL, szBuf, -1, widPtr, widLen))
			{
				delete[] widPtr;
				widPtr = NULL;
				return resault;
			}
			MultiLen = ::WideCharToMultiByte(CP_ACP, NULL, widPtr, -1, NULL, NULL, NULL, NULL);
			if (MultiLen)
			{
				MulitPtr = new CHAR[MultiLen + 1];
				if (!MulitPtr)
				{
					delete[] widPtr;
					widPtr = NULL;
					return resault;
				}
				ZeroMemory(MulitPtr, (MultiLen + 1) * sizeof(CHAR));
				::WideCharToMultiByte(CP_ACP, NULL, widPtr, -1, MulitPtr, MultiLen, NULL, NULL);
				resault = MulitPtr;
				delete[] MulitPtr;
				MulitPtr = NULL;
			}
			delete[] widPtr;
			widPtr = NULL;
			return resault;
		}
		inline static std::string AnsiToUtf8(const char* szBuf)
		{
			std::string resault;
			int widLen = 0;
			int MultiLen = 0;
			WCHAR* widPtr = NULL;
			CHAR* MulitPtr = NULL;
			widLen = ::MultiByteToWideChar(CP_ACP, NULL, szBuf, -1, NULL, NULL);//获取转换后需要的空间
			widPtr = new WCHAR[widLen + 1];
			if (!widPtr)
				return resault;
			ZeroMemory(widPtr, (widLen + 1) * sizeof(WCHAR));
			if (!::MultiByteToWideChar(CP_ACP, NULL, szBuf, -1, widPtr, widLen))
			{
				delete[] widPtr;
				widPtr = NULL;
				return resault;
			}
			MultiLen = ::WideCharToMultiByte(CP_UTF8, NULL, widPtr, -1, NULL, NULL, NULL, NULL);
			if (MultiLen)
			{
				MulitPtr = new CHAR[MultiLen + 1];
				if (!MulitPtr)
				{
					delete[] widPtr;
					widPtr = NULL;
					return resault;
				}
				ZeroMemory(MulitPtr, (MultiLen + 1) * sizeof(CHAR));
				::WideCharToMultiByte(CP_UTF8, NULL, widPtr, -1, MulitPtr, MultiLen, NULL, NULL);
				resault = MulitPtr;
				delete[] MulitPtr;
				MulitPtr = NULL;
			}
			delete[] widPtr;
			widPtr = NULL;
			return resault;
		}
		inline static std::wstring AnsiToUincode(const char* szBuf)
		{
			std::wstring result;
			WCHAR* widePtr = NULL;
			int wideLen = -1;
			wideLen = ::MultiByteToWideChar(CP_ACP, NULL, szBuf, -1, NULL, NULL);
			widePtr = new WCHAR[wideLen + 1];
			if (widePtr)
			{
				ZeroMemory(widePtr, (wideLen + 1) * sizeof(WCHAR));
				::MultiByteToWideChar(CP_ACP, NULL, szBuf, -1, widePtr, wideLen);
				result = widePtr;
				delete[] widePtr;
				widePtr = NULL;
			}
			return result;
		}
		inline static std::string UnicodeToAnsi(const wchar_t* szBuf)
		{
			std::string result;
			CHAR* MultPtr = NULL;
			int MultLen = -1;
			MultLen = ::WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, szBuf, -1, NULL, NULL, NULL, NULL);
			MultPtr = new CHAR[MultLen + 1];
			if (MultPtr)
			{
				ZeroMemory(MultPtr, MultLen + 1);
				::WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, szBuf, -1, MultPtr, MultLen, NULL, NULL);
				result = MultPtr;
				delete[] MultPtr;
				MultPtr = NULL;
			}
			return result;
		}
	};
};
#endif