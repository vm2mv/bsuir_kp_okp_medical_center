#include "stdafx.h"
#include "SessionUser.h"
#include "../PatientsRegistry/PatientsRegistry.h"
#include "Console.h"
#include "Language.h"


SessionUser::SessionUser(const std::string& userName, const Language& language, Console& console,
	std::shared_ptr<PatientsRegistry> patientsRegistry) 
	:
	m_userName(userName),
	m_language(language),
	m_console(console),
	m_patientsController(console, language, patientsRegistry),
	m_menu(console, language)
{
	CreateMenu();
}


SessionUser::~SessionUser()
{
}


void SessionUser::StartSessionAndWaitExit()
{
	m_menu.StartMenuAndWaitExit();
}


void SessionUser::CreateMenu()
{
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

	m_menu.AddItem(m_language.GetString(LanguageString::MenuViewAllPatients),
		std::bind(&PatientsController::ViewAllPatients, &m_patientsController));
	m_menu.AddItem(m_language.GetString(LanguageString::MenuViewNonresidentPatients),
		std::bind(&PatientsController::ViewNonresidentPatients, &m_patientsController));
	m_menu.AddItem(m_language.GetString(LanguageString::MenuViewPatientsByAgeAndDiagnose),
		std::bind(&PatientsController::ViewPatientsByAgeAndDiagnose, &m_patientsController));
	
	m_menu.AddMenu(m_language.GetString(LanguageString::MenuSearchPatients),
		menuDataSearch);
	m_menu.AddMenu(m_language.GetString(LanguageString::MenuOrderPatients),
		menuDataOrder);
}
