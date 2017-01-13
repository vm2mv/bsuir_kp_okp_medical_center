#include "stdafx.h"
#include "ConsoleInputFormLogin.h"
#include "Language.h"


const std::string ConsoleInputFormLogin::FIELD_USERNAME = "username";
const std::string ConsoleInputFormLogin::FIELD_PASSWORD = "password";


ConsoleInputFormLogin::ConsoleInputFormLogin(Console& console, const Language& language)
	:
	m_consoleInputForm(console, language, ConsoleInputForm::InputFields
	{
		ConsoleInputForm::InputFieldItem
		{
			FIELD_USERNAME, 
			false,
			language.GetString(LanguageString::LoginName),
			nullptr 
		},
		ConsoleInputForm::InputFieldItem
		{
			FIELD_PASSWORD, 
			true,
			language.GetString(LanguageString::LoginPasword),
			nullptr 
		},
	})
{
	m_consoleInputForm.DoInputFormFields();
}


const std::string& ConsoleInputFormLogin::GetUsername() const
{
	return m_consoleInputForm.GetInputFieldsValues().at(FIELD_USERNAME);
}


const std::string& ConsoleInputFormLogin::GetPassword() const
{
	return m_consoleInputForm.GetInputFieldsValues().at(FIELD_PASSWORD);
}
