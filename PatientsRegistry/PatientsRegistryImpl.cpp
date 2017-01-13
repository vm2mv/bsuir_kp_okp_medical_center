#include "stdafx.h"
#include "PatientsRegistryImpl.h"
#include "../DatabaseSQLite/DatabaseSQLiteException.h"
#include "DatabaseSchemaSql.h"


PatientsRegistryImpl::PatientsRegistryImpl(const std::string& connectionString, bool createIfNotExist, bool readOnly) :
	m_sqliteDatabase(connectionString, createIfNotExist, readOnly)
{
	if (!readOnly)
	{
		ValidateDatabaseSQLiteSchema();
	}

	PrepareSqlStatements();
}


void PatientsRegistryImpl::PrepareSqlStatements()
{
	m_insertPatientStatement = m_sqliteDatabase.CreateStatement(DatabaseSchemaSql::SQL_INSERT_PATIENT);
	m_updatePatientStatement = m_sqliteDatabase.CreateStatement(DatabaseSchemaSql::SQL_UPDATE_PATIENT);
	m_deletePatientStatement = m_sqliteDatabase.CreateStatement(DatabaseSchemaSql::SQL_DELETE_PATIENT);
	m_selectPatientStatement = m_sqliteDatabase.CreateStatement(DatabaseSchemaSql::SQL_SELECT_PATIENT_BY_ID);
	m_selectAllPatientsStatement = m_sqliteDatabase.CreateStatement(DatabaseSchemaSql::SQL_SELECT_ALLS_PATIENTS);

	m_selectAllPatientsOrderByNameStatement =
		m_sqliteDatabase.CreateStatement(DatabaseSchemaSql::SQL_SELECT_ALLS_PATIENTS_ORDER_BY_NAME);
	m_selectAllPatientsOrderByBirthDateStatement =
		m_sqliteDatabase.CreateStatement(DatabaseSchemaSql::SQL_SELECT_ALLS_PATIENTS_ORDER_BY_BIRTHDATE);
	m_selectAllPatientsOrderByCityStatement =
		m_sqliteDatabase.CreateStatement(DatabaseSchemaSql::SQL_SELECT_ALLS_PATIENTS_ORDER_BY_CITY);

	m_selectDiagnosesOfPatientStatement =
		m_sqliteDatabase.CreateStatement(DatabaseSchemaSql::SQL_SELECT_DIAGNOSES_BY_PATIENT_ID);

	m_insertDiagnosisStatement = m_sqliteDatabase.CreateStatement(DatabaseSchemaSql::SQL_INSERT_DIAGNOSE);
	m_selectDiagnosisStatement = m_sqliteDatabase.CreateStatement(DatabaseSchemaSql::SQL_SELECT_DIAGNOSE_ID_BY_NAME);
	m_insertDiagnosisOfPatientStatement = m_sqliteDatabase.CreateStatement(DatabaseSchemaSql::SQL_INSERT_DIAGNOSE_OF_PATIENT);
	m_deleteDiagnosesOfPatientStatement = m_sqliteDatabase.CreateStatement(DatabaseSchemaSql::SQL_DELETE_DIAGNOSES_OF_PATIENT);

	m_insertCityStatement = m_sqliteDatabase.CreateStatement(DatabaseSchemaSql::SQL_INSERT_CITY);
	m_selectCityByNameStatement = m_sqliteDatabase.CreateStatement(DatabaseSchemaSql::SQL_SELECT_CITY_ID_BY_NAME);
	m_selectCityByIdStatement = m_sqliteDatabase.CreateStatement(DatabaseSchemaSql::SQL_SELECT_CITY_NAME_BY_ID);

	m_searchPatientsByNameStatement = m_sqliteDatabase.CreateStatement(DatabaseSchemaSql::SQL_SELECT_PATIENTS_BY_NAME);
	m_searchPatientsByPhoneStatement = m_sqliteDatabase.CreateStatement(DatabaseSchemaSql::SQL_SELECT_PATIENTS_BY_PHONE);
	m_searchPatientsByCityStatement = m_sqliteDatabase.CreateStatement(DatabaseSchemaSql::SQL_SELECT_PATIENTS_BY_CITY_NAME);

	m_searchPatientsByDiagnoseStatement =
		m_sqliteDatabase.CreateStatement(DatabaseSchemaSql::SQL_SELECT_PATIENTS_BY_DIAGNOSE_NAME);

	m_searchPatientsByCityNonresidentStatement =
		m_sqliteDatabase.CreateStatement(DatabaseSchemaSql::SQL_SELECT_PATIENTS_BY_NOT_CITY_NAME);

	m_searchPatientsByAgeAndDiagnoseStatement =
		m_sqliteDatabase.CreateStatement(DatabaseSchemaSql::SQL_SELECT_PATIENTS_BY_AGE_AND_DIAGNOSE_NAME);
}


