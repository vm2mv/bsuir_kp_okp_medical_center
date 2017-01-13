#pragma once

#include "Patient.h"
#include "../DatabaseSQLite/DatabaseSQLite.h"


class PatientsRegistryImpl
{
public:
	explicit PatientsRegistryImpl(const std::string& connectionString, bool createIfNotExist, bool readOnly);

	PatientsRegistryImpl() = delete;
	PatientsRegistryImpl(const PatientsRegistryImpl&) = delete;
	PatientsRegistryImpl& operator = (const PatientsRegistryImpl&) = delete;

public:
	const std::vector<PatientRecord_t> GetAllPatients() const;
	void AddPatient(const Patient& patient);
	bool UpdatePatient(const PatientId_t patientId, const Patient& patient);
	bool DeletePatient(const PatientId_t patientId);
	PatientRecord_t GetPatientById(const PatientId_t patientId);
	const std::string& GetConnectionString() const;

	const std::vector<PatientRecord_t> SearchPatientsByName(const std::string& name) const;
	const std::vector<PatientRecord_t> SearchPatientsByCity(const std::string& city) const;
	const std::vector<PatientRecord_t> SearchPatientsByDiagnose(const std::string& diagnose) const;
	const std::vector<PatientRecord_t> SearchPatientsByPhone(const std::string& phone) const;

	const std::vector<PatientRecord_t> OrderPatientsByName() const;
	const std::vector<PatientRecord_t> OrderPatientsByCity() const;
	const std::vector<PatientRecord_t> OrderPatientsByBirthDate() const;

	const std::vector<PatientRecord_t> ViewNonresidentPatients(const std::string& city) const;
	const std::vector<PatientRecord_t> ViewPatientsByAgeAndDiagnose(uint16_t age, const std::string& diagnose) const;

private:
	Gender GetGenderById(const uint32_t genderId) const;
	uint32_t GetGenderId(const Gender gender);

	const std::string GetCityById(const uint64_t cityId) const;
	uint64_t GetCityId(const std::string& city);

	const std::vector<std::string> GetDiagnosesOfPatient(const uint64_t patientId) const;
	void SetDiagnosesOfPatient(const uint64_t patientId, const std::vector<std::string>& diagnoses);
	void DeleteDiagnosesOfPatient(const uint64_t patientId);

private:
	void ValidateDatabaseSQLiteSchema();
	void PrepareSqlStatements();

	const std::vector<PatientRecord_t> GetPatientsFromStatement(sqlite3_stmt* statementPtr) const;

private:
	mutable std::mutex m_mutex;
	
	DatabaseSQLite m_sqliteDatabase;
	
	SQLiteStatement_t m_insertPatientStatement;
	SQLiteStatement_t m_deletePatientStatement;
	SQLiteStatement_t m_selectPatientStatement;
	SQLiteStatement_t m_selectAllPatientsStatement;
	SQLiteStatement_t m_selectAllPatientsOrderByNameStatement;
	SQLiteStatement_t m_selectAllPatientsOrderByBirthDateStatement;
	SQLiteStatement_t m_selectAllPatientsOrderByCityStatement;
	SQLiteStatement_t m_updatePatientStatement;
	SQLiteStatement_t m_insertDiagnosisStatement;
	SQLiteStatement_t m_selectDiagnosisStatement;
	SQLiteStatement_t m_insertDiagnosisOfPatientStatement;
	SQLiteStatement_t m_selectDiagnosesOfPatientStatement;
	SQLiteStatement_t m_deleteDiagnosesOfPatientStatement;
	SQLiteStatement_t m_insertCityStatement;
	SQLiteStatement_t m_selectCityByNameStatement;
	SQLiteStatement_t m_selectCityByIdStatement;

	SQLiteStatement_t m_searchPatientsByNameStatement;
	SQLiteStatement_t m_searchPatientsByPhoneStatement;
	SQLiteStatement_t m_searchPatientsByCityStatement;
	SQLiteStatement_t m_searchPatientsByDiagnoseStatement;

	SQLiteStatement_t m_searchPatientsByCityNonresidentStatement;
	SQLiteStatement_t m_searchPatientsByAgeAndDiagnoseStatement;
};

