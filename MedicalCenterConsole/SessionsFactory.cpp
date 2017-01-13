#include "stdafx.h"
#include "SessionsFactory.h"
#include "SessionAdmin.h"
#include "SessionUser.h"
#include "Language.h"


SessionsFactory::SessionsFactory(const Language& language, Console& console, const std::string& usersDatabaseFilePath) :
	m_language(language),
	m_console(console),
	m_usersDatabaseFilePath(usersDatabaseFilePath)
{
}


SessionsFactory::~SessionsFactory()
{
}


std::shared_ptr<Session> SessionsFactory::CreateSession(const std::string& userName, const UserRole userRole,
	std::shared_ptr<PatientsRegistry> patientsRegistry)
{
	switch (userRole)
	{
	case UserRole::Admin:
		return std::make_shared<SessionAdmin>(userName, m_usersDatabaseFilePath, m_language, m_console, patientsRegistry);
		break;

	case UserRole::User:
		return std::make_shared<SessionUser>(userName, m_language, m_console, patientsRegistry);
		break;

	default:
		throw std::logic_error("unknown user role id");
	}
}
