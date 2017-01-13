#include "stdafx.h"
#include "PatientsRegistry.h"
#include "PatientsRegistryImpl.h"


PatientsRegistry::PatientsRegistry(const std::string& connectionString, bool createIfNotExist, bool readOnly) :
	m_patientsRegistryImpl(std::make_unique<PatientsRegistryImpl>(connectionString, createIfNotExist, readOnly))
{
}


std::shared_ptr<PatientsRegistry> PatientsRegistry::CreatePatientsRegistry(const std::string& connectionString)
{
	return std::shared_ptr<PatientsRegistry>(new PatientsRegistry(connectionString, true, false));
}


std::shared_ptr<PatientsRegistry> PatientsRegistry::OpenPatientsRegistry(const std::string& connectionString, bool readOnly)
{
	return std::shared_ptr<PatientsRegistry>(new PatientsRegistry(connectionString, false, readOnly));
}


const std::string& PatientsRegistry::GetConnectionString() const
{
	return m_patientsRegistryImpl->GetConnectionString();
}


const std::vector<PatientRecord_t> PatientsRegistry::GetAllPatients() const
{
	return m_patientsRegistryImpl->GetAllPatients();
}


void PatientsRegistry::AddPatient(const Patient& patient)
{
	m_patientsRegistryImpl->AddPatient(patient);
}


bool PatientsRegistry::UpdatePatient(const PatientId_t patientId, const Patient& patient)
{
	return m_patientsRegistryImpl->UpdatePatient(patientId, patient);
}


bool PatientsRegistry::DeletePatient(const PatientId_t patientId)
{
	return m_patientsRegistryImpl->DeletePatient(patientId);
}


PatientRecord_t PatientsRegistry::GetPatientById(const PatientId_t patientId)
{
	return m_patientsRegistryImpl->GetPatientById(patientId);
}


const std::vector<PatientRecord_t> PatientsRegistry::SearchPatientsByName(const std::string& name) const
{
	return m_patientsRegistryImpl->SearchPatientsByName(name);
}


const std::vector<PatientRecord_t> PatientsRegistry::SearchPatientsByCity(const std::string& city) const
{
	return m_patientsRegistryImpl->SearchPatientsByCity(city);
}


const std::vector<PatientRecord_t> PatientsRegistry::SearchPatientsByDiagnose(const std::string& diagnose) const
{
	return m_patientsRegistryImpl->SearchPatientsByDiagnose(diagnose);
}


const std::vector<PatientRecord_t> PatientsRegistry::SearchPatientsByPhone(const std::string& phone) const
{
	return m_patientsRegistryImpl->SearchPatientsByPhone(phone);
}


const std::vector<PatientRecord_t> PatientsRegistry::OrderPatientsByName() const
{
	return m_patientsRegistryImpl->OrderPatientsByName();
}


const std::vector<PatientRecord_t> PatientsRegistry::OrderPatientsByCity() const
{
	return m_patientsRegistryImpl->OrderPatientsByCity();
}


const std::vector<PatientRecord_t> PatientsRegistry::OrderPatientsByBirthDate() const
{
	return m_patientsRegistryImpl->OrderPatientsByBirthDate();
}


const std::vector<PatientRecord_t> PatientsRegistry::ViewNonresidentPatients(const std::string& city) const
{
	return m_patientsRegistryImpl->ViewNonresidentPatients(city);
}


const std::vector<PatientRecord_t> PatientsRegistry::ViewPatientsByAgeAndDiagnose(uint16_t age,
	const std::string& diagnose) const
{
	return m_patientsRegistryImpl->ViewPatientsByAgeAndDiagnose(age, diagnose);
}