const std::string& PatientsRegistryImpl::GetConnectionString() const
{
	return m_sqliteDatabase.GetConnectionString();
}


void PatientsRegistryImpl::AddPatient(const Patient& patient)
{
	std::lock_guard<std::mutex> lock(m_mutex);

	m_sqliteDatabase.BeginTransaction();

	try
	{
		sqlite3_reset(m_insertPatientStatement.get());

		sqlite3_bind_text(m_insertPatientStatement.get(), 1, patient.GetName().c_str(), (int)patient.GetName().size(), SQLITE_STATIC);
		sqlite3_bind_int(m_insertPatientStatement.get(), 2, patient.GetBirthDate().GetBirthDateAsInt());
		sqlite3_bind_int64(m_insertPatientStatement.get(), 3, GetGenderId(patient.GetGender()));
		sqlite3_bind_int64(m_insertPatientStatement.get(), 4, GetCityId(patient.GetCity()));
		sqlite3_bind_text(m_insertPatientStatement.get(), 5, patient.GetPhone().c_str(), (int)patient.GetPhone().size(), SQLITE_STATIC);

		int sqliteStepResult = sqlite3_step(m_insertPatientStatement.get());

		if (sqliteStepResult != SQLITE_DONE)
		{
			const char* pExecErrorMessage = sqlite3_errmsg(m_sqliteDatabase.GetSqliteConnectionPtr());

			throw DatabaseSQLiteException(pExecErrorMessage != nullptr ? pExecErrorMessage : "",
				sqliteStepResult);
		}

		uint64_t patientId = sqlite3_last_insert_rowid(m_sqliteDatabase.GetSqliteConnectionPtr());

		SetDiagnosesOfPatient(patientId, patient.GetDiagnoses());
	}
	catch (...)
	{
		m_sqliteDatabase.RollbackTransaction();

		throw;
	}

	m_sqliteDatabase.CommitTransaction();
}


bool PatientsRegistryImpl::UpdatePatient(const PatientId_t patientId, const Patient& patient)
{
	std::lock_guard<std::mutex> lock(m_mutex);

	m_sqliteDatabase.BeginTransaction();

	try
	{
		sqlite3_reset(m_updatePatientStatement.get());

		sqlite3_bind_text(m_updatePatientStatement.get(), 1, patient.GetName().c_str(), (int)patient.GetName().size(), SQLITE_STATIC);
		sqlite3_bind_int(m_updatePatientStatement.get(), 2, patient.GetBirthDate().GetBirthDateAsInt());
		sqlite3_bind_int64(m_updatePatientStatement.get(), 3, GetGenderId(patient.GetGender()));
		sqlite3_bind_int64(m_updatePatientStatement.get(), 4, GetCityId(patient.GetCity()));
		sqlite3_bind_text(m_updatePatientStatement.get(), 5, patient.GetPhone().c_str(), (int)patient.GetPhone().size(), SQLITE_STATIC);
		sqlite3_bind_int64(m_updatePatientStatement.get(), 6, patientId);

		int sqliteStepResult = sqlite3_step(m_updatePatientStatement.get());

		if (sqliteStepResult != SQLITE_DONE)
		{
			const char* pExecErrorMessage = sqlite3_errmsg(m_sqliteDatabase.GetSqliteConnectionPtr());

			throw DatabaseSQLiteException(pExecErrorMessage != nullptr ? pExecErrorMessage : "",
				sqliteStepResult);
		}

		if (sqlite3_changes(m_sqliteDatabase.GetSqliteConnectionPtr()) == 0)
		{
			m_sqliteDatabase.RollbackTransaction();

			return false;
		}

		DeleteDiagnosesOfPatient(patientId);
		SetDiagnosesOfPatient(patientId, patient.GetDiagnoses());
	}
	catch (...)
	{
		m_sqliteDatabase.RollbackTransaction();

		throw;
	}

	m_sqliteDatabase.CommitTransaction();

	return true;
}


