#pragma once

#include "Patient.h"

class PatientsRegistryImpl;


class PatientsRegistry
{
public:
	static std::shared_ptr<PatientsRegistry> CreatePatientsRegistry(const std::string& connectionString);
	static std::shared_ptr<PatientsRegistry> OpenPatientsRegistry(const std::string& connectionString, bool readOnly);

	PatientsRegistry() = delete;
	PatientsRegistry(const PatientsRegistry&) = delete;
	PatientsRegistry& operator = (const PatientsRegistry&) = delete;

private:
	explicit PatientsRegistry(const std::string& connectionString, bool createIfNotExist, bool readOnly);

public:
	const std::string& GetConnectionString() const;

	const std::vector<PatientRecord_t> GetAllPatients() const;
	void AddPatient(const Patient& patient);
	bool UpdatePatient(const PatientId_t patientId, const Patient& patient);
	bool DeletePatient(const PatientId_t patientId);
	PatientRecord_t GetPatientById(const PatientId_t patientId);

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
	std::unique_ptr<PatientsRegistryImpl> m_patientsRegistryImpl;
};

