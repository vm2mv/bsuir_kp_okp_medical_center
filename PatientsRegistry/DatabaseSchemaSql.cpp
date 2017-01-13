#include "stdafx.h"
#include "DatabaseSchemaSql.h"


const std::map<uint32_t, Gender> DatabaseSchemaSql::DATABASE_SQLITE_GENDERS_BIND_TABLE = {
	{ 1, Gender::Male },
	{ 2, Gender::Feemale },
	{ 3, Gender::Schemale },
};


const std::string DatabaseSchemaSql::PATIENTS_DATABASE_SQLITE_SCHEMA = R"(

CREATE TABLE IF NOT EXISTS genders (
	gender_id INTEGER PRIMARY KEY NOT NULL,
	gender_name TEXT COLLATE NOCASE NOT NULL UNIQUE
);

CREATE UNIQUE INDEX IF NOT EXISTS genders_name_idx ON genders (gender_name);

INSERT OR REPLACE INTO genders(gender_id, gender_name) VALUES(1, 'male');
INSERT OR REPLACE INTO genders(gender_id, gender_name) VALUES(2, 'female');
INSERT OR REPLACE INTO genders(gender_id, gender_name) VALUES(3, 'shemale');

CREATE TABLE IF NOT EXISTS cities (
	city_id INTEGER PRIMARY KEY NOT NULL,
	city_name TEXT COLLATE NOCASE NOT NULL UNIQUE
);

CREATE UNIQUE INDEX IF NOT EXISTS cities_name_idx ON cities (city_name);

CREATE TABLE IF NOT EXISTS patients (
	patient_id INTEGER PRIMARY KEY AUTOINCREMENT,
	patient_name TEXT COLLATE NOCASE NOT NULL,
	patient_birth_day INTEGER NOT NULL,
	gender_id INTEGER NOT NULL,
	city_id INTEGER NOT NULL,
	patient_phone TEXT COLLATE NOCASE DEFAULT NULL,

	FOREIGN KEY(gender_id) REFERENCES genders(gender_id) ON UPDATE CASCADE,
	FOREIGN KEY(city_id) REFERENCES cities(city_id) ON UPDATE CASCADE
);

CREATE INDEX IF NOT EXISTS patients_name_idx ON patients (patient_name);
CREATE INDEX IF NOT EXISTS patients_birth_day_idx ON patients (patient_birth_day);
CREATE INDEX IF NOT EXISTS patients_phone_idx ON patients (patient_phone);

CREATE TABLE IF NOT EXISTS diagnoses (
	diagnosis_id INTEGER PRIMARY KEY AUTOINCREMENT,
	diagnosis_name TEXT COLLATE NOCASE NOT NULL UNIQUE
);

CREATE UNIQUE INDEX IF NOT EXISTS diagnoses_name_idx ON diagnoses (diagnosis_name);

CREATE TABLE IF NOT EXISTS patients_diagnoses (
	patient_diagnosis_id INTEGER PRIMARY KEY AUTOINCREMENT,
	patient_id INTEGER NOT NULL,
	diagnosis_id INTEGER NOT NULL,

	FOREIGN KEY(patient_id) REFERENCES patients(patient_id) ON UPDATE CASCADE ON DELETE CASCADE,
	FOREIGN KEY(diagnosis_id) REFERENCES diagnoses(diagnosis_id) ON UPDATE CASCADE ON DELETE CASCADE
);

)";

const std::string DatabaseSchemaSql::SQL_INSERT_PATIENT =
	"insert into patients (patient_name, patient_birth_day, gender_id, city_id, patient_phone) "
	"values (?, ?, ?, ?, ?);";

const std::string DatabaseSchemaSql::SQL_UPDATE_PATIENT =
	"update patients set patient_name = ?, patient_birth_day = ?, gender_id = ?, city_id = ?, patient_phone = ? "
	"where patient_id = ?;";

const std::string DatabaseSchemaSql::SQL_DELETE_PATIENT =
	"delete from patients where patient_id = ?;";

const std::string DatabaseSchemaSql::SQL_SELECT_PATIENT_BY_ID =
	"select patient_id, patient_name, patient_birth_day, gender_id, city_id, patient_phone from patients where "
	"patient_id = ?;";

