#pragma once


class DatabaseSQLiteException : public std::exception
{
public:
	explicit DatabaseSQLiteException(const std::string& msg, int code);

	const char* what() const throw() override;

	const std::string& GetMessage() const;
	int GetCode() const;

private:
	std::string m_msg;
	int m_code;
};
