#include "stdafx.h"
#include "Utils.h"
#include "Console.h"
#include "Language.h"


bool Utils::RequestExistFilePath(Console& console, const Language& language, std::string& filePath)
{
	return RequestFilePath(console, language, true, filePath);
}


bool Utils::RequestFilePath(Console& console, const Language& language, bool checkExist, std::string& filePath)
{
	return RequestFilePathWithDefaultValue(console, language, checkExist, "", filePath);
}


bool Utils::RequestExistFilePathWithDefaultValue(Console& console, const Language& language,
	const std::string& defaultPath, std::string& filePath)
{
	return RequestFilePathWithDefaultValue(console, language, true, defaultPath, filePath);
}


bool Utils::RequestFilePathWithDefaultValue(Console& console, const Language& language, bool checkExist,
	const std::string& defaultPath, std::string& filePath)
{
	while (true)
	{
		console.PrintYellow(language.GetString(LanguageString::EnterPathToPatientsDatabase));

		if (!defaultPath.empty())
		{
			console.PrintYellow(u8" [%s]", defaultPath.c_str());
		}

		console.PrintYellow(": ");

		std::string inputValue = console.RequestInputString();

		if (inputValue.empty() && !defaultPath.empty())
		{
			inputValue = defaultPath;
		}

		if (checkExist &&
			!CheckExistPatientsFile(console, language, inputValue))
		{
			return false;
		}

		filePath = inputValue;

		return true;
	}

	return false;
}


bool Utils::CheckExistPatientsFile(Console& console, const Language& language, const std::string& filePath)
{
	try
	{
		if (!boost::filesystem::exists(filePath))
		{
			console.PrintErrorWithNewLine(language.GetString(LanguageString::PatientsDatabaseFileNotFound));

			return false;
		}
	}
	catch (const boost::filesystem::filesystem_error&)
	{
		console.PrintErrorWithNewLine(language.GetString(LanguageString::CantCheckExistPatientsDatabase));

		return false;
	}

	return true;
}


const std::string Utils::ConvertStringFromLocalCodepageToUtf8(const std::string& str)
{
	int iUnicodeLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), (int)str.size(), NULL, 0);

	if (iUnicodeLen <= 0)
	{
		return "";
	}

	std::vector<wchar_t> unicodeString(iUnicodeLen + 1);
	iUnicodeLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), (int)str.size(),
		unicodeString.data(), (int)iUnicodeLen);

	if (iUnicodeLen <= 0)
	{
		return "";
	}

	unicodeString[iUnicodeLen] = 0;

	int iUtf8Len = ::WideCharToMultiByte(CP_UTF8, 0, unicodeString.data(), iUnicodeLen, NULL, 0, NULL, NULL);

	if (iUtf8Len <= 0)
	{
		return "";
	}

	std::vector<char> utf8String(iUtf8Len + 1);
	iUtf8Len = ::WideCharToMultiByte(CP_UTF8, 0, unicodeString.data(), iUnicodeLen, utf8String.data(), iUtf8Len, NULL, NULL);

	if (iUtf8Len <= 0)
	{
		return "";
	}

	utf8String[iUtf8Len] = 0;


	return utf8String.data();
}
