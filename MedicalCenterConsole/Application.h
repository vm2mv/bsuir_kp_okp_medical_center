#pragma once

#include "Console.h"
#include "../PatientsRegistry/PatientsRegistry.h"
#include "../UsersRegistry/UsersRegistry.h"
#include "Language.h"


class Application
{
	static const std::string USERS_REGISTRY_DATABASE_FILENAME;
	static const std::string PATIENTS_REGISTRY_DATABASE_FILENAME_DEFAULT;

public:
	Application();

	void StartAndWaitExit();

private:
	void PrintWelcomText();
	bool ValidateUsersDatabase();
	bool AuthenticateUser(std::string& userName, UserRole& userRole);
	bool CreateDatabaseWithAdmin(bool databaseIsExist);

	std::shared_ptr<PatientsRegistry> OpenPatientsRegistryForAdmin();
	std::shared_ptr<PatientsRegistry> OpenPatientsRegistryForUser();

	const Language& GetLanguage() const;

private:
	Language m_language;
	Console m_console;
};

