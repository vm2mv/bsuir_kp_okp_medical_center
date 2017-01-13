#pragma once

#include "UserRole.h"


class User
{
public:
	User();
	explicit User(const std::string& login, const UserRole role);
	
	User(const User&);
	User& operator= (const User&);

	static const User& GetEmptyUser();

	const std::string& GetLogin() const;
	const UserRole GetRole() const;

private:
	std::string m_login;
	UserRole m_role;
};


using UserId_t = uint64_t;
using UserRecord_t = std::pair<UserId_t, User>;

static const UserId_t USERID_EMPTY = 0;

bool operator==(const User& lhs, const User& rhs);
bool operator!=(const User& lhs, const User& rhs);
