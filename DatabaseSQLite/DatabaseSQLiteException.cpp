#include "stdafx.h"
#include "DatabaseSQLiteException.h"


DatabaseSQLiteException::DatabaseSQLiteException(const std::string& msg, int code) :
	m_msg(msg),
	m_code(code)
{
}


const char* DatabaseSQLiteException::what() const throw()
{
	return m_msg.c_str();
}


const std::string& DatabaseSQLiteException::GetMessage() const
{
	return m_msg;
}


int DatabaseSQLiteException::GetCode() const
{
	return m_code;
}
