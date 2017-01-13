#include "stdafx.h"
#include "PatientsController.h"
#include "Console.h"
#include "ConsoleInputFormPatient.h"
#include "Utils.h"
#include "Language.h"

namespace ph = std::placeholders;


const std::unordered_map<std::string, Gender> PatientsController::GENDER_STRINGS_MAP = {
	{ u8"М", Gender::Male },
	{ u8"м", Gender::Male },
	{ u8"M", Gender::Male },
	{ u8"m", Gender::Male },
	{ u8"Ж", Gender::Feemale },
	{ u8"ж", Gender::Feemale },
	{ u8"F", Gender::Feemale },
	{ u8"f", Gender::Feemale },
	{ u8"Т", Gender::Schemale },
	{ u8"т", Gender::Schemale },
	{ u8"S", Gender::Schemale },
	{ u8"s", Gender::Schemale },
};


PatientsController::PatientsController(Console& console, const Language& language,
	std::shared_ptr<PatientsRegistry> patientsRegistry) 
	:
	m_console(console),
	m_language(language),
	m_patientsRegistry(patientsRegistry)
{
}


PatientsController::~PatientsController()
{
}


void PatientsController::CreateFile()
{
	std::string filePath;

	if (!Utils::RequestFilePath(m_console, m_language, false, filePath))
	{
		return;
	}

	if (boost::filesystem::exists(filePath))
	{
		m_console.PrintErrorWithNewLine(GetLanguage().GetString(LanguageString::PatientsFileExistAlready));

		return;
	}

	m_patientsRegistry = PatientsRegistry::CreatePatientsRegistry(filePath);

	m_console.PrintYellowWithNewLine(GetLanguage().GetString(LanguageString::PatientsFileCreated));
}


void PatientsController::OpenFileForReadAndWrite()
{
	OpenFile(false);
}


void PatientsController::OpenFileForReadOnly()
{
	OpenFile(true);
}


void PatientsController::OpenFile(bool readOnly)
{
	std::string filePath;

	if (!Utils::RequestFilePath(m_console, m_language, true, filePath))
	{
		return;
	}

	m_patientsRegistry = PatientsRegistry::OpenPatientsRegistry(filePath, readOnly);

	m_console.PrintYellowWithNewLine(GetLanguage().GetString(LanguageString::PatientsFileOpened));
}


void PatientsController::CloseFile()
{
	if (m_patientsRegistry != nullptr)
	{
		m_patientsRegistry.reset();

		m_console.PrintYellowWithNewLine(GetLanguage().GetString(LanguageString::PatientsFileClosed));
	}
	else
	{
		m_console.PrintYellowWithNewLine(GetLanguage().GetString(LanguageString::NotFoundOpenedPatientsFile));
	}
}


void PatientsController::RemoveFile()
{
	std::string filePath;

	if (!Utils::RequestFilePath(m_console, m_language, true, filePath))
	{
		return;
	}

	if (m_patientsRegistry != nullptr)
	{
		if (boost::filesystem::equivalent(m_patientsRegistry->GetConnectionString(), filePath))
		{
			m_console.PrintErrorWithNewLine(GetLanguage().GetString(LanguageString::CantRemoveOpenedPatientsFile));

			return;
		}
	}

	if (!m_console.RequestInputYesNo(GetLanguage().GetString(LanguageString::RemoveFileConfirm), false))
	{
		return;
	}

	try
	{
		boost::filesystem::remove(filePath);

		m_console.PrintYellowWithNewLine(GetLanguage().GetString(LanguageString::PatientsFileRemoved));
	}
	catch (const boost::filesystem::filesystem_error&)
	{
		m_console.PrintErrorWithNewLine(GetLanguage().GetString(LanguageString::CantRemovePatientsFile));
	}
}


bool PatientsController::CheckFileIsOpened() const
{
	if (m_patientsRegistry != nullptr)
	{
		return true;
	}
	else
	{
		m_console.PrintErrorWithNewLine(GetLanguage().GetString(LanguageString::NotFoundOpenedPatientsFile));

		return false;
	}
}


