#include "stdafx.h"
#include "User.h"

bool operator==(const User& lhs, const User& rhs)
{
	return (lhs.GetLogin() == rhs.GetLogin() &&
		lhs.GetRole() == rhs.GetRole());
};


bool operator!=(const User& lhs, const User& rhs)
{
	return (lhs.GetLogin() != rhs.GetLogin() ||
		lhs.GetRole() != rhs.GetRole());
};


User::User() :
	m_login(""),
	m_role(UserRole::Unknown)
{
}


User::User(const std::string& login, const UserRole role) :
	m_login(login),
	m_role(role)
{
}


User::User(const User& other) :
	m_login(other.m_login),
	m_role(other.m_role)
{
}


User& User::operator= (const User& other)
{
	m_login = other.m_login;
	m_role = other.m_role;

	return *this;
}


const User& User::GetEmptyUser()
{
	static User emptyUser;

	return emptyUser;
}


const std::string& User::GetLogin() const
{
	return m_login;
}


const UserRole User::GetRole() const
{
	return m_role;
}
