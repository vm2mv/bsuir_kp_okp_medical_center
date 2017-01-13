#include "stdafx.h"
#include "ConsoleInputForm.h"
#include "Console.h"
#include "Language.h"


ConsoleInputForm::ConsoleInputForm(Console& console, const Language& language, const InputFields& inputFields) :
	m_console(console),
	m_language(language),
	m_inputFields(inputFields)
{
}


const ConsoleInputForm::InputFieldsValues& ConsoleInputForm::GetInputFieldsValues() const
{
	return m_fieldsValues;
}


void ConsoleInputForm::DoInputFormFields()
{
	for (const auto& fieldItem : m_inputFields)
	{
		DoInputFormField(fieldItem);
	}
}


void ConsoleInputForm::DoInputFormField(const InputFieldItem& fieldItem)
{
	while (true)
	{
		m_console.PrintWhite(fieldItem.display.c_str());
		m_console.PrintWhite(": ");

		std::string fieldValue;

		if (fieldItem.isPassword)
		{
			fieldValue = m_console.RequestInputPassword();
		}
		else
		{
			fieldValue = m_console.RequestInputString();
		}

		if (!CheckFieldValue(fieldItem.predicat, fieldItem.name, fieldValue))
		{
			continue;
		}

		m_fieldsValues.emplace(fieldItem.name, fieldValue);

		break;
	}
}


bool ConsoleInputForm::CheckFieldValue(const FieldCheckValuePredicat& predicat,
	const std::string& fieldName, const std::string& fieldValue)
{
	if (predicat == nullptr)
	{
		return true;
	}

	std::string fieldValueFormatDescription;

	if (predicat(fieldName, fieldValue, fieldValueFormatDescription))
	{
		return true;
	}

	m_console.PrintErrorWithNewLine(m_language.GetString(LanguageString::IncorrectValue));

	if (!fieldValueFormatDescription.empty())
	{
		m_console.PrintInfo(u8"\t%s\r\n", fieldValueFormatDescription.c_str());
	}

	return false;
}
