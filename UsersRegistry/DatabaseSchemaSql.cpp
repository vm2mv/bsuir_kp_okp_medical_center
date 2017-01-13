#include "stdafx.h"
#include "DatabaseSchemaSql.h"


const std::map<uint32_t, UserRole> DatabaseSchemaSql::DATABASE_SQLITE_USER_ROLES_BIND_TABLE = {
	{ 1, UserRole::Admin },
	{ 2, UserRole::User },
};


const std::string DatabaseSchemaSql::USERS_DATABASE_SQLITE_SCHEMA = R"(

CREATE TABLE IF NOT EXISTS roles (
	role_id INTEGER PRIMARY KEY NOT NULL,
	role_name TEXT COLLATE NOCASE NOT NULL UNIQUE
);

CREATE UNIQUE INDEX IF NOT EXISTS roles_name_idx ON roles (role_name);

INSERT OR REPLACE INTO roles(role_id, role_name) VALUES(1, 'admin');
INSERT OR REPLACE INTO roles(role_id, role_name) VALUES(2, 'user');


CREATE TABLE IF NOT EXISTS users (
	user_id INTEGER PRIMARY KEY AUTOINCREMENT,
	user_login TEXT NOT NULL UNIQUE,
	user_pswd_key TEXT NOT NULL,
	role_id INTEGER NOT NULL,

	FOREIGN KEY(role_id) REFERENCES roles(role_id) ON UPDATE CASCADE
);

CREATE UNIQUE INDEX IF NOT EXISTS users_login_idx ON users (user_login);
CREATE INDEX IF NOT EXISTS users_login_with_keys_idx ON users (user_login, user_pswd_key);

)";

const std::string DatabaseSchemaSql::SQL_CHECK_USER_AND_PASSWORD =
	"select user_id, role_id from users where user_login = ? and user_pswd_key = ?;";

const std::string DatabaseSchemaSql::SQL_INSERT_USER =
	"insert into users (user_login, user_pswd_key, role_id) values (?, ?, ?);";

const std::string DatabaseSchemaSql::SQL_UPDATE_USER_WITHOUT_PASSWORD =
	"update users set user_login = ?, role_id = ? where user_id = ?;";

const std::string DatabaseSchemaSql::SQL_UPDATE_USER =
	"update users set user_login = ?, user_pswd_key = ?, role_id = ? where user_id = ?;";

const std::string DatabaseSchemaSql::SQL_DELETE_USER_BY_ID =
	"delete from users where user_id = ?;";

const std::string DatabaseSchemaSql::SQL_DELETE_USER_BY_LOGIN =
	"delete from users where user_login = ?;";

const std::string DatabaseSchemaSql::SQL_SELECT_ALL_USERS =
	"select user_id, user_login, role_id from users;";

const std::string DatabaseSchemaSql::SQL_SELECT_USERS_COUNT =
	"select count(*) from users;";

const std::string DatabaseSchemaSql::SQL_SELECT_ADMIN_USERS_COUNT =
	"select count(*) from users where role_id = 1;";

const std::string DatabaseSchemaSql::SQL_SELECT_USER_ID_AND_ROLE_BY_LOGIN =
	"select user_id, role_id from users where user_login = ?;";

const std::string DatabaseSchemaSql::SQL_SELECT_USER_LOGIN_AND_ROLE_BY_ID =
	"select user_login, role_id from users where user_id = ?;";

const std::string DatabaseSchemaSql::SQL_SELECT_USER_ID_BY_LOGIN =
	"select user_id from users where user_login = ?;";