bool PatientsRegistryImpl::DeletePatient(const PatientId_t patientId)
{
	std::lock_guard<std::mutex> lock(m_mutex);

	sqlite3_reset(m_deletePatientStatement.get());
	sqlite3_bind_int64(m_deletePatientStatement.get(), 1, patientId);

	int sqliteStepResult = sqlite3_step(m_deletePatientStatement.get());

	if (sqliteStepResult == SQLITE_DONE)
	{
		if (sqlite3_changes(m_sqliteDatabase.GetSqliteConnectionPtr()) == 0)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	else
	{
		const char* pExecErrorMessage = sqlite3_errmsg(m_sqliteDatabase.GetSqliteConnectionPtr());

		throw DatabaseSQLiteException(pExecErrorMessage != nullptr ? pExecErrorMessage : "",
			sqliteStepResult);
	}
}


const std::vector<PatientRecord_t> PatientsRegistryImpl::GetAllPatients() const
{
	std::lock_guard<std::mutex> lock(m_mutex);

	sqlite3_reset(m_selectAllPatientsStatement.get());

	return GetPatientsFromStatement(m_selectAllPatientsStatement.get());
}


const std::vector<PatientRecord_t> PatientsRegistryImpl::GetPatientsFromStatement(sqlite3_stmt* statementPtr) const
{
	std::vector<PatientRecord_t> patientsRecords;

	int sqliteStepResult = sqlite3_step(statementPtr);

	while (sqliteStepResult == SQLITE_ROW)
	{
		const uint64_t patientId = sqlite3_column_int64(statementPtr, 0);
		const std::string name = (const char*)sqlite3_column_text(statementPtr, 1);
		const uint32_t birthDay = (uint32_t)sqlite3_column_int(statementPtr, 2);
		const uint32_t genderId = (uint32_t)sqlite3_column_int(statementPtr, 3);
		const uint32_t cityId = (uint32_t)sqlite3_column_int(statementPtr, 4);
		const std::string phone = (const char*)sqlite3_column_text(statementPtr, 5);

		patientsRecords.emplace_back(std::make_pair(patientId, Patient{
			name,
			GetGenderById(genderId),
			BirthDate{ birthDay },
			GetCityById(cityId),
			phone,
			GetDiagnosesOfPatient(patientId)
		}));

		sqliteStepResult = sqlite3_step(statementPtr);
	}

	if (sqliteStepResult != SQLITE_DONE)
	{
		const char* pExecErrorMessage = sqlite3_errmsg(m_sqliteDatabase.GetSqliteConnectionPtr());

		throw DatabaseSQLiteException(pExecErrorMessage != nullptr ? pExecErrorMessage : "",
			sqliteStepResult);
	}

	return patientsRecords;
}


PatientRecord_t PatientsRegistryImpl::GetPatientById(const PatientId_t patientId)
{
	if (patientId == PATIENTID_EMPTY)
	{
		return std::make_pair(patientId, Patient::GetEmptyPatient());
	}

	std::lock_guard<std::mutex> lock(m_mutex);

	sqlite3_reset(m_selectPatientStatement.get());
	sqlite3_bind_int64(m_selectPatientStatement.get(), 1, patientId);

	const std::vector<PatientRecord_t> patientsRecords = GetPatientsFromStatement(m_selectPatientStatement.get());

	if (patientsRecords.empty())
	{
		return std::make_pair(PATIENTID_EMPTY, Patient::GetEmptyPatient());
	}
	else
	{
		return patientsRecords.front();
	}
}


const std::vector<PatientRecord_t> PatientsRegistryImpl::SearchPatientsByName(const std::string& name) const
{
	std::lock_guard<std::mutex> lock(m_mutex);

	sqlite3_reset(m_searchPatientsByNameStatement.get());
	sqlite3_bind_text(m_searchPatientsByNameStatement.get(), 1, name.c_str(), (int)name.size(), SQLITE_STATIC);

	return GetPatientsFromStatement(m_searchPatientsByNameStatement.get());
}


const std::vector<PatientRecord_t> PatientsRegistryImpl::SearchPatientsByCity(const std::string& city) const
{
	std::lock_guard<std::mutex> lock(m_mutex);

	sqlite3_reset(m_searchPatientsByCityStatement.get());
	sqlite3_bind_text(m_searchPatientsByCityStatement.get(), 1, city.c_str(), (int)city.size(), SQLITE_STATIC);

	return GetPatientsFromStatement(m_searchPatientsByCityStatement.get());
}


const std::vector<PatientRecord_t> PatientsRegistryImpl::SearchPatientsByDiagnose(const std::string& diagnose) const
{
	std::lock_guard<std::mutex> lock(m_mutex);

	sqlite3_reset(m_searchPatientsByDiagnoseStatement.get());
	sqlite3_bind_text(m_searchPatientsByDiagnoseStatement.get(), 1, diagnose.c_str(), (int)diagnose.size(), SQLITE_STATIC);

	return GetPatientsFromStatement(m_searchPatientsByDiagnoseStatement.get());
}


const std::vector<PatientRecord_t> PatientsRegistryImpl::SearchPatientsByPhone(const std::string& phone) const
{
	std::lock_guard<std::mutex> lock(m_mutex);

	sqlite3_reset(m_searchPatientsByPhoneStatement.get());
	sqlite3_bind_text(m_searchPatientsByPhoneStatement.get(), 1, phone.c_str(), (int)phone.size(), SQLITE_STATIC);

	return GetPatientsFromStatement(m_searchPatientsByPhoneStatement.get());
}


const std::vector<PatientRecord_t> PatientsRegistryImpl::OrderPatientsByName() const
{
	std::lock_guard<std::mutex> lock(m_mutex);

	sqlite3_reset(m_selectAllPatientsOrderByNameStatement.get());

	return GetPatientsFromStatement(m_selectAllPatientsOrderByNameStatement.get());
}


const std::vector<PatientRecord_t> PatientsRegistryImpl::OrderPatientsByCity() const
{
	std::lock_guard<std::mutex> lock(m_mutex);

	sqlite3_reset(m_selectAllPatientsOrderByCityStatement.get());

	return GetPatientsFromStatement(m_selectAllPatientsOrderByCityStatement.get());
}


const std::vector<PatientRecord_t> PatientsRegistryImpl::OrderPatientsByBirthDate() const
{
	std::lock_guard<std::mutex> lock(m_mutex);

	sqlite3_reset(m_selectAllPatientsOrderByBirthDateStatement.get());

	return GetPatientsFromStatement(m_selectAllPatientsOrderByBirthDateStatement.get());
}


const std::vector<PatientRecord_t> PatientsRegistryImpl::ViewNonresidentPatients(const std::string& city) const
{
	std::lock_guard<std::mutex> lock(m_mutex);

	sqlite3_reset(m_searchPatientsByCityNonresidentStatement.get());
	sqlite3_bind_text(m_searchPatientsByCityNonresidentStatement.get(), 1, city.c_str(), (int)city.size(), SQLITE_STATIC);

	return GetPatientsFromStatement(m_searchPatientsByCityNonresidentStatement.get());
}


const std::vector<PatientRecord_t> PatientsRegistryImpl::ViewPatientsByAgeAndDiagnose(uint16_t age,
	const std::string& diagnose) const
{
	BirthDate birthDateToday(BirthDate::GetCurrentDate());
	BirthDate birthDateMore(birthDateToday.GetYear() - age, birthDateToday.GetMonth(), birthDateToday.GetDay());
	int birthDateMoreInt = static_cast<int>(birthDateMore.GetBirthDateAsInt());

	std::lock_guard<std::mutex> lock(m_mutex);

	sqlite3_reset(m_searchPatientsByAgeAndDiagnoseStatement.get());
	sqlite3_bind_int(m_searchPatientsByAgeAndDiagnoseStatement.get(), 1, birthDateMoreInt);
	sqlite3_bind_text(m_searchPatientsByAgeAndDiagnoseStatement.get(), 2, diagnose.c_str(), (int)diagnose.size(), SQLITE_STATIC);

	return GetPatientsFromStatement(m_searchPatientsByAgeAndDiagnoseStatement.get());
}


const std::vector<std::string> PatientsRegistryImpl::GetDiagnosesOfPatient(const uint64_t patientId) const
{
	std::vector<std::string> patientDiagnoses;

	sqlite3_reset(m_selectDiagnosesOfPatientStatement.get());
	sqlite3_bind_int64(m_selectDiagnosesOfPatientStatement.get(), 1, patientId);

	int sqliteStepResult = sqlite3_step(m_selectDiagnosesOfPatientStatement.get());

	while (sqliteStepResult == SQLITE_ROW)
	{
		const std::string name = (const char*)sqlite3_column_text(m_selectDiagnosesOfPatientStatement.get(), 0);
		patientDiagnoses.emplace_back(name);

		sqliteStepResult = sqlite3_step(m_selectDiagnosesOfPatientStatement.get());
	}

	if (sqliteStepResult != SQLITE_DONE)
	{
		const char* pExecErrorMessage = sqlite3_errmsg(m_sqliteDatabase.GetSqliteConnectionPtr());

		throw DatabaseSQLiteException(pExecErrorMessage != nullptr ? pExecErrorMessage : "",
			sqliteStepResult);
	}

	return patientDiagnoses;
}


void PatientsRegistryImpl::SetDiagnosesOfPatient(const uint64_t patientId, const std::vector<std::string>& diagnoses)
{
	std::unordered_set<uint64_t> diagnosesIds;

	for (const auto& diagnosisName : diagnoses)
	{
		sqlite3_reset(m_selectDiagnosisStatement.get());
		sqlite3_bind_text(m_selectDiagnosisStatement.get(), 1, diagnosisName.c_str(), (int)diagnosisName.size(), SQLITE_STATIC);

		int sqliteStepResult = sqlite3_step(m_selectDiagnosisStatement.get());

		if (sqliteStepResult == SQLITE_ROW)
		{
			const uint64_t diagnosisId = sqlite3_column_int64(m_selectDiagnosisStatement.get(), 0);
			diagnosesIds.insert(diagnosisId);
		}
		else if (sqliteStepResult == SQLITE_DONE)
		{
			sqlite3_reset(m_insertDiagnosisStatement.get());
			sqlite3_bind_text(m_insertDiagnosisStatement.get(), 1, diagnosisName.c_str(), (int)diagnosisName.size(), SQLITE_STATIC);

			int sqliteInsertStepResult = sqlite3_step(m_insertDiagnosisStatement.get());

			if (sqliteInsertStepResult != SQLITE_DONE)
			{
				const char* pExecErrorMessage = sqlite3_errmsg(m_sqliteDatabase.GetSqliteConnectionPtr());

				throw DatabaseSQLiteException(pExecErrorMessage != nullptr ? pExecErrorMessage : "",
					sqliteStepResult);
			}

			const uint64_t diagnosisId = sqlite3_last_insert_rowid(m_sqliteDatabase.GetSqliteConnectionPtr());
			diagnosesIds.insert(diagnosisId);
		}
		else
		{
			const char* pExecErrorMessage = sqlite3_errmsg(m_sqliteDatabase.GetSqliteConnectionPtr());

			throw DatabaseSQLiteException(pExecErrorMessage != nullptr ? pExecErrorMessage : "",
				sqliteStepResult);
		}
	}

	for (const auto diagnoseId : diagnosesIds)
	{
		sqlite3_reset(m_insertDiagnosisOfPatientStatement.get());
		sqlite3_bind_int64(m_insertDiagnosisOfPatientStatement.get(), 1, patientId);
		sqlite3_bind_int64(m_insertDiagnosisOfPatientStatement.get(), 2, diagnoseId);

		int sqliteStepResult = sqlite3_step(m_insertDiagnosisOfPatientStatement.get());

		if (sqliteStepResult != SQLITE_DONE)
		{
			const char* pExecErrorMessage = sqlite3_errmsg(m_sqliteDatabase.GetSqliteConnectionPtr());

			throw DatabaseSQLiteException(pExecErrorMessage != nullptr ? pExecErrorMessage : "",
				sqliteStepResult);
		}
	}
}


void PatientsRegistryImpl::DeleteDiagnosesOfPatient(const uint64_t patientId)
{
	sqlite3_reset(m_deleteDiagnosesOfPatientStatement.get());
	sqlite3_bind_int64(m_deleteDiagnosesOfPatientStatement.get(), 1, patientId);

	int sqliteStepResult = sqlite3_step(m_deleteDiagnosesOfPatientStatement.get());

	if (sqliteStepResult == SQLITE_DONE)
	{
		return;
	}
	else
	{
		const char* pExecErrorMessage = sqlite3_errmsg(m_sqliteDatabase.GetSqliteConnectionPtr());

		throw DatabaseSQLiteException(pExecErrorMessage != nullptr ? pExecErrorMessage : "",
			sqliteStepResult);
	}
}


const std::string PatientsRegistryImpl::GetCityById(const uint64_t cityId) const
{
	sqlite3_reset(m_selectCityByIdStatement.get());
	sqlite3_bind_int64(m_selectCityByIdStatement.get(), 1, cityId);

	int sqliteStepResult = sqlite3_step(m_selectCityByIdStatement.get());

	if (sqliteStepResult != SQLITE_ROW)
	{
		const char* pExecErrorMessage = sqlite3_errmsg(m_sqliteDatabase.GetSqliteConnectionPtr());

		throw DatabaseSQLiteException(pExecErrorMessage != nullptr ? pExecErrorMessage : "",
			sqliteStepResult);
	}

	const std::string name = (const char*)sqlite3_column_text(m_selectCityByIdStatement.get(), 0);

	return name;
}


uint64_t PatientsRegistryImpl::GetCityId(const std::string& city)
{
	uint64_t cityId;

	sqlite3_reset(m_selectCityByNameStatement.get());
	sqlite3_bind_text(m_selectCityByNameStatement.get(), 1, city.c_str(), (int)city.size(), SQLITE_STATIC);

	int sqliteStepResult = sqlite3_step(m_selectCityByNameStatement.get());

	if (sqliteStepResult == SQLITE_ROW)
	{
		cityId = sqlite3_column_int64(m_selectCityByNameStatement.get(), 0);
	}
	else if (sqliteStepResult == SQLITE_DONE)
	{
		sqlite3_reset(m_insertCityStatement.get());
		sqlite3_bind_text(m_insertCityStatement.get(), 1, city.c_str(), (int)city.size(), SQLITE_STATIC);

		int sqliteInsertStepResult = sqlite3_step(m_insertCityStatement.get());

		if (sqliteInsertStepResult != SQLITE_DONE)
		{
			const char* pExecErrorMessage = sqlite3_errmsg(m_sqliteDatabase.GetSqliteConnectionPtr());

			throw DatabaseSQLiteException(pExecErrorMessage != nullptr ? pExecErrorMessage : "",
				sqliteStepResult);
		}

		cityId = sqlite3_last_insert_rowid(m_sqliteDatabase.GetSqliteConnectionPtr());
	}
	else
	{
		const char* pExecErrorMessage = sqlite3_errmsg(m_sqliteDatabase.GetSqliteConnectionPtr());

		throw DatabaseSQLiteException(pExecErrorMessage != nullptr ? pExecErrorMessage : "",
			sqliteStepResult);
	}

	return cityId;
}


Gender PatientsRegistryImpl::GetGenderById(const uint32_t genderId) const
{
	return DatabaseSchemaSql::DATABASE_SQLITE_GENDERS_BIND_TABLE.at(genderId);
}


uint32_t PatientsRegistryImpl::GetGenderId(const Gender gender)
{
	const auto& bindMap = DatabaseSchemaSql::DATABASE_SQLITE_GENDERS_BIND_TABLE;

	auto it = std::find_if(bindMap.cbegin(), bindMap.cend(),
		[&gender](const std::pair<uint32_t, Gender>& t) -> bool {
		return (t.second == gender);
	});

	if (it == bindMap.cend())
	{
		throw std::invalid_argument("Invalid gender value");
	}

	return it->first;
}


void PatientsRegistryImpl::ValidateDatabaseSQLiteSchema()
{
	m_sqliteDatabase.BeginTransaction();

	try
	{
		m_sqliteDatabase.ExecuteSql(DatabaseSchemaSql::PATIENTS_DATABASE_SQLITE_SCHEMA);
	}
	catch (...)
	{
		m_sqliteDatabase.RollbackTransaction();

		throw;
	}

	m_sqliteDatabase.CommitTransaction();
}