void PatientsController::ViewAllPatients() const
{
	if (!CheckFileIsOpened())
	{
		return;
	}

	PrintPatients(m_patientsRegistry->GetAllPatients());
}


void PatientsController::AddPatient()
{
	if (!CheckFileIsOpened())
	{
		return;
	}

	m_patientsRegistry->AddPatient(CreatePatientForm());

	m_console.PrintYellowWithNewLine(GetLanguage().GetString(LanguageString::PatientAdded));
}


void PatientsController::EditPatient()
{
	if (!CheckFileIsOpened())
	{
		return;
	}

	uint64_t patientId;

	if (!RequestPatientId(patientId))
	{
		return;
	}

	if (!m_console.RequestInputYesNo(GetLanguage().GetString(LanguageString::PatientEditConfirm), true))
	{
		return;
	}

	if (m_patientsRegistry->UpdatePatient(patientId, CreatePatientForm()))
	{
		m_console.PrintYellowWithNewLine(GetLanguage().GetString(LanguageString::PatientChanged));
	}
	else
	{
		m_console.PrintErrorWithNewLine(GetLanguage().GetString(LanguageString::CantChangePatient));
	}
}


void PatientsController::RemovePatient()
{
	if (!CheckFileIsOpened())
	{
		return;
	}

	uint64_t patientId;

	if (!RequestPatientId(patientId))
	{
		return;
	}

	if (!m_console.RequestInputYesNo(GetLanguage().GetString(LanguageString::PatientRemoveConfirm), false))
	{
		return;
	}

	if (m_patientsRegistry->DeletePatient(patientId))
	{
		m_console.PrintYellowWithNewLine(GetLanguage().GetString(LanguageString::PatientRemoved));
	}
	else
	{
		m_console.PrintErrorWithNewLine(GetLanguage().GetString(LanguageString::CantRemovePatient));
	}
}


void PatientsController::SearchPatientsByName() const
{
	if (!CheckFileIsOpened())
	{
		return;
	}

	m_console.PrintWhite(GetLanguage().GetString(LanguageString::SearchPatientName));
	const std::string searchName = m_console.RequestInputString();

	PrintPatients(m_patientsRegistry->SearchPatientsByName(searchName));
}


void PatientsController::SearchPatientsByCity() const
{
	if (!CheckFileIsOpened())
	{
		return;
	}

	m_console.PrintWhite(GetLanguage().GetString(LanguageString::SearchPatientCity));
	const std::string searchCity = m_console.RequestInputString();

	PrintPatients(m_patientsRegistry->SearchPatientsByCity(searchCity));
}


void PatientsController::SearchPatientsByDiagnose() const
{
	if (!CheckFileIsOpened())
	{
		return;
	}

	m_console.PrintWhite(GetLanguage().GetString(LanguageString::SearchPatientDiagnose));
	const std::string searchDiagnose = m_console.RequestInputString();

	PrintPatients(m_patientsRegistry->SearchPatientsByDiagnose(searchDiagnose));
}


void PatientsController::SearchPatientsByPhone() const
{
	if (!CheckFileIsOpened())
	{
		return;
	}

	m_console.PrintWhite(GetLanguage().GetString(LanguageString::SearchPatientPhone));
	const std::string searchPhone = m_console.RequestInputString();

	PrintPatients(m_patientsRegistry->SearchPatientsByPhone(searchPhone));
}


void PatientsController::OrderPatientsByName() const
{
	if (!CheckFileIsOpened())
	{
		return;
	}

	PrintPatients(m_patientsRegistry->OrderPatientsByName());
}


void PatientsController::OrderPatientsByCity() const
{
	if (!CheckFileIsOpened())
	{
		return;
	}

	PrintPatients(m_patientsRegistry->OrderPatientsByCity());
}