const std::string DatabaseSchemaSql::SQL_SELECT_ALLS_PATIENTS =
	"select patient_id, patient_name, patient_birth_day, gender_id, city_id, patient_phone from patients;";

const std::string DatabaseSchemaSql::SQL_SELECT_ALLS_PATIENTS_ORDER_BY_NAME =
	"select patient_id, patient_name, patient_birth_day, gender_id, city_id, patient_phone from patients "
	"order by patient_name asc;";

const std::string DatabaseSchemaSql::SQL_SELECT_ALLS_PATIENTS_ORDER_BY_BIRTHDATE =
	"select patient_id, patient_name, patient_birth_day, gender_id, city_id, patient_phone from patients "
	"order by patient_birth_day asc;";

const std::string DatabaseSchemaSql::SQL_SELECT_ALLS_PATIENTS_ORDER_BY_CITY =
	"select patient_id, patient_name, patient_birth_day, gender_id, p.city_id, patient_phone from patients as p "
	"join cities as c on c.city_id = p.city_id order by c.city_name asc;";

const std::string DatabaseSchemaSql::SQL_SELECT_DIAGNOSES_BY_PATIENT_ID =
	"select diagnosis_name from diagnoses where diagnosis_id in " \
	"(select diagnosis_id from patients_diagnoses where patient_id = ?);";

const std::string DatabaseSchemaSql::SQL_INSERT_DIAGNOSE =
	"insert into diagnoses (diagnosis_name) values (?);";

const std::string DatabaseSchemaSql::SQL_SELECT_DIAGNOSE_ID_BY_NAME =
	"select diagnosis_id from diagnoses where diagnosis_name = ?;";

const std::string DatabaseSchemaSql::SQL_INSERT_DIAGNOSE_OF_PATIENT =
	"insert into patients_diagnoses (patient_id, diagnosis_id) values (?, ?);";

const std::string DatabaseSchemaSql::SQL_DELETE_DIAGNOSES_OF_PATIENT =
	"delete from patients_diagnoses where patient_id = ?;";

const std::string DatabaseSchemaSql::SQL_INSERT_CITY =
	"insert into cities (city_name) values (?);";

const std::string DatabaseSchemaSql::SQL_SELECT_CITY_ID_BY_NAME =
	"select city_id from cities where city_name = ?;";

const std::string DatabaseSchemaSql::SQL_SELECT_CITY_NAME_BY_ID =
	"select city_name from cities where city_id = ?;";

const std::string DatabaseSchemaSql::SQL_SELECT_PATIENTS_BY_NAME =
	"select patient_id, patient_name, patient_birth_day, gender_id, city_id, patient_phone from patients "
	"where patient_name like '%'||?||'%';";

const std::string DatabaseSchemaSql::SQL_SELECT_PATIENTS_BY_PHONE =
	"select patient_id, patient_name, patient_birth_day, gender_id, city_id, patient_phone from patients "
	"where patient_phone like '%'||?||'%';";

const std::string DatabaseSchemaSql::SQL_SELECT_PATIENTS_BY_CITY_NAME =
	"select patient_id, patient_name, patient_birth_day, gender_id, city_id, patient_phone from patients "
	"where city_id in (select city_id from cities where city_name like '%'||?||'%');";

const std::string DatabaseSchemaSql::SQL_SELECT_PATIENTS_BY_DIAGNOSE_NAME =
	"select patient_id, patient_name, patient_birth_day, gender_id, city_id, patient_phone from patients "
	"where patient_id in (select patient_id from patients_diagnoses where diagnosis_id in "
	"(select diagnosis_id from diagnoses where diagnosis_name like '%'||?||'%'));";

const std::string DatabaseSchemaSql::SQL_SELECT_PATIENTS_BY_NOT_CITY_NAME =
	"select patient_id, patient_name, patient_birth_day, gender_id, city_id, patient_phone from patients "
	"where city_id not in (select city_id from cities where city_name like '%'||?||'%');";

const std::string DatabaseSchemaSql::SQL_SELECT_PATIENTS_BY_AGE_AND_DIAGNOSE_NAME =
	"select patient_id, patient_name, patient_birth_day, gender_id, city_id, patient_phone from patients "
	"where patient_birth_day <= ? and "
	"patient_id in (select patient_id from patients_diagnoses where diagnosis_id in "
	"(select diagnosis_id from diagnoses where diagnosis_name like '%'||?||'%'));";
