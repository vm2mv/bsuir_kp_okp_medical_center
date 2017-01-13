#pragma once

#include "UserRole.h"

struct DatabaseSchemaSql
{
	static const std::map<uint32_t, UserRole> DATABASE_SQLITE_USER_ROLES_BIND_TABLE;

	static const std::string USERS_DATABASE_SQLITE_SCHEMA;

	static const std::string SQL_CHECK_USER_AND_PASSWORD;
	static const std::string SQL_INSERT_USER;
	static const std::string SQL_UPDATE_USER_WITHOUT_PASSWORD;
	static const std::string SQL_UPDATE_USER;
	static const std::string SQL_DELETE_USER_BY_ID;
	static const std::string SQL_DELETE_USER_BY_LOGIN;
	static const std::string SQL_SELECT_ALL_USERS;
	static const std::string SQL_SELECT_USERS_COUNT;
	static const std::string SQL_SELECT_ADMIN_USERS_COUNT;
	static const std::string SQL_SELECT_USER_ID_AND_ROLE_BY_LOGIN;
	static const std::string SQL_SELECT_USER_LOGIN_AND_ROLE_BY_ID;
	static const std::string SQL_SELECT_USER_ID_BY_LOGIN;
};
