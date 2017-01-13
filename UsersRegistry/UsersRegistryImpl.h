#pragma once

#include "../DatabaseSQLite/DatabaseSQLite.h"
#include "User.h"


class UsersRegistryImpl
{
public:
	explicit UsersRegistryImpl(const std::string& connectionString, bool createIfNotExist, bool readOnly);

	UsersRegistryImpl() = delete;
	UsersRegistryImpl(const UsersRegistryImpl&) = delete;
	UsersRegistryImpl& operator = (const UsersRegistryImpl&) = delete;

public:
	const std::vector<UserRecord_t> GetAllUsers() const;
	uint64_t GetAllUsersCount() const;
	uint64_t GetAdminUsersCount() const;
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
	void ValidateDatabaseSQLiteSchema();

	bool IsExistUserInDatabase(const std::string& login) const;

	UserRole GetRoleById(const uint32_t roleId) const;
	uint32_t GetRoleId(const UserRole role);

private:
	mutable  std::mutex m_mutex;

	DatabaseSQLite m_sqliteDatabase;

	SQLiteStatement_t m_checkUserPasswordStatement;
};

