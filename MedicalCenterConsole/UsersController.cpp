#include "stdafx.h"
#include "UsersController.h"
#include "Console.h"
#include "ConsoleInputFormAddUser.h"
#include "Language.h"


UsersController::UsersController(const std::string& usersDatabaseFilePath, Console& console, const Language& language)
	:
	m_usersRegistry(UsersRegistry::OpenUsersRegistry(usersDatabaseFilePath, false)),
	m_language(language),
	m_console(console)
{
}


UsersController::~UsersController()
{
}


void UsersController::PrintAllUsers() const
{
	const std::vector<UserRecord_t> allUsers = m_usersRegistry->GetAllUsers();

	PrintUsers(allUsers);
}


void UsersController::AddNewUser()
{
	User user;
	UserRole role(UserRole::User);
	std::string password;

	if (m_console.RequestInputYesNo(GetLanguage().GetString(LanguageString::CreateNewUserAsAdmin), false))
	{
		role = UserRole::Admin;
	}

	while (true)
	{
		m_console.PrintYellowWithNewLine(GetLanguage().GetString(LanguageString::NewUserFormCaption));

		ConsoleInputFormAddUser formAddUser(m_console, m_language);

		if (formAddUser.GetPassword() == formAddUser.GetPasswordConfirm())
		{
			user = User(formAddUser.GetUsername(), role);
			password = formAddUser.GetPassword();

			break;
		}
		else
		{
			m_console.PrintErrorWithNewLine(GetLanguage().GetString(LanguageString::PasswordsNotEq));

			if (!m_console.RequestInputYesNo(GetLanguage().GetString(LanguageString::QuestionRepeatEnterYesNo), true))
			{
				return;
			}
		}
	}

	if (m_usersRegistry->IsExistUser(user.GetLogin()))
	{
		m_console.PrintErrorWithNewLine(GetLanguage().GetString(LanguageString::UserLoginExistAlready));

		return;
	}

	if (m_usersRegistry->AddUser(user, password))
	{
		m_console.PrintYellowWithNewLine(GetLanguage().GetString(LanguageString::UsersAdded));
	}
	else
	{
		m_console.PrintErrorWithNewLine(GetLanguage().GetString(LanguageString::CantAddUser));
	}
}


void UsersController::RemoveUser()
{
	uint64_t userId;
	
	if (!RequestUserId(userId))
	{
		return;
	}

	if (!m_console.RequestInputYesNo(GetLanguage().GetString(LanguageString::RemoveUserConfirm), false))
	{
		return;
	}

	if (m_usersRegistry->DeleteUser(userId))
	{
		m_console.PrintYellowWithNewLine(GetLanguage().GetString(LanguageString::UserRemoved));
	}
	else
	{
		m_console.PrintErrorWithNewLine(GetLanguage().GetString(LanguageString::CantRemoveUser));
	}
}


bool UsersController::RequestUserId(uint64_t& userId)
{
	m_console.PrintWhite(GetLanguage().GetString(LanguageString::EnterUserId));
	uint64_t requestUserId = m_console.RequestInputInteger();

	const UserRecord_t existUserRecord = m_usersRegistry->GetUserById(requestUserId);

	if (existUserRecord.second == User::GetEmptyUser())
	{
		m_console.PrintErrorWithNewLine(GetLanguage().GetString(LanguageString::UserIdNotFound));

		return false;
	}

	m_console.PrintInfoWithNewLine(GetLanguage().GetString(LanguageString::UserFound));
	PrintUser(existUserRecord);

	userId = requestUserId;

	return true;
}


