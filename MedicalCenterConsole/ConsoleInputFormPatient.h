#pragma once

#include "ConsoleInputForm.h"

class Language;

class ConsoleInputFormPatient
{
	static const std::string FIELD_NAME;
	static const std::string FIELD_GENDER;
	static const std::string FIELD_BIRTHDATE;
	static const std::string FIELD_CITY;
	static const std::string FIELD_PHONE;
	static const std::string FIELD_DIAGNOSES;

public:
	explicit ConsoleInputFormPatient(Console& console, const Language& language,
		ConsoleInputForm::FieldCheckValuePredicat checkFieldGenderPredicate,
		ConsoleInputForm::FieldCheckValuePredicat checkFieldBirthDatePredicate);

	const std::string& GetName() const;
	const std::string& GetGender() const;
	const std::string& GetBirthDate() const;
	const std::string& GetCity() const;
	const std::string& GetPhone() const;
	const std::string& GetDiagnoses() const;

private:
	bool CheckFieldNoEmptyPredicate(const ConsoleInputForm::FieldName& fieldName,
		const std::string& fieldValue, std::string& fieldValueFormatDescription);

private:
	const Language& m_language;
	ConsoleInputForm m_consoleInputForm;
};

