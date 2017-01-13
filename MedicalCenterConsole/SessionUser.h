#pragma once

#include "Session.h"
#include "ConsoleMenu.h"
#include "PatientsController.h"

class Console;
class Language;


class SessionUser : public Session
{
public:
	explicit SessionUser(const std::string& userName, const Language& language, Console& console,
		std::shared_ptr<PatientsRegistry> patientsRegistry);
	~SessionUser();

	void StartSessionAndWaitExit() override;

private:
	void CreateMenu();

private:
	const std::string m_userName;
	const Language& m_language;
	Console& m_console;
	PatientsController m_patientsController;
	ConsoleMenu m_menu;
};

