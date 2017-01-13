#pragma once

#include "SQLiteStatementType.h"


class DatabaseSQLite
{
	static const int SQLITE_BUSY_WAIT_PERIOD_MS;

	struct SQLiteConnectionDeleter
	{
		void operator()(sqlite3* p)
		{
			int res = sqlite3_close_v2(p);

			if (res != SQLITE_OK)
			{
				res = res;
			}
		}
	};

	static int SqliteBusyWaitHandler(void* ud, int count);

public:
	using SQLiteConnection_t = std::unique_ptr < sqlite3, SQLiteConnectionDeleter >;

public:
	explicit DatabaseSQLite(const std::string& connectionString, bool createIfNotExist, bool readOnly);

	const std::string& GetConnectionString() const;

	SQLiteStatement_t CreateStatement(const std::string& sql) const;
	void ExecuteSql(const std::string& sql);

	void BeginTransaction();
	void CommitTransaction();
	void RollbackTransaction();

	sqlite3* GetSqliteConnectionPtr() const;

private:
	void EnableWaitBusyDatabase();
	void EnableForeignKeys();

private:
	const std::string m_connectionString;
	SQLiteConnection_t m_sqliteConnection;
};

