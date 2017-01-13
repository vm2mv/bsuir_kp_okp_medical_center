#pragma once

#include "ConsoleInputForm.h"

class Language;

class ConsoleInputFormAddUser
{
	static const std::string FIELD_USERNAME;
	static const std::string FIELD_PASSWORD;
	static const std::string FIELD_CONFIRM_PASSWORD;

public:
	explicit ConsoleInputFormAddUser(Console& console, const Language& language);

	const std::string& GetUsername() const;
	const std::string& GetPassword() const;
	const std::string& GetPasswordConfirm() const;

private:
	bool CheckFieldValuePredicate(const ConsoleInputForm::FieldName& fieldName,
		const std::string& fieldValue, std::string& fieldValueFormatDescription);

private:
	const Language& m_language;
	ConsoleInputForm m_consoleInputForm;
};

