#pragma once

#include "Session.h"
#include "../UsersRegistry/UserRole.h"

class Language;
class Console;
class PatientsRegistry;


class SessionsFactory
{
public:
	explicit SessionsFactory(const Language& language, Console& console, const std::string& usersDatabaseFilePath);
	~SessionsFactory();

	std::shared_ptr<Session> CreateSession(const std::string& userName, const UserRole userRole,
		std::shared_ptr<PatientsRegistry> patientsRegistry);

private:
	const Language& m_language;
	Console& m_console;
	const std::string m_usersDatabaseFilePath;
};

