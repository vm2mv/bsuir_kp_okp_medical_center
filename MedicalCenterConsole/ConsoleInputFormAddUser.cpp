#include "stdafx.h"
#include "ConsoleInputFormAddUser.h"
#include "Language.h"


const std::string ConsoleInputFormAddUser::FIELD_USERNAME = "username";
const std::string ConsoleInputFormAddUser::FIELD_PASSWORD = "password";
const std::string ConsoleInputFormAddUser::FIELD_CONFIRM_PASSWORD = "password_confirm";


namespace ph = std::placeholders;


ConsoleInputFormAddUser::ConsoleInputFormAddUser(Console& console, const Language& language)
	:
	m_language(language),
	m_consoleInputForm(console, language, ConsoleInputForm::InputFields
	{
		ConsoleInputForm::InputFieldItem
		{ 
			FIELD_USERNAME,
			false,
			language.GetString(LanguageString::UserFormName),
			std::bind(&ConsoleInputFormAddUser::CheckFieldValuePredicate, this, ph::_1, ph::_2, ph::_3)
		},
		ConsoleInputForm::InputFieldItem
		{
			FIELD_PASSWORD,
			true,
			language.GetString(LanguageString::UserFormPassword),
			nullptr 
		},
		ConsoleInputForm::InputFieldItem
		{
			FIELD_CONFIRM_PASSWORD, 
			true,
			language.GetString(LanguageString::UserFormConfirmPassword),
			nullptr 
		},
	})
{
	m_consoleInputForm.DoInputFormFields();
}


bool ConsoleInputFormAddUser::CheckFieldValuePredicate(const ConsoleInputForm::FieldName& fieldName,
	const std::string& fieldValue, std::string& fieldValueFormatDescription)
{
	if (fieldName == FIELD_USERNAME)
	{
		if (fieldValue.empty())
		{
			fieldValueFormatDescription = m_language.GetString(LanguageString::EmptyValueNotAllowed);

			return false;
		}
	}

	return true;
}


const std::string& ConsoleInputFormAddUser::GetUsername() const
{
	return m_consoleInputForm.GetInputFieldsValues().at(FIELD_USERNAME);
}


const std::string& ConsoleInputFormAddUser::GetPassword() const
{
	return m_consoleInputForm.GetInputFieldsValues().at(FIELD_PASSWORD);
}


const std::string& ConsoleInputFormAddUser::GetPasswordConfirm() const
{
	return m_consoleInputForm.GetInputFieldsValues().at(FIELD_CONFIRM_PASSWORD);
}
