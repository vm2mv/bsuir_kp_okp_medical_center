#include "stdafx.h"
#include "UsersRegistryImpl.h"
#include "DatabaseSchemaSql.h"
#include "PasswordKeyGenerator.h"
#include "../DatabaseSQLite/DatabaseSQLiteException.h"


UsersRegistryImpl::UsersRegistryImpl(const std::string& connectionString, bool createIfNotExist, bool readOnly) :
	m_sqliteDatabase(connectionString, createIfNotExist, readOnly)
{
	if (!readOnly)
	{
		ValidateDatabaseSQLiteSchema();
	}

	m_checkUserPasswordStatement = m_sqliteDatabase.CreateStatement(DatabaseSchemaSql::SQL_CHECK_USER_AND_PASSWORD);
}


bool UsersRegistryImpl::IsExistUser(const std::string& login) const
{
	std::lock_guard<std::mutex> lock(m_mutex);

	return IsExistUserInDatabase(login);
}


bool UsersRegistryImpl::AddUser(const User& user, const std::string& password)
{
	std::lock_guard<std::mutex> lock(m_mutex);

	if (IsExistUserInDatabase(user.GetLogin()))
	{
		return false;
	}

	const std::string passwordKey = PasswordKeyGenerator::GenerateKey_PBKDF2_HMAC(password);

	SQLiteStatement_t addUserStatement = m_sqliteDatabase.CreateStatement(DatabaseSchemaSql::SQL_INSERT_USER);

	sqlite3_bind_text(addUserStatement.get(), 1, user.GetLogin().c_str(), (int)user.GetLogin().size(), SQLITE_STATIC);
	sqlite3_bind_text(addUserStatement.get(), 2, passwordKey.c_str(), (int)passwordKey.size(), SQLITE_STATIC);
	sqlite3_bind_int(addUserStatement.get(), 3, GetRoleId(user.GetRole()));

	int sqliteStepResult = sqlite3_step(addUserStatement.get());

	if (sqliteStepResult != SQLITE_DONE)
	{
		const char* pExecErrorMessage = sqlite3_errmsg(m_sqliteDatabase.GetSqliteConnectionPtr());

		throw DatabaseSQLiteException(pExecErrorMessage != nullptr ? pExecErrorMessage : "",
			sqliteStepResult);
	}

	return true;
}


