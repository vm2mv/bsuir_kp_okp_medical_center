#pragma once

#include "Gender.h"

struct DatabaseSchemaSql
{
	static const std::map<uint32_t, Gender> DATABASE_SQLITE_GENDERS_BIND_TABLE;

	static const std::string PATIENTS_DATABASE_SQLITE_SCHEMA;

	static const std::string DatabaseSchemaSql::SQL_INSERT_PATIENT;
	static const std::string DatabaseSchemaSql::SQL_UPDATE_PATIENT;
	static const std::string DatabaseSchemaSql::SQL_DELETE_PATIENT;
	static const std::string DatabaseSchemaSql::SQL_SELECT_PATIENT_BY_ID;
	static const std::string DatabaseSchemaSql::SQL_SELECT_ALLS_PATIENTS;
	static const std::string DatabaseSchemaSql::SQL_SELECT_ALLS_PATIENTS_ORDER_BY_NAME;
	static const std::string DatabaseSchemaSql::SQL_SELECT_ALLS_PATIENTS_ORDER_BY_BIRTHDATE;
	static const std::string DatabaseSchemaSql::SQL_SELECT_ALLS_PATIENTS_ORDER_BY_CITY;
	static const std::string DatabaseSchemaSql::SQL_SELECT_DIAGNOSES_BY_PATIENT_ID;
	static const std::string DatabaseSchemaSql::SQL_INSERT_DIAGNOSE;
	static const std::string DatabaseSchemaSql::SQL_SELECT_DIAGNOSE_ID_BY_NAME;
	static const std::string DatabaseSchemaSql::SQL_INSERT_DIAGNOSE_OF_PATIENT;
	static const std::string DatabaseSchemaSql::SQL_DELETE_DIAGNOSES_OF_PATIENT;
	static const std::string DatabaseSchemaSql::SQL_INSERT_CITY;
	static const std::string DatabaseSchemaSql::SQL_SELECT_CITY_ID_BY_NAME;
	static const std::string DatabaseSchemaSql::SQL_SELECT_CITY_NAME_BY_ID;
	static const std::string DatabaseSchemaSql::SQL_SELECT_PATIENTS_BY_NAME;
	static const std::string DatabaseSchemaSql::SQL_SELECT_PATIENTS_BY_PHONE;
	static const std::string DatabaseSchemaSql::SQL_SELECT_PATIENTS_BY_CITY_NAME;
	static const std::string DatabaseSchemaSql::SQL_SELECT_PATIENTS_BY_DIAGNOSE_NAME;
	static const std::string DatabaseSchemaSql::SQL_SELECT_PATIENTS_BY_NOT_CITY_NAME;
	static const std::string DatabaseSchemaSql::SQL_SELECT_PATIENTS_BY_AGE_AND_DIAGNOSE_NAME;
};
