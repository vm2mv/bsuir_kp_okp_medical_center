#pragma once

class Language;

class Console
{
	enum ConsoleColors
	{
		CC_BLACK = 0x0000,
		CC_RED = 0x0004,
		CC_GREEN = 0x0002,
		CC_BROWN = 0x0006,
		CC_BLUE = 0x0001,
		CC_MAGENTA = 0x0005,
		CC_CYAN = 0x0003,
		CC_GRAY = 0x0007,
		CC_DARKGRAY = 0x0008,
		CC_LIGHTRED = 0x000C,
		CC_LIGHTGREEN = 0x000A,
		CC_YELLOW = 0x000E,
		CC_LIGHTBLUE = 0x0009,
		CC_LIGHTMAGENTA = 0x000D,
		CC_LIGHTCYAN = 0x000B,
		CC_WHITE = 0x000F
	};

	static const size_t FORMAT_TEXT_BUFFER_SIZE;

public:
	explicit Console(const Language& language);

	void PrintInfo(const char* msg, ...);
	void PrintWhite(const char* msg, ...);
	void PrintYellow(const char* msg, ...);
	void PrintError(const char* msg, ...);

	void PrintInfoWithNewLine(const char* msg, ...);
	void PrintWhiteWithNewLine(const char* msg, ...);
	void PrintYellowWithNewLine(const char* msg, ...);
	void PrintErrorWithNewLine(const char* msg, ...);

	bool RequestInputYesNo(const char* requestText, bool defaultValue);
	const std::string RequestInputString();
	const std::string RequestInputNonEmptyString();
	const uint64_t RequestInputInteger();
	const std::string RequestInputPassword();

private:
	const Language& GetLanguage() const;

	void PrintTextWithColor(WORD color, bool newLine, const char* msg, va_list args);

	void SetConsoleColor(WORD color);
	void ResetConsoleColor();

private:
	const Language& m_language;
	HANDLE m_consoleHandle;
	WORD m_defaultAttributes;
	std::wstring_convert<std::codecvt_utf8<wchar_t>> m_utf8Conveter;
};

