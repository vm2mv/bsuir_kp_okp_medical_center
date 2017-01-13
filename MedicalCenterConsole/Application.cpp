#include "stdafx.h"
#include "Application.h"
#include "Utils.h"
#include "ConsoleInputFormAddUser.h"
#include "ConsoleInputFormLogin.h"
#include "SessionsFactory.h"


const std::string Application::USERS_REGISTRY_DATABASE_FILENAME = "MedicalCenterUsers.db";
const std::string Application::PATIENTS_REGISTRY_DATABASE_FILENAME_DEFAULT = "MedicalCenterPatients.db";


Application::Application() :
	m_console(m_language)
{
}


void Application::StartAndWaitExit()
{
	PrintWelcomText();

	try
	{
		if (!ValidateUsersDatabase())
		{
			return;
		}
	}
	catch (const std::exception& ex)
	{
		m_console.PrintErrorWithNewLine(GetLanguage().GetString(LanguageString::CantOpenUsersDatabase));
		m_console.PrintErrorWithNewLine("\t %s", ex.what());

		return;
	}

	try
	{
		std::string userName;
		UserRole userRole;
		bool authenticationSuccess = AuthenticateUser(userName, userRole);

		if (!authenticationSuccess)
		{
			return;
		}

		m_console.PrintYellowWithNewLine(GetLanguage().GetString(LanguageString::LoginSuccess));

		std::shared_ptr<PatientsRegistry> patientsRegistry;

		if (userRole == UserRole::Admin)
		{
			m_console.PrintYellowWithNewLine(GetLanguage().GetString(LanguageString::LoginedUserIsAdmin));

			patientsRegistry = OpenPatientsRegistryForAdmin();
		}
		else
		{
			patientsRegistry = OpenPatientsRegistryForUser();

			if (patientsRegistry == nullptr)
			{
				return;
			}
		}

		SessionsFactory sessionsFactory(m_language, m_console, USERS_REGISTRY_DATABASE_FILENAME);
		std::shared_ptr<Session> session = sessionsFactory.CreateSession(userName, userRole, patientsRegistry);

		session->StartSessionAndWaitExit();
	}
	catch (...)
	{
		m_console.PrintErrorWithNewLine(GetLanguage().GetString(
			LanguageString::UnknowError));

		throw;
	}
}


bool Application::AuthenticateUser(std::string& userName, UserRole& userRole)
{
	bool authenticationSuccess = false;
	
	std::shared_ptr<UsersRegistry> usersRegistry =
		UsersRegistry::OpenUsersRegistry(USERS_REGISTRY_DATABASE_FILENAME, true);

	while (!authenticationSuccess)
	{
		ConsoleInputFormLogin formLogin(m_console, m_language);

		authenticationSuccess = usersRegistry->CheckUserPassword(formLogin.GetUsername(), formLogin.GetPassword());

		if (authenticationSuccess)
		{
			UserRecord_t userRecord = usersRegistry->GetUserByLogin(formLogin.GetUsername());

			userName = userRecord.second.GetLogin();
			userRole = userRecord.second.GetRole();
		}
		else
		{
			m_console.PrintErrorWithNewLine(GetLanguage().GetString(LanguageString::LoginFailed));

			if (!m_console.RequestInputYesNo(GetLanguage().GetString(LanguageString::QuestionRepeatEnterYesNo), true))
			{
				break;
			}
		}
	}

	return authenticationSuccess;
}


std::shared_ptr<PatientsRegistry> Application::OpenPatientsRegistryForAdmin()
{
	std::shared_ptr<PatientsRegistry> patientsRegistry;

	if (!boost::filesystem::exists(PATIENTS_REGISTRY_DATABASE_FILENAME_DEFAULT))
	{
		patientsRegistry = PatientsRegistry::CreatePatientsRegistry(PATIENTS_REGISTRY_DATABASE_FILENAME_DEFAULT);

		if (patientsRegistry != nullptr)
		{
			m_console.PrintYellowWithNewLine(
				GetLanguage().GetString(LanguageString::PatientsDatabaseDefaultHasCreatedAndOpenend),
				PATIENTS_REGISTRY_DATABASE_FILENAME_DEFAULT.c_str());
		}
	}
	else
	{
		patientsRegistry = PatientsRegistry::OpenPatientsRegistry(PATIENTS_REGISTRY_DATABASE_FILENAME_DEFAULT, false);

		if (patientsRegistry != nullptr)
		{
			m_console.PrintYellowWithNewLine(
				GetLanguage().GetString(LanguageString::PatientsDatabaseDefaultHasOpenend),
				PATIENTS_REGISTRY_DATABASE_FILENAME_DEFAULT.c_str());
		}
	}

	return patientsRegistry;
}


