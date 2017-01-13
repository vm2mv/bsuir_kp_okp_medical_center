#pragma once

#include "User.h"

class UsersRegistryImpl;


class UsersRegistry
{
public:
	static std::shared_ptr<UsersRegistry> CreateUsersRegistry(const std::string& connectionString);
	static std::shared_ptr<UsersRegistry> OpenUsersRegistry(const std::string& connectionString, bool readOnly);

	UsersRegistry() = delete;
	UsersRegistry(const UsersRegistry&) = delete;
	UsersRegistry& operator = (const UsersRegistry&) = delete;

private:
	explicit UsersRegistry(const std::string& connectionString, bool createIfNotExist, bool readOnly);

public:
	uint64_t GetAllUsersCount() const;
	uint64_t GetAdminUsersCount() const;
	const std::vector<UserRecord_t> GetAllUsers() const;
	bool AddUser(const User& user, const std::string& password);
	bool DeleteUser(const UserId_t userId);
	bool DeleteUserByLogin(const std::string& login);
	bool UpdateUser(const UserId_t userId, const User& user);
	bool UpdateUserAndPassword(const UserId_t userId, const User& user, const std::string& password);
	bool IsExistUser(const std::string& login) const;
	UserRecord_t GetUserByLogin(const std::string& login) const;
	UserRecord_t GetUserById(const UserId_t userId) const;

	bool CheckUserPassword(const std::string& login, const std::string& password) const;

private:
	std::unique_ptr<UsersRegistryImpl> m_usersRegistryImpl;
};

