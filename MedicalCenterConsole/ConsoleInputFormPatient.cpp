#include "stdafx.h"
#include "ConsoleInputFormPatient.h"
#include "Language.h"


const std::string ConsoleInputFormPatient::FIELD_NAME = "name";
const std::string ConsoleInputFormPatient::FIELD_GENDER = "gender";
const std::string ConsoleInputFormPatient::FIELD_BIRTHDATE = "birthdate";
const std::string ConsoleInputFormPatient::FIELD_CITY = "city";
const std::string ConsoleInputFormPatient::FIELD_PHONE = "phone";
const std::string ConsoleInputFormPatient::FIELD_DIAGNOSES = "diagnoses";

namespace ph = std::placeholders;


ConsoleInputFormPatient::ConsoleInputFormPatient(Console& console, const Language& language,
	ConsoleInputForm::FieldCheckValuePredicat checkFieldGenderPredicate,
	ConsoleInputForm::FieldCheckValuePredicat checkFieldBirthDatePredicate)
	:
	m_language(language),
	m_consoleInputForm(console, language, ConsoleInputForm::InputFields
	{
		ConsoleInputForm::InputFieldItem
		{ 
			FIELD_NAME, 
			false,
			language.GetString(LanguageString::PatientFormName),
			std::bind(&ConsoleInputFormPatient::CheckFieldNoEmptyPredicate, this, ph::_1, ph::_2, ph::_3)
		},
		ConsoleInputForm::InputFieldItem
		{ 
			FIELD_GENDER,
			false,
			language.GetString(LanguageString::PatientFormGender),
			checkFieldGenderPredicate 
		},
		ConsoleInputForm::InputFieldItem
		{ 
			FIELD_BIRTHDATE, 
			false,
			language.GetString(LanguageString::PatientFormBirthDate),
			checkFieldBirthDatePredicate
		},
		ConsoleInputForm::InputFieldItem
		{ 
			FIELD_CITY, 
			false,
			language.GetString(LanguageString::PatientFormCity),
			std::bind(&ConsoleInputFormPatient::CheckFieldNoEmptyPredicate, this, ph::_1, ph::_2, ph::_3) 
		},
		ConsoleInputForm::InputFieldItem
		{ 
			FIELD_PHONE,
			false,
			language.GetString(LanguageString::PatientFormPhone),
			nullptr
		},
		ConsoleInputForm::InputFieldItem
		{ 
			FIELD_DIAGNOSES, 
			false,
			language.GetString(LanguageString::PatientFormDiagnoses),
			nullptr 
		},
	})
{
	m_consoleInputForm.DoInputFormFields();
}


const std::string& ConsoleInputFormPatient::GetName() const
{
	return m_consoleInputForm.GetInputFieldsValues().at(FIELD_NAME);
}


const std::string& ConsoleInputFormPatient::GetGender() const
{
	return m_consoleInputForm.GetInputFieldsValues().at(FIELD_GENDER);
}


const std::string& ConsoleInputFormPatient::GetBirthDate() const
{
	return m_consoleInputForm.GetInputFieldsValues().at(FIELD_BIRTHDATE);
}


const std::string& ConsoleInputFormPatient::GetCity() const
{
	return m_consoleInputForm.GetInputFieldsValues().at(FIELD_CITY);
}


const std::string& ConsoleInputFormPatient::GetPhone() const
{
	return m_consoleInputForm.GetInputFieldsValues().at(FIELD_PHONE);
}


const std::string& ConsoleInputFormPatient::GetDiagnoses() const
{
	return m_consoleInputForm.GetInputFieldsValues().at(FIELD_DIAGNOSES);
}


bool ConsoleInputFormPatient::CheckFieldNoEmptyPredicate(const ConsoleInputForm::FieldName& fieldName,
	const std::string& fieldValue, std::string& fieldValueFormatDescription)
{	
	if (fieldValue.empty())
	{
		fieldValueFormatDescription = m_language.GetString(LanguageString::EmptyValueNotAllowed);

		return false;
	}

	return true;
}