void PatientsController::OrderPatientsByBirthDate() const
{
	if (!CheckFileIsOpened())
	{
		return;
	}

	PrintPatients(m_patientsRegistry->OrderPatientsByBirthDate());
}


void PatientsController::ViewNonresidentPatients() const
{
	if (!CheckFileIsOpened())
	{
		return;
	}

	m_console.PrintWhite(GetLanguage().GetString(LanguageString::SearchPatientCity));
	const std::string searchCity = m_console.RequestInputString();

	PrintPatients(m_patientsRegistry->ViewNonresidentPatients(searchCity));
}


void PatientsController::ViewPatientsByAgeAndDiagnose() const
{
	if (!CheckFileIsOpened())
	{
		return;
	}

	m_console.PrintWhite(GetLanguage().GetString(LanguageString::SearchPatientDiagnose));
	const std::string searchDiagnose = m_console.RequestInputString();

	m_console.PrintWhite(GetLanguage().GetString(LanguageString::SearchPatientAge));
	const uint16_t searchAge = static_cast<uint16_t>(m_console.RequestInputInteger());

	PrintPatients(m_patientsRegistry->ViewPatientsByAgeAndDiagnose(searchAge, searchDiagnose));
}


const Patient PatientsController::CreatePatientForm() const
{
	ConsoleInputFormPatient inputFormPatient(m_console, m_language,
		std::bind(&PatientsController::CheckFormFieldGenderString, this, ph::_1, ph::_2, ph::_3),
		std::bind(&PatientsController::CheckFormFieldBirthDateString, this, ph::_1, ph::_2, ph::_3));

	Patient newPatient(
		inputFormPatient.GetName(),
		GetGenderByInputString(inputFormPatient.GetGender()),
		GetBirthDateByInputString(inputFormPatient.GetBirthDate()),
		inputFormPatient.GetCity(),
		inputFormPatient.GetPhone(),
		GetDiagnosesByInputString(inputFormPatient.GetDiagnoses()));

	return newPatient;
}


bool PatientsController::RequestPatientId(uint64_t& patientId) const
{
	m_console.PrintWhite(GetLanguage().GetString(LanguageString::EnterPatientId));
	uint64_t requestPatientId = m_console.RequestInputInteger();

	const PatientRecord_t existPatientRecord = m_patientsRegistry->GetPatientById(requestPatientId);

	if (existPatientRecord.second == Patient::GetEmptyPatient())
	{
		m_console.PrintErrorWithNewLine(GetLanguage().GetString(LanguageString::PatientIdNotFound));

		return false;
	}

	m_console.PrintInfoWithNewLine(GetLanguage().GetString(LanguageString::PatientFound));
	PrintPatient(existPatientRecord);

	patientId = requestPatientId;

	return true;
}


void PatientsController::PrintPatients(const std::vector<PatientRecord_t>& patientsRecords) const
{
	if (patientsRecords.empty())
	{
		m_console.PrintYellowWithNewLine(GetLanguage().GetString(LanguageString::NotFoundAnyRecord));

		return;
	}

	m_console.PrintWhiteWithNewLine("--------------------------------------------------------------------------------");

	for (const auto& patientRecord : patientsRecords)
	{
		PrintPatient(patientRecord);
	}

	m_console.PrintWhiteWithNewLine("--------------------------------------------------------------------------------");
	m_console.PrintWhite(GetLanguage().GetString(LanguageString::FoundRecords));
	m_console.PrintWhite(u8"%zu\r\n", patientsRecords.size());
}


