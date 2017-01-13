#pragma once

#include "ConsoleInputForm.h"

class Language;

class ConsoleInputFormLogin
{
	static const std::string FIELD_USERNAME;
	static const std::string FIELD_PASSWORD;

public:
	explicit ConsoleInputFormLogin(Console& console, const Language& language);

	const std::string& GetUsername() const;
	const std::string& GetPassword() const;

private:
	ConsoleInputForm m_consoleInputForm;
};

