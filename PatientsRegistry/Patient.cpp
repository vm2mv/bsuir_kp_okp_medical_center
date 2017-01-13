#include "stdafx.h"
#include "Patient.h"


bool operator==(const Patient& lhs, const Patient& rhs)
{
	return (lhs.GetName() == rhs.GetName() &&
		lhs.GetGender() == rhs.GetGender() &&
		lhs.GetBirthDate() == rhs.GetBirthDate() &&
		lhs.GetCity() == rhs.GetCity() &&
		lhs.GetPhone() == rhs.GetPhone() &&
		lhs.GetDiagnosesAsString() == rhs.GetDiagnosesAsString());
};


bool operator!=(const Patient& lhs, const Patient& rhs)
{
	return (lhs.GetName() != rhs.GetName() ||
		lhs.GetGender() != rhs.GetGender() ||
		lhs.GetBirthDate() != rhs.GetBirthDate() ||
		lhs.GetCity() != rhs.GetCity() ||
		lhs.GetPhone() != rhs.GetPhone() ||
		lhs.GetDiagnosesAsString() != rhs.GetDiagnosesAsString());
};


Patient::Patient() :
	m_gender(Gender::Unknown),
	m_birthDate(BirthDate::GetEmptyBirthDate())
{
}


Patient::Patient(const std::string name, const Gender gender, const BirthDate& birthDate, const std::string& city,
	const std::string& phone, const std::vector<std::string>& diagnoses)
	:
	m_name(name),
	m_gender(gender),
	m_birthDate(birthDate),
	m_city(city),
	m_phone(phone),
	m_diagnoses(diagnoses)
{
}


const Patient& Patient::GetEmptyPatient()
{
	static Patient emptyPatient;

	return emptyPatient;
}


const std::string& Patient::GetName() const
{
	return m_name;
}


const Gender Patient::GetGender() const
{
	return m_gender;
}


const BirthDate& Patient::GetBirthDate() const
{
	return m_birthDate;
}


const std::string& Patient::GetCity() const
{
	return m_city;
}


const std::string& Patient::GetPhone() const
{
	return m_phone;
}


const std::vector<std::string>& Patient::GetDiagnoses() const
{
	return m_diagnoses;
}


const std::string Patient::GetDiagnosesAsString() const
{
	std::string diagnosesAsString;

	for (const auto& i : m_diagnoses)
	{
		if (!diagnosesAsString.empty())
		{
			diagnosesAsString += ", ";
		}

		diagnosesAsString += i;
	}

	return diagnosesAsString;
}
