#pragma once

#include "../UsersRegistry/UsersRegistry.h"

class Console;
class Language;


class UsersController
{
public:
	explicit UsersController(const std::string& usersDatabaseFilePath, Console& console, const Language& language);
	~UsersController();

	void PrintAllUsers() const;
	void AddNewUser();
	void RemoveUser();
	void EditUser();

private:
	const Language& GetLanguage() const;
	void PrintUsers(const std::vector<UserRecord_t>& usersRecords) const;
	void PrintUser(const UserRecord_t& userRecord) const;
	const std::string GetUserDisplayRole(const UserRole userRole) const;
	bool RequestUserId(uint64_t& userId);

private:
	const Language& m_language;
	Console& m_console;
	std::shared_ptr<UsersRegistry> m_usersRegistry;
};

