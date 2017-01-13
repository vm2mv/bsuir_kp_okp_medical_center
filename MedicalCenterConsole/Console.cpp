#include "stdafx.h"
#include "Console.h"
#include "Utils.h"
#include "Language.h"

const size_t Console::FORMAT_TEXT_BUFFER_SIZE = 4096;


Console::Console(const Language& language) :
	m_language(language)
{
	m_consoleHandle = ::GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	
	if (::GetConsoleScreenBufferInfo(m_consoleHandle, &csbi))
	{
		m_defaultAttributes = csbi.wAttributes;
	}
	else
	{
		m_defaultAttributes = CC_WHITE;
	}
}


void Console::SetConsoleColor(WORD color)
{
	WORD attr = m_defaultAttributes;
	attr &= 0xFFF0;
	attr |= color;
	::SetConsoleTextAttribute(m_consoleHandle, attr);
}


void Console::ResetConsoleColor()
{
	::SetConsoleTextAttribute(m_consoleHandle, m_defaultAttributes);
}


void Console::PrintTextWithColor(WORD color, bool newLine, const char* msg, va_list args)
{
	char formattedLine[FORMAT_TEXT_BUFFER_SIZE] = { 0 };
	vsnprintf_s((char*)formattedLine, _countof(formattedLine), _TRUNCATE, msg, args);

	SetConsoleColor(color);

	printf("%s", formattedLine);

	if (newLine)
	{
		printf("\r\n");
	}

	ResetConsoleColor();
}


void Console::PrintInfo(const char* msg, ...)
{
	va_list args;
	va_start(args, msg);

	PrintTextWithColor(CC_GRAY, false, msg, args);

	va_end(args);
}


void Console::PrintWhite(const char* msg, ...)
{
	va_list args;
	va_start(args, msg);

	PrintTextWithColor(CC_WHITE, false, msg, args);

	va_end(args);
}


void Console::PrintYellow(const char* msg, ...)
{
	va_list args;
	va_start(args, msg);

	PrintTextWithColor(CC_YELLOW, false, msg, args);

	va_end(args);
}


void Console::PrintError(const char* msg, ...)
{
	va_list args;
	va_start(args, msg);

	PrintTextWithColor(CC_LIGHTRED, false, msg, args);

	va_end(args);
}


void Console::PrintInfoWithNewLine(const char* msg, ...)
{
	va_list args;
	va_start(args, msg);

	PrintTextWithColor(CC_GRAY, true, msg, args);

	va_end(args);
}


void Console::PrintWhiteWithNewLine(const char* msg, ...)
{
	va_list args;
	va_start(args, msg);

	PrintTextWithColor(CC_WHITE, true, msg, args);

	va_end(args);
}


void Console::PrintYellowWithNewLine(const char* msg, ...)
{
	va_list args;
	va_start(args, msg);

	PrintTextWithColor(CC_YELLOW, true, msg, args);

	va_end(args);
}


void Console::PrintErrorWithNewLine(const char* msg, ...)
{
	va_list args;
	va_start(args, msg);

	PrintTextWithColor(CC_LIGHTRED, true, msg, args);

	va_end(args);
}


bool Console::RequestInputYesNo(const char* requestText, bool defaultValue)
{
	bool reponseValue = defaultValue;

	PrintYellow(requestText);
	PrintYellow(" (y/n) [");
	PrintYellow(defaultValue ? "y" : "n");
	PrintYellow("]: ");

	SetConsoleColor(CC_YELLOW);

	while (true)
	{
		std::string inputString;
		getline(std::cin, inputString);

		if (inputString.empty())
		{
			break;
		}
		
		char responseSymbol = inputString.front();

		// responseSymbol is in local codepage
		if (responseSymbol == 'Y' || responseSymbol == 'y' || responseSymbol == 'Ä' || responseSymbol == 'ä')
		{
			reponseValue = true;

			break;
		}
		else if (responseSymbol == 'N' || responseSymbol == 'n' || responseSymbol == 'Í' || responseSymbol == 'í')
		{
			reponseValue = false;

			break;
		}
	}

	ResetConsoleColor();

	return reponseValue;
}


const std::string Console::RequestInputString()
{
	SetConsoleColor(CC_GRAY);

	std::string inputString;
	getline(std::cin, inputString);

	ResetConsoleColor();

	return Utils::ConvertStringFromLocalCodepageToUtf8(inputString);
}


const std::string Console::RequestInputNonEmptyString()
{
	while (true)
	{
		const std::string inputValue = RequestInputString();

		if (inputValue.empty())
		{
			continue;
		}

		return inputValue;
	}
}


const std::string Console::RequestInputPassword()
{
	static const char RETURN_SYMBOL_CODE = 13;

	SetConsoleColor(CC_GRAY);

	std::string inputString;

	int ch = _getch();
	
	while (ch != RETURN_SYMBOL_CODE)
	{
		inputString.push_back(ch);
		std::cout << '*';
	
		ch = _getch();
	}
	
	std::cout << "\r\n";

	ResetConsoleColor();

	return Utils::ConvertStringFromLocalCodepageToUtf8(inputString);
}


const uint64_t Console::RequestInputInteger()
{
	while (true)
	{
		const std::string inputValue = RequestInputNonEmptyString();

		uint64_t inputValueInteger;

		try
		{
			inputValueInteger = std::stoull(inputValue);
		}
		catch (const std::invalid_argument&)
		{
			PrintErrorWithNewLine(GetLanguage().GetString(LanguageString::IncorrectValue));

			continue;
		}
		catch (const std::out_of_range&)
		{
			PrintErrorWithNewLine(GetLanguage().GetString(LanguageString::IncorrectValue));

			continue;
		}

		return inputValueInteger;
	}
}


const Language& Console::GetLanguage() const
{
	return m_language;
}
