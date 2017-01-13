#pragma once

struct SQLiteStatementDeleter
{
	void operator()(sqlite3_stmt* stmt)
	{
		sqlite3_finalize(stmt);
	}
};

using SQLiteStatement_t = std::unique_ptr < sqlite3_stmt, SQLiteStatementDeleter > ;
