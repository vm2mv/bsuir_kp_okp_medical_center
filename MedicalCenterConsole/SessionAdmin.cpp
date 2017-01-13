#include "stdafx.h"
#include "SessionAdmin.h"
#include "../PatientsRegistry/PatientsRegistry.h"
#include "../UsersRegistry/UsersRegistry.h"
#include "Console.h"
#include "Language.h"


SessionAdmin::SessionAdmin(const std::string& userName, const std::string& usersDatabaseFilePath,
	const Language& language, Console& console, std::shared_ptr<PatientsRegistry> patientsRegistry) 
	:
	m_userName(userName),
	m_usersController(usersDatabaseFilePath, console, language),
	m_patientsController(console, language, patientsRegistry),
	m_language(language),
	m_console(console),
	m_menu(console, language)
{
	CreateMenu();
}


SessionAdmin::~SessionAdmin()
{
}


void SessionAdmin::StartSessionAndWaitExit()
{
	m_menu.StartMenuAndWaitExit();
}


void SessionAdmin::CreateMenu()
{
	ConsoleMenu menuUsers(m_console, m_language);
	menuUsers.AddItem(m_language.GetString(LanguageString::MenuViewAllUsers),
		std::bind(&UsersController::PrintAllUsers, &m_usersController));
	menuUsers.AddItem(m_language.GetString(LanguageString::MenuAddNewUser),
		std::bind(&UsersController::AddNewUser, &m_usersController));
	menuUsers.AddItem(m_language.GetString(LanguageString::MenuEditUser),
		std::bind(&UsersController::EditUser, &m_usersController));
	menuUsers.AddItem(m_language.GetString(LanguageString::MenuRemoveUser),
		std::bind(&UsersController::RemoveUser, &m_usersController));

	ConsoleMenu menuFile(m_console, m_language);
	menuFile.AddItem(m_language.GetString(LanguageString::MenuPatientsFileCreateFile),
		std::bind(&PatientsController::CreateFile, &m_patientsController));
	menuFile.AddItem(m_language.GetString(LanguageString::MenuPatientsFileOpen),
		std::bind(&PatientsController::OpenFileForReadAndWrite, &m_patientsController));
	menuFile.AddItem(m_language.GetString(LanguageString::MenuPatientsCloseFile),
		std::bind(&PatientsController::CloseFile, &m_patientsController));
	menuFile.AddItem(m_language.GetString(LanguageString::MenuPatientsRemoveFile),
		std::bind(&PatientsController::RemoveFile, &m_patientsController));

	ConsoleMenu menuDataEdit(m_console, m_language);
	menuDataEdit.AddItem(m_language.GetString(LanguageString::MenuViewAllPatients),
		std::bind(&PatientsController::ViewAllPatients, &m_patientsController));
	menuDataEdit.AddItem(m_language.GetString(LanguageString::MenuAddPatient),
		std::bind(&PatientsController::AddPatient, &m_patientsController));
	menuDataEdit.AddItem(m_language.GetString(LanguageString::MenuEditPatient),
		std::bind(&PatientsController::EditPatient, &m_patientsController));
	menuDataEdit.AddItem(m_language.GetString(LanguageString::MenuRemovePatient),
		std::bind(&PatientsController::RemovePatient, &m_patientsController));

	ConsoleMenu menuDataSearch(m_console, m_language);
	menuDataSearch.AddItem(m_language.GetString(LanguageString::MenuSearchPatientsByName),
		std::bind(&PatientsController::SearchPatientsByName, &m_patientsController));
	menuDataSearch.AddItem(m_language.GetString(LanguageString::MenuSearchPatientsByCity),
		std::bind(&PatientsController::SearchPatientsByCity, &m_patientsController));
	menuDataSearch.AddItem(m_language.GetString(LanguageString::MenuSearchPatientsByDiagnose),
		std::bind(&PatientsController::SearchPatientsByDiagnose, &m_patientsController));
	menuDataSearch.AddItem(m_language.GetString(LanguageString::MenuSearchPatientsByPhone),
		std::bind(&PatientsController::SearchPatientsByPhone, &m_patientsController));

	ConsoleMenu menuDataOrder(m_console, m_language);
	menuDataOrder.AddItem(m_language.GetString(LanguageString::MenuOrderPatientsByName),
		std::bind(&PatientsController::OrderPatientsByName, &m_patientsController));
	menuDataOrder.AddItem(m_language.GetString(LanguageString::MenuOrderPatientsByCity),
		std::bind(&PatientsController::OrderPatientsByCity, &m_patientsController));
	menuDataOrder.AddItem(m_language.GetString(LanguageString::MenuOrderPatientsByBirthDate),
		std::bind(&PatientsController::OrderPatientsByBirthDate, &m_patientsController));

	ConsoleMenu menuDataView(m_console, m_language);
	menuDataView.AddItem(m_language.GetString(LanguageString::MenuViewNonresidentPatients),
		std::bind(&PatientsController::ViewNonresidentPatients, &m_patientsController));
	menuDataView.AddItem(m_language.GetString(LanguageString::MenuViewPatientsByAgeAndDiagnose),
		std::bind(&PatientsController::ViewPatientsByAgeAndDiagnose, &m_patientsController));
	menuDataView.AddMenu(m_language.GetString(LanguageString::MenuSearchPatients),
		menuDataSearch);
	menuDataView.AddMenu(m_language.GetString(LanguageString::MenuOrderPatients),
		menuDataOrder);

	ConsoleMenu menuData(m_console, m_language);
	menuData.AddMenu(m_language.GetString(LanguageString::MenuEditPatientsData),
		menuDataEdit);
	menuData.AddMenu(m_language.GetString(LanguageString::MenuProcessPatientsData),
		menuDataView);

	m_menu.AddMenu(m_language.GetString(LanguageString::MenuControlUsers),
		menuUsers);
	m_menu.AddMenu(m_language.GetString(LanguageString::MenuControlPatientsDatabasesFiles),
		menuFile);
	m_menu.AddMenu(m_language.GetString(LanguageString::MenuControlData),
		menuData);
}