bool UsersRegistryImpl::DeleteUser(const UserId_t userId)
{
	std::lock_guard<std::mutex> lock(m_mutex);

	SQLiteStatement_t deleteUserStatement = m_sqliteDatabase.CreateStatement(DatabaseSchemaSql::SQL_DELETE_USER_BY_ID);
	
	sqlite3_bind_int64(deleteUserStatement.get(), 1, userId);

	int sqliteStepResult = sqlite3_step(deleteUserStatement.get());

	if (sqliteStepResult == SQLITE_DONE)
	{
		if (sqlite3_changes(m_sqliteDatabase.GetSqliteConnectionPtr()) == 0)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	else
	{
		const char* pExecErrorMessage = sqlite3_errmsg(m_sqliteDatabase.GetSqliteConnectionPtr());

		throw DatabaseSQLiteException(pExecErrorMessage != nullptr ? pExecErrorMessage : "",
			sqliteStepResult);
	}
}


bool UsersRegistryImpl::DeleteUserByLogin(const std::string& login)
{
	std::lock_guard<std::mutex> lock(m_mutex);

	SQLiteStatement_t deleteUserStatement = m_sqliteDatabase.CreateStatement(DatabaseSchemaSql::SQL_DELETE_USER_BY_LOGIN);

	sqlite3_bind_text(deleteUserStatement.get(), 1, login.c_str(), (int)login.size(), SQLITE_STATIC);

	int sqliteStepResult = sqlite3_step(deleteUserStatement.get());

	if (sqliteStepResult == SQLITE_DONE)
	{
		if (sqlite3_changes(m_sqliteDatabase.GetSqliteConnectionPtr()) == 0)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	else
	{
		const char* pExecErrorMessage = sqlite3_errmsg(m_sqliteDatabase.GetSqliteConnectionPtr());

		throw DatabaseSQLiteException(pExecErrorMessage != nullptr ? pExecErrorMessage : "",
			sqliteStepResult);
	}
}


bool UsersRegistryImpl::UpdateUser(const UserId_t userId, const User& user)
{
	std::lock_guard<std::mutex> lock(m_mutex);

	SQLiteStatement_t addUserStatement = m_sqliteDatabase.CreateStatement(
		DatabaseSchemaSql::SQL_UPDATE_USER_WITHOUT_PASSWORD);

	sqlite3_bind_text(addUserStatement.get(), 1, user.GetLogin().c_str(), (int)user.GetLogin().size(), SQLITE_STATIC);
	sqlite3_bind_int(addUserStatement.get(), 2, GetRoleId(user.GetRole()));
	sqlite3_bind_int64(addUserStatement.get(), 3, userId);

	int sqliteStepResult = sqlite3_step(addUserStatement.get());

	if (sqliteStepResult == SQLITE_DONE)
	{
		if (sqlite3_changes(m_sqliteDatabase.GetSqliteConnectionPtr()) == 0)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	else
	{
		const char* pExecErrorMessage = sqlite3_errmsg(m_sqliteDatabase.GetSqliteConnectionPtr());

		throw DatabaseSQLiteException(pExecErrorMessage != nullptr ? pExecErrorMessage : "",
			sqliteStepResult);
	}
}


bool UsersRegistryImpl::UpdateUserAndPassword(const UserId_t userId, const User& user, const std::string& password)
{
	std::lock_guard<std::mutex> lock(m_mutex);

	const std::string passwordKey = PasswordKeyGenerator::GenerateKey_PBKDF2_HMAC(password);

	SQLiteStatement_t addUserStatement = m_sqliteDatabase.CreateStatement(DatabaseSchemaSql::SQL_UPDATE_USER);

	sqlite3_bind_text(addUserStatement.get(), 1, user.GetLogin().c_str(), (int)user.GetLogin().size(), SQLITE_STATIC);
	sqlite3_bind_text(addUserStatement.get(), 2, passwordKey.c_str(), (int)passwordKey.size(), SQLITE_STATIC);
	sqlite3_bind_int(addUserStatement.get(), 3, GetRoleId(user.GetRole()));
	sqlite3_bind_int64(addUserStatement.get(), 4, userId);

	int sqliteStepResult = sqlite3_step(addUserStatement.get());

	if (sqliteStepResult == SQLITE_DONE)
	{
		if (sqlite3_changes(m_sqliteDatabase.GetSqliteConnectionPtr()) == 0)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	else
	{
		const char* pExecErrorMessage = sqlite3_errmsg(m_sqliteDatabase.GetSqliteConnectionPtr());

		throw DatabaseSQLiteException(pExecErrorMessage != nullptr ? pExecErrorMessage : "",
			sqliteStepResult);
	}
}


const std::vector<UserRecord_t> UsersRegistryImpl::GetAllUsers() const
{
	std::lock_guard<std::mutex> lock(m_mutex);

	std::vector<UserRecord_t> allUsers;

	SQLiteStatement_t selectUsersStatement = m_sqliteDatabase.CreateStatement(DatabaseSchemaSql::SQL_SELECT_ALL_USERS);

	int sqliteStepResult = sqlite3_step(selectUsersStatement.get());

	while (sqliteStepResult == SQLITE_ROW)
	{
		const uint64_t userId = sqlite3_column_int64(selectUsersStatement.get(), 0);
		const std::string login = (const char*)sqlite3_column_text(selectUsersStatement.get(), 1);
		const uint32_t roleId = (uint32_t)sqlite3_column_int(selectUsersStatement.get(), 2);

		allUsers.emplace_back(std::make_pair(userId, User{
			login,
			GetRoleById(roleId)
		}));

		sqliteStepResult = sqlite3_step(selectUsersStatement.get());
	}

	if (sqliteStepResult != SQLITE_DONE)
	{
		const char* pExecErrorMessage = sqlite3_errmsg(m_sqliteDatabase.GetSqliteConnectionPtr());

		throw DatabaseSQLiteException(pExecErrorMessage != nullptr ? pExecErrorMessage : "",
			sqliteStepResult);
	}

	return allUsers;
}


uint64_t UsersRegistryImpl::GetAllUsersCount() const
{
	uint64_t allUsersCount = 0;

	std::lock_guard<std::mutex> lock(m_mutex);
	
	SQLiteStatement_t selectStatement = m_sqliteDatabase.CreateStatement(DatabaseSchemaSql::SQL_SELECT_USERS_COUNT);

	int sqliteStepResult = sqlite3_step(selectStatement.get());

	if (sqliteStepResult == SQLITE_ROW)
	{
		allUsersCount = sqlite3_column_int64(selectStatement.get(), 0);
	}

	return allUsersCount;
}


uint64_t UsersRegistryImpl::GetAdminUsersCount() const
{
	uint64_t adminUsersCount = 0;

	std::lock_guard<std::mutex> lock(m_mutex);

	SQLiteStatement_t selectStatement = m_sqliteDatabase.CreateStatement(
		DatabaseSchemaSql::SQL_SELECT_ADMIN_USERS_COUNT);

	int sqliteStepResult = sqlite3_step(selectStatement.get());

	if (sqliteStepResult == SQLITE_ROW)
	{
		adminUsersCount = sqlite3_column_int64(selectStatement.get(), 0);
	}

	return adminUsersCount;
}


bool UsersRegistryImpl::CheckUserPassword(const std::string& login, const std::string& password) const
{
	const std::string passwordKey = PasswordKeyGenerator::GenerateKey_PBKDF2_HMAC(password);
	
	std::lock_guard<std::mutex> lock(m_mutex);

	sqlite3_reset(m_checkUserPasswordStatement.get());
	sqlite3_bind_text(m_checkUserPasswordStatement.get(), 1, login.c_str(), (int)login.size(), SQLITE_STATIC);
	sqlite3_bind_text(m_checkUserPasswordStatement.get(), 2, passwordKey.c_str(), (int)passwordKey.size(), SQLITE_STATIC);

	int sqliteStepResult = sqlite3_step(m_checkUserPasswordStatement.get());

	if (sqliteStepResult == SQLITE_ROW)
	{
		return true;
	}
	else if (sqliteStepResult == SQLITE_DONE)
	{
		return false;
	}
	else
	{
		const char* pExecErrorMessage = sqlite3_errmsg(m_sqliteDatabase.GetSqliteConnectionPtr());

		throw DatabaseSQLiteException(pExecErrorMessage != nullptr ? pExecErrorMessage : "",
			sqliteStepResult);
	}
}


UserRecord_t UsersRegistryImpl::GetUserByLogin(const std::string& login) const
{
	std::lock_guard<std::mutex> lock(m_mutex);

	SQLiteStatement_t selectUserStatement = m_sqliteDatabase.CreateStatement(
		DatabaseSchemaSql::SQL_SELECT_USER_ID_AND_ROLE_BY_LOGIN);

	sqlite3_bind_text(selectUserStatement.get(), 1, login.c_str(), (int)login.size(), SQLITE_STATIC);

	int sqliteStepResult = sqlite3_step(selectUserStatement.get());

	if (sqliteStepResult == SQLITE_ROW)
	{
		const uint64_t userId = sqlite3_column_int64(selectUserStatement.get(), 0);
		const uint32_t roleId = (uint32_t)sqlite3_column_int(selectUserStatement.get(), 1);

		UserRecord_t userRecord = std::make_pair(userId, User{
			login,
			GetRoleById(roleId)
		});

		return userRecord;
	}
	else if (sqliteStepResult == SQLITE_DONE)
	{
		return std::make_pair(USERID_EMPTY, User::GetEmptyUser());
	}
	else
	{
		const char* pExecErrorMessage = sqlite3_errmsg(m_sqliteDatabase.GetSqliteConnectionPtr());

		throw DatabaseSQLiteException(pExecErrorMessage != nullptr ? pExecErrorMessage : "",
			sqliteStepResult);
	}
}


UserRecord_t UsersRegistryImpl::GetUserById(const UserId_t userId) const
{
	if (userId == USERID_EMPTY)
	{
		return std::make_pair(userId, User::GetEmptyUser());
	}

	std::lock_guard<std::mutex> lock(m_mutex);

	SQLiteStatement_t selectUserStatement = m_sqliteDatabase.CreateStatement(
		DatabaseSchemaSql::SQL_SELECT_USER_LOGIN_AND_ROLE_BY_ID);

	sqlite3_bind_int64(selectUserStatement.get(), 1, userId);

	int sqliteStepResult = sqlite3_step(selectUserStatement.get());

	if (sqliteStepResult != SQLITE_ROW)
	{
		return std::make_pair(USERID_EMPTY, User::GetEmptyUser());
	}

	const std::string login = (const char*)sqlite3_column_text(selectUserStatement.get(), 0);
	const uint32_t roleId = (uint32_t)sqlite3_column_int(selectUserStatement.get(), 1);

	UserRecord_t userRecord = std::make_pair(userId, User{
		login,
		GetRoleById(roleId)
	});

	return userRecord;
}


bool UsersRegistryImpl::IsExistUserInDatabase(const std::string& login) const
{
	SQLiteStatement_t selectUserStatement = m_sqliteDatabase.CreateStatement(
		DatabaseSchemaSql::SQL_SELECT_USER_ID_BY_LOGIN);

	sqlite3_bind_text(selectUserStatement.get(), 1, login.c_str(), (int)login.size(), SQLITE_STATIC);

	int sqliteStepResult = sqlite3_step(selectUserStatement.get());

	if (sqliteStepResult == SQLITE_ROW)
	{
		return true;
	}
	else if (sqliteStepResult == SQLITE_DONE)
	{
		return false;
	}
	else
	{
		const char* pExecErrorMessage = sqlite3_errmsg(m_sqliteDatabase.GetSqliteConnectionPtr());

		throw DatabaseSQLiteException(pExecErrorMessage != nullptr ? pExecErrorMessage : "",
			sqliteStepResult);
	}
}


UserRole UsersRegistryImpl::GetRoleById(const uint32_t roleId) const
{
	return DatabaseSchemaSql::DATABASE_SQLITE_USER_ROLES_BIND_TABLE.at(roleId);
}


uint32_t UsersRegistryImpl::GetRoleId(const UserRole role)
{
	const auto& bindMap = DatabaseSchemaSql::DATABASE_SQLITE_USER_ROLES_BIND_TABLE;

	auto it = std::find_if(bindMap.cbegin(), bindMap.cend(), [&role](const std::pair<uint32_t, UserRole>& t) -> bool 
	{
		return (t.second == role);
	});

	if (it == bindMap.cend())
	{
		throw std::invalid_argument("Invalid role value");
	}

	return it->first;
}


void UsersRegistryImpl::ValidateDatabaseSQLiteSchema()
{
	m_sqliteDatabase.BeginTransaction();

	try
	{
		m_sqliteDatabase.ExecuteSql(DatabaseSchemaSql::USERS_DATABASE_SQLITE_SCHEMA);
	}
	catch (...)
	{
		m_sqliteDatabase.RollbackTransaction();

		throw;
	}

	m_sqliteDatabase.CommitTransaction();
}