void UsersController::EditUser()
{
	uint64_t userId;

	if (!RequestUserId(userId))
	{
		return;
	}

	if (!m_console.RequestInputYesNo(GetLanguage().GetString(LanguageString::EditUserConfirm), false))
	{
		return;
	}

	const UserRecord_t existUserRecord = m_usersRegistry->GetUserById(userId);
	User existUser = existUserRecord.second;

	UserRole newRole;
	std::string newName;
	bool changePassword = false;
	std::string newPassword;

	if (m_console.RequestInputYesNo(GetLanguage().GetString(LanguageString::EditUserChangeRole), false))
	{
		if (m_console.RequestInputYesNo(GetLanguage().GetString(LanguageString::EditUserAsAdmin), false))
		{
			newRole = UserRole::Admin;
		}
		else
		{
			newRole = UserRole::User;
		}
	}
	else
	{
		newRole = existUser.GetRole();
	}

	if (m_console.RequestInputYesNo(GetLanguage().GetString(LanguageString::EditUserChangeName), false))
	{
		m_console.PrintWhite(GetLanguage().GetString(LanguageString::EditUserNewName));
		newName = m_console.RequestInputNonEmptyString();

		if (m_usersRegistry->IsExistUser(newName) &&
			m_usersRegistry->GetUserByLogin(newName).first != userId)
		{
			m_console.PrintErrorWithNewLine(GetLanguage().GetString(LanguageString::UserLoginExistAlready));

			return;
		}
	}
	else
	{
		newName = existUser.GetLogin();
	}

	while (true)
	{
		if (!m_console.RequestInputYesNo(GetLanguage().GetString(LanguageString::EditUserChangePassword), false))
		{
			break;
		}

		m_console.PrintWhite(GetLanguage().GetString(LanguageString::EditUserNewPassword));
		std::string tempNewPassword = m_console.RequestInputPassword();

		m_console.PrintWhite(GetLanguage().GetString(LanguageString::EditUserNewPasswordConfirm));
		std::string tempNewPasswordConfirm = m_console.RequestInputPassword();

		if (tempNewPassword == tempNewPasswordConfirm)
		{
			newPassword = tempNewPassword;
			changePassword = true;

			break;
		}
		else
		{
			m_console.PrintErrorWithNewLine(GetLanguage().GetString(LanguageString::PasswordsNotEq));

			if (!m_console.RequestInputYesNo(GetLanguage().GetString(LanguageString::QuestionRepeatEnterYesNo), true))
			{
				break;
			}
		}
	}

	User updateUser(newName, newRole);
	bool updateUserSuccess;

	if (changePassword)
	{
		updateUserSuccess = m_usersRegistry->UpdateUserAndPassword(userId, updateUser, newPassword);
	}
	else
	{
		updateUserSuccess = m_usersRegistry->UpdateUser(userId, updateUser);
	}

	if (updateUserSuccess)
	{
		m_console.PrintYellowWithNewLine(GetLanguage().GetString(LanguageString::UserChanged));
	}
	else
	{
		m_console.PrintErrorWithNewLine(GetLanguage().GetString(LanguageString::CantChangeUser));
	}
}


void UsersController::PrintUsers(const std::vector<UserRecord_t>& usersRecords) const
{
	if (usersRecords.empty())
	{
		m_console.PrintYellowWithNewLine(GetLanguage().GetString(LanguageString::NotFoundAnyRecord));

		return;
	}

	m_console.PrintWhiteWithNewLine("--------------------------------------------------------------------------------");

	for (const auto& userRecord : usersRecords)
	{
		PrintUser(userRecord);
	}

	m_console.PrintWhiteWithNewLine("--------------------------------------------------------------------------------");
	m_console.PrintWhite(GetLanguage().GetString(LanguageString::FoundRecords));
	m_console.PrintWhite(u8"%zu\r\n", usersRecords.size());
}


void UsersController::PrintUser(const UserRecord_t& userRecord) const
{
	const uint64_t userId = userRecord.first;
	const User& user = userRecord.second;

	m_console.PrintInfoWithNewLine("%s: %I64u, %s: %s, %s: %s",
		GetLanguage().GetString(LanguageString::UserID),
		userId,
		GetLanguage().GetString(LanguageString::UserName),
		user.GetLogin().c_str(),
		GetLanguage().GetString(LanguageString::UserRole),
		GetUserDisplayRole(user.GetRole()).c_str());
}


const std::string UsersController::GetUserDisplayRole(const UserRole userRole) const
{
	switch (userRole)
	{
	case UserRole::Admin:
		return GetLanguage().GetString(LanguageString::UserRoleIsAdmin);
		break;

	case UserRole::User:
		return GetLanguage().GetString(LanguageString::UserRoleIsUser);
		break;
	}

	throw std::logic_error("unknown user role");
}


const Language& UsersController::GetLanguage() const
{
	return m_language;
}
