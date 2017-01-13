#pragma once

#include "BirthDate.h"
#include "Gender.h"


class Patient
{
public:
	Patient();
	explicit Patient(const std::string name, const Gender gender, const BirthDate& birthDate, const std::string& city,
		const std::string& phone, const std::vector<std::string>& diagnoses);

	static const Patient& GetEmptyPatient();

	const std::string& GetName() const;
	const Gender GetGender() const;
	const BirthDate& GetBirthDate() const;
	const std::string& GetCity() const;
	const std::string& GetPhone() const;
	const std::vector<std::string>& GetDiagnoses() const;
	
	const std::string GetDiagnosesAsString() const;

private:
	std::string m_name;
	Gender m_gender;
	BirthDate m_birthDate;
	std::string m_city;
	std::string m_phone;
	std::vector<std::string> m_diagnoses;
};

using PatientId_t = uint64_t;
using PatientRecord_t = std::pair<PatientId_t, Patient>;

static const PatientId_t PATIENTID_EMPTY = 0;

bool operator==(const Patient& lhs, const Patient& rhs);
bool operator!=(const Patient& lhs, const Patient& rhs);
