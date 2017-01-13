#pragma once


class Console;
class Language;


class Utils
{
public:
	static bool RequestExistFilePath(Console& console, const Language& language, std::string& filePath);
	static bool RequestFilePath(Console& console, const Language& language, bool checkExist, std::string& filePath);
	static bool RequestExistFilePathWithDefaultValue(Console& console, const Language& language,
		const std::string& defaultPath,	std::string& filePath);
	static bool RequestFilePathWithDefaultValue(Console& console, const Language& language, bool checkExist,
		const std::string& defaultPath,	std::string& filePath);

	static const std::string ConvertStringFromLocalCodepageToUtf8(const std::string& str);

private:
	static bool CheckExistPatientsFile(Console& console, const Language& language, const std::string& filePath);
};

