#pragma once

#include "../PatientsRegistry/PatientsRegistry.h"
#include "ConsoleInputForm.h"

class Console;
class Language;


class PatientsController
{
	static const std::unordered_map<std::string, Gender> GENDER_STRINGS_MAP;

public:
	explicit PatientsController(Console& console, const Language& language,
		std::shared_ptr<PatientsRegistry> patientsRegistry);
	~PatientsController();

	PatientsController() = delete;
	PatientsController(const PatientsController&) = delete;
	PatientsController& operator=(const PatientsController&) = delete;

	void OpenFileForReadAndWrite();
	void OpenFileForReadOnly();
	void CreateFile();
	void CloseFile();
	void RemoveFile();

	void ViewAllPatients() const;
	void AddPatient();
	void EditPatient();
	void RemovePatient();

	void SearchPatientsByName() const;
	void SearchPatientsByCity() const;
	void SearchPatientsByDiagnose() const;
	void SearchPatientsByPhone() const;

	void OrderPatientsByName() const;
	void OrderPatientsByCity() const;
	void OrderPatientsByBirthDate() const;

	void ViewNonresidentPatients() const;
	void ViewPatientsByAgeAndDiagnose() const;

private:
	const Language& GetLanguage() const;
	void OpenFile(bool readOnly);
	bool CheckFileIsOpened() const;
	void PrintPatients(const std::vector<PatientRecord_t>& patientsRecords) const;
	void PrintPatient(const PatientRecord_t& patientRecord) const;
	const Patient CreatePatientForm() const;
	bool RequestPatientId(uint64_t& patientId) const;

	const std::string GetGenderDisplayString(const Gender gender) const;
	const std::string GetBirthDateDisplayString(const BirthDate& birthDate) const;

	const Gender GetGenderByInputString(const std::string& genderString) const;
	const BirthDate GetBirthDateByInputString(const std::string& birthDateString) const;
	const std::vector<std::string> GetDiagnosesByInputString(const std::string& diagnosesString) const;

	bool CheckFormFieldGenderString(const ConsoleInputForm::FieldName& fieldName,
		const std::string& fieldValue, std::string& fieldValueFormatDescription) const;
	bool CheckFormFieldBirthDateString(const ConsoleInputForm::FieldName& fieldName,
		const std::string& fieldValue, std::string& fieldValueFormatDescription) const;

private:
	const Language& m_language;
	Console& m_console;
	std::shared_ptr<PatientsRegistry> m_patientsRegistry;
};