std::shared_ptr<PatientsRegistry> Application::OpenPatientsRegistryForUser()
{
	std::shared_ptr<PatientsRegistry> patientsRegistry;
	
	while (true)
	{
		std::string filePath;

		if (Utils::RequestExistFilePathWithDefaultValue(m_console, m_language, 
			PATIENTS_REGISTRY_DATABASE_FILENAME_DEFAULT, filePath))
		{
			patientsRegistry = 
				PatientsRegistry::OpenPatientsRegistry(PATIENTS_REGISTRY_DATABASE_FILENAME_DEFAULT, true);

			if (patientsRegistry != nullptr)
			{
				break;
			}
			else
			{
				m_console.PrintErrorWithNewLine(GetLanguage().GetString(LanguageString::CantOpenPatientsDatabase));
			}
		}

		if (!m_console.RequestInputYesNo(GetLanguage().GetString(LanguageString::QuestionRepeatEnterYesNo), true))
		{
			break;
		}
	}

	return patientsRegistry;
}


bool Application::ValidateUsersDatabase()
{
	bool databaseIsExist = false;

	if (boost::filesystem::exists(USERS_REGISTRY_DATABASE_FILENAME))
	{
		std::shared_ptr<UsersRegistry> usersRegistry = 
			UsersRegistry::OpenUsersRegistry(USERS_REGISTRY_DATABASE_FILENAME, true);

		databaseIsExist = true;

		if (usersRegistry->GetAdminUsersCount() > 0)
		{
			return true;
		}
	}

	if (databaseIsExist)
	{
		m_console.PrintYellowWithNewLine(GetLanguage().GetString(LanguageString::AdminNotFoundInUsersDatabase));

		if (!m_console.RequestInputYesNo(GetLanguage().GetString(LanguageString::QuestionCreateFirstAdmin), true))
		{
			return false;
		}
	}
	else
	{
		m_console.PrintYellowWithNewLine(GetLanguage().GetString(LanguageString::CreateNewUsersDatabaseInfo));

		if (!m_console.RequestInputYesNo(GetLanguage().GetString(LanguageString::QuestionContinueYesNo), true))
		{
			return false;
		}

		m_console.PrintYellowWithNewLine(GetLanguage().GetString(LanguageString::CreateAdminInNewUsersDatabaseInfo));
	}

	return CreateDatabaseWithAdmin(databaseIsExist);
}


bool Application::CreateDatabaseWithAdmin(bool databaseIsExist)
{
	User adminUser;
	std::string adminUserPassword;

	while (true)
	{
		m_console.PrintWhiteWithNewLine(GetLanguage().GetString(LanguageString::NewUserFormCaption));

		ConsoleInputFormAddUser formAddUserAdmin(m_console, m_language);

		if (formAddUserAdmin.GetPassword() == formAddUserAdmin.GetPasswordConfirm())
		{
			adminUser = User(formAddUserAdmin.GetUsername(), UserRole::Admin);
			adminUserPassword = formAddUserAdmin.GetPassword();

			break;
		}
		else
		{
			m_console.PrintErrorWithNewLine(GetLanguage().GetString(LanguageString::PasswordsNotEq));

			if (!m_console.RequestInputYesNo(GetLanguage().GetString(LanguageString::QuestionRepeatEnterYesNo), true))
			{
				return false;
			}
		}
	}

	std::shared_ptr<UsersRegistry> usersRegistry;

	if (databaseIsExist)
	{
		usersRegistry = UsersRegistry::OpenUsersRegistry(USERS_REGISTRY_DATABASE_FILENAME, false);
	}
	else
	{
		usersRegistry = UsersRegistry::CreateUsersRegistry(USERS_REGISTRY_DATABASE_FILENAME);

		m_console.PrintYellowWithNewLine(GetLanguage().GetString(LanguageString::UsersDatabaseCreated));
	}

	usersRegistry->AddUser(adminUser, adminUserPassword);

	m_console.PrintYellowWithNewLine(GetLanguage().GetString(LanguageString::AdminCreatedInUsersDatabase));

	return true;
}


const Language& Application::GetLanguage() const
{
	return m_language;
}


void Application::PrintWelcomText()
{
	m_console.PrintWhiteWithNewLine(GetLanguage().GetString(LanguageString::Welcom));
}
