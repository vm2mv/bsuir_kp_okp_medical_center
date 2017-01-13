#pragma once

#include "Session.h"
#include "ConsoleMenu.h"
#include "UsersController.h"
#include "PatientsController.h"

class Console;
class Language;
class PatientsRegistry;


class SessionAdmin : public Session
{
public:
	explicit SessionAdmin(const std::string& userName, const std::string& usersDatabaseFilePath, 
		const Language& language, Console& console,	std::shared_ptr<PatientsRegistry> patientsRegistry);
	~SessionAdmin();

	void StartSessionAndWaitExit() override;

private:
	void CreateMenu();

private:
	const std::string m_userName;
	const Language& m_language;
	Console& m_console;
	UsersController m_usersController;
	PatientsController m_patientsController;
	ConsoleMenu m_menu;
};

