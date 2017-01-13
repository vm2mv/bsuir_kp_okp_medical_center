#include "stdafx.h"
#include "DatabaseSQLite.h"
#include "DatabaseSQLiteException.h"


const int DatabaseSQLite::SQLITE_BUSY_WAIT_PERIOD_MS = 20;


DatabaseSQLite::DatabaseSQLite(const std::string& connectionString, bool createIfNotExist, bool readOnly) :
	m_connectionString(connectionString)
{
	int openFlags = SQLITE_OPEN_NOMUTEX;

	if (createIfNotExist)
	{
		openFlags |= SQLITE_OPEN_CREATE;
	}

	if (readOnly)
	{
		openFlags |= SQLITE_OPEN_READONLY;
	}
	else
	{
		openFlags |= SQLITE_OPEN_READWRITE;
	}

	sqlite3* pSqliteConnection = nullptr;
	int res = sqlite3_open_v2(connectionString.c_str(), &pSqliteConnection,
		openFlags, nullptr);

	if (res != SQLITE_OK)
	{
		std::string exceptionText = "can`t open database file: " + connectionString;

		throw std::exception(exceptionText.c_str());
	}

	m_sqliteConnection = SQLiteConnection_t(pSqliteConnection);

	EnableWaitBusyDatabase();
	EnableForeignKeys();
}


void DatabaseSQLite::EnableWaitBusyDatabase()
{
	sqlite3_busy_handler(m_sqliteConnection.get(), SqliteBusyWaitHandler, this);
}


int DatabaseSQLite::SqliteBusyWaitHandler(void* /*udp*/, int /*count*/)
{
	std::this_thread::sleep_for(
		std::chrono::milliseconds(SQLITE_BUSY_WAIT_PERIOD_MS));

	return 1;
}


void DatabaseSQLite::EnableForeignKeys()
{
	ExecuteSql("PRAGMA foreign_keys = ON;");
}


void DatabaseSQLite::BeginTransaction()
{
	ExecuteSql("BEGIN TRANSACTION;");
}


void DatabaseSQLite::CommitTransaction()
{
	ExecuteSql("COMMIT TRANSACTION;");
}


void DatabaseSQLite::RollbackTransaction()
{
	ExecuteSql("ROLLBACK TRANSACTION");
}


sqlite3* DatabaseSQLite::GetSqliteConnectionPtr() const
{
	return m_sqliteConnection.get();
}


const std::string& DatabaseSQLite::GetConnectionString() const
{
	return m_connectionString;
}


SQLiteStatement_t DatabaseSQLite::CreateStatement(const std::string& sql) const
{
	sqlite3_stmt* pStatement = nullptr;
	int sqlitePrepareResult = sqlite3_prepare_v2(m_sqliteConnection.get(),
		sql.c_str(), (int)sql.length(),
		&pStatement, nullptr);

	if (sqlitePrepareResult == SQLITE_OK)
	{
		return SQLiteStatement_t(pStatement);
	}
	else
	{
		const char* pExecErrorMessage = sqlite3_errmsg(m_sqliteConnection.get());

		throw DatabaseSQLiteException(pExecErrorMessage != nullptr ? pExecErrorMessage : "",
			sqlitePrepareResult);
	}
}


void DatabaseSQLite::ExecuteSql(const std::string& sql)
{
	bool executeSuccess = false;

	std::string execErrorMessage;
	char* pExecErrorMessage = nullptr;

	int sqliteExecResult = sqlite3_exec(m_sqliteConnection.get(), sql.c_str(), 
		nullptr, nullptr, &pExecErrorMessage);

	if (sqliteExecResult == SQLITE_OK ||
		sqliteExecResult == SQLITE_DONE || sqliteExecResult == SQLITE_ROW)
	{
		executeSuccess = true;
	}

	if (pExecErrorMessage != nullptr)
	{
		execErrorMessage = pExecErrorMessage;
		sqlite3_free(pExecErrorMessage);
	}

	if (!executeSuccess)
	{
		throw DatabaseSQLiteException(execErrorMessage, sqliteExecResult);
	}
}