void PatientsController::PrintPatient(const PatientRecord_t& patientRecord) const
{
	const uint64_t patientId = patientRecord.first;
	const Patient& patient = patientRecord.second;

	m_console.PrintInfoWithNewLine("%s: %I64u; %s: %s; %s: %s; %s: %s; %s: %s; %s: %s; %s: %s",
		GetLanguage().GetString(LanguageString::PatientId),
		patientId,
		GetLanguage().GetString(LanguageString::PatientName),
		patient.GetName().c_str(),
		GetLanguage().GetString(LanguageString::PatientGender),
		GetGenderDisplayString(patient.GetGender()).c_str(),
		GetLanguage().GetString(LanguageString::PatientBirthDate),
		GetBirthDateDisplayString(patient.GetBirthDate()).c_str(),
		GetLanguage().GetString(LanguageString::PatientCity),
		patient.GetCity().c_str(),
		GetLanguage().GetString(LanguageString::PatientPhone),
		patient.GetPhone().c_str(),
		GetLanguage().GetString(LanguageString::PatientDiagnoses),
		patient.GetDiagnosesAsString().c_str()
	);
}


const std::string PatientsController::GetGenderDisplayString(const Gender gender) const
{
	switch (gender)
	{
	case Gender::Male:
		return GetLanguage().GetString(LanguageString::GenderMale);
		break;

	case Gender::Feemale:
		return GetLanguage().GetString(LanguageString::GenderFeemale);
		break;

	case Gender::Schemale:
		return GetLanguage().GetString(LanguageString::GenderShemale);
		break;
	}

	throw std::logic_error("unknown gender");
}


const std::string PatientsController::GetBirthDateDisplayString(const BirthDate& birthDate) const
{
	char birthDateAsString[24] = { 0 };
	sprintf_s(birthDateAsString, _countof(birthDateAsString), "%02u.%02u.%04u",
		birthDate.GetDay(), birthDate.GetMonth(), birthDate.GetYear());

	return std::string(birthDateAsString);
}


const BirthDate PatientsController::GetBirthDateByInputString(const std::string& birthDateString) const
{
	// date format: 31.12.2016
	static const size_t DATE_STRING_LENGTH = 10;
	static const char DATE_SEPARATOR_CHAR = '.';

	if (birthDateString.size() != DATE_STRING_LENGTH || 
		birthDateString[2] != DATE_SEPARATOR_CHAR || birthDateString[5] != DATE_SEPARATOR_CHAR)
	{
		throw std::invalid_argument("Invalid birth date format");
	}

	return BirthDate(
		static_cast<int16_t>(std::stoul(birthDateString.substr(6, 4))),
		static_cast<int8_t>(std::stoul(birthDateString.substr(3, 2))),
		static_cast<int8_t>(std::stoul(birthDateString.substr(0, 2))));
}


bool PatientsController::CheckFormFieldBirthDateString(const ConsoleInputForm::FieldName& fieldName,
	const std::string& fieldValue, std::string& fieldValueFormatDescription) const
{
	try
	{
		GetBirthDateByInputString(fieldValue);
	}
	catch (const std::invalid_argument&)
	{
		return false;
	}

	return true;
}


const std::vector<std::string> PatientsController::GetDiagnosesByInputString(const std::string& diagnosesString) const
{
	std::vector<std::string> diagnoses;

	boost::char_separator<char> sep(",");
	boost::tokenizer<boost::char_separator<char>, std::string::const_iterator, std::string>	tokens(diagnosesString, sep);

	for (const auto& t : tokens) 
	{
		diagnoses.push_back(boost::trim_copy(t));
	}

	return diagnoses;
}


const Gender PatientsController::GetGenderByInputString(const std::string& genderString) const
{
	auto it = GENDER_STRINGS_MAP.find(genderString);

	if (it == GENDER_STRINGS_MAP.cend())
	{
		throw std::invalid_argument("Invalid gender string");
	}

	return it->second;
}


bool PatientsController::CheckFormFieldGenderString(const ConsoleInputForm::FieldName& fieldName,
	const std::string& fieldValue, std::string& fieldValueFormatDescription) const
{
	if (GENDER_STRINGS_MAP.find(fieldValue) != GENDER_STRINGS_MAP.cend())
	{
		return true;
	}
	else
	{
		return false;
	}
}


const Language& PatientsController::GetLanguage() const
{
	return m_language;
}
