#include "stdafx.h"
#include "../PatientsRegistry/PatientsRegistry.h"


class PatientsManagmentTest : public ::testing::Test
{
protected:
	void SetUp()
	{
		m_patientsRegistry = PatientsRegistry::CreatePatientsRegistry(R"(:memory:)");
	}

	void TearDown()
	{
	}

	std::shared_ptr<PatientsRegistry> m_patientsRegistry;
};


TEST_F(PatientsManagmentTest, AddNewPatientWithoutDiagnoses)
{
	Patient patient("AddNewPatientWithoutDiagnoses", Gender::Male, BirthDate(27061984), "Luninets", "+375292222222", {});

	ASSERT_NO_THROW(m_patientsRegistry->AddPatient(patient));
}


TEST_F(PatientsManagmentTest, AddNewPatientWithOneDiagnoses)
{
	Patient patient("AddNewPatientWithOneDiagnoses", Gender::Male, BirthDate(27061984), "Luninets", "+375292222222", {
		"diagnosis_1"});

	ASSERT_NO_THROW(m_patientsRegistry->AddPatient(patient));
}


TEST_F(PatientsManagmentTest, AddNewPatientWithThreeDiagnoses)
{
	Patient patient("AddNewPatientWithOneDiagnoses", Gender::Male, BirthDate(27061984), "Luninets", "+375292222222", {
		"diagnosis_1", "diagnosis_2", "diagnosis_3" });

	ASSERT_NO_THROW(m_patientsRegistry->AddPatient(patient));
}


TEST_F(PatientsManagmentTest, DeletePatient)
{
	const std::vector<PatientRecord_t> allPatients1 = m_patientsRegistry->GetAllPatients();

	Patient patient("DeletePatient", Gender::Male, BirthDate(27061984), "Luninets", "+375292222222", {
		"diagnosis_1" });

	ASSERT_NO_THROW(m_patientsRegistry->AddPatient(patient));

	const std::vector<PatientRecord_t> allPatients2 = m_patientsRegistry->GetAllPatients();

	ASSERT_EQ(allPatients1.size() + 1, allPatients2.size());

	const PatientRecord_t& lastPatientRecord = allPatients2.back();
	ASSERT_TRUE(m_patientsRegistry->DeletePatient(lastPatientRecord.first));

	const std::vector<PatientRecord_t> allPatients3 = m_patientsRegistry->GetAllPatients();

	ASSERT_EQ(allPatients1.size(), allPatients3.size());
}


TEST_F(PatientsManagmentTest, UpdatePatient)
{
	Patient patient("UpdatePatient", Gender::Male, BirthDate(27061984), "Minsk", "+375292222223", {
		"diagnosis_1" });

	ASSERT_NO_THROW(m_patientsRegistry->AddPatient(patient));

	const std::vector<PatientRecord_t> allPatients = m_patientsRegistry->GetAllPatients();
	const PatientRecord_t& lastPatientRecord = allPatients.back();

	Patient newPatient("UpdatePatient2", Gender::Schemale, BirthDate(27061985), "Gomel", "+275292222223", {
		"diagnosis_0" });

	ASSERT_TRUE(m_patientsRegistry->UpdatePatient(lastPatientRecord.first, newPatient));

	const PatientRecord_t& updatedPatientRecord = m_patientsRegistry->GetPatientById(lastPatientRecord.first);
	const Patient& updatedPatient = updatedPatientRecord.second;
	ASSERT_NE(updatedPatient, Patient::GetEmptyPatient());

	ASSERT_NE(updatedPatient, patient);
	ASSERT_EQ(updatedPatient, newPatient);
}


TEST_F(PatientsManagmentTest, SearchPatientsByNameWIthDiffCase)
{
	Patient patient1("NewPatient1", Gender::Male, BirthDate(27061984), "Luninets", "+375291222222", {
		"diagnosis_1" });

	ASSERT_NO_THROW(m_patientsRegistry->AddPatient(patient1));

	Patient patient2("newPatient1", Gender::Male, BirthDate(27061984), "Luninets", "+375291222222", {
		"diagnosis_1" });

	ASSERT_NO_THROW(m_patientsRegistry->AddPatient(patient2));
}


TEST_F(PatientsManagmentTest, SearchPatientsByNameWIthDiffCaseRussian)
{
	Patient patient1(u8"НовыйПациент1", Gender::Male, BirthDate(27061984), "Luninets", "+375291222222", {
		"diagnosis_1" });

	ASSERT_NO_THROW(m_patientsRegistry->AddPatient(patient1));

	Patient patient2(u8"НОВЫЙПАЦИЕНТ1", Gender::Male, BirthDate(27061984), "Luninets", "+375291222222", {
		"diagnosis_1" });

	ASSERT_NO_THROW(m_patientsRegistry->AddPatient(patient2));

	const std::vector<PatientRecord_t> foundPatientsByName =
		m_patientsRegistry->SearchPatientsByName(u8"новыйпациент1");

	ASSERT_EQ(foundPatientsByName.size(), 2);
}


TEST_F(PatientsManagmentTest, SearchPatientsByNameWIthDiffCaseGerman)
{
	Patient patient1(u8"Blue Öyster Cult", Gender::Male, BirthDate(27061984), "Luninets", "+375291222222", {
		"diagnosis_1" });

	ASSERT_NO_THROW(m_patientsRegistry->AddPatient(patient1));

	Patient patient2(u8"BLUE ÖYSTER CULT", Gender::Male, BirthDate(27061984), "Luninets", "+375291222222", {
		"diagnosis_1" });

	ASSERT_NO_THROW(m_patientsRegistry->AddPatient(patient2));

	const std::vector<PatientRecord_t> foundPatientsByName =
		m_patientsRegistry->SearchPatientsByName(u8"blue öyster cult");

	ASSERT_EQ(foundPatientsByName.size(), 2);

	const std::vector<PatientRecord_t> notfoundPatientsByName =
		m_patientsRegistry->SearchPatientsByName(u8"Blue Öuster Cult");

	ASSERT_EQ(notfoundPatientsByName.size(), 0);
}
