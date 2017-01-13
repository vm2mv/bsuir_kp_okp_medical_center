#include "stdafx.h"
#include "../UsersRegistry/UsersRegistry.h"


class UsersAuthTest : public ::testing::Test
{
protected:
	void SetUp()
	{
		m_usersRegistry = UsersRegistry::CreateUsersRegistry(R"(:memory:)");
	}

	void TearDown()
	{
	}

	std::shared_ptr<UsersRegistry> m_usersRegistry;
};


TEST_F(UsersAuthTest, AuthNotExistUser)
{
	ASSERT_FALSE(m_usersRegistry->IsExistUser("not_exist_user"));
	ASSERT_FALSE(m_usersRegistry->CheckUserPassword("not_exist_user", "1"));
}


TEST_F(UsersAuthTest, AuthUserWithInvalidPassword)
{
	ASSERT_TRUE(m_usersRegistry->AddUser(User{ "user_for_invalid_password", UserRole::User }, "1"));
	ASSERT_TRUE(m_usersRegistry->IsExistUser("user_for_invalid_password"));
	ASSERT_FALSE(m_usersRegistry->CheckUserPassword("user_for_invalid_password", "123"));
}


TEST_F(UsersAuthTest, AuthUserWithEmptyPassword)
{
	ASSERT_TRUE(m_usersRegistry->AddUser(User{ "user_with_empty_password", UserRole::User }, ""));
	ASSERT_TRUE(m_usersRegistry->IsExistUser("user_with_empty_password"));

	ASSERT_TRUE(m_usersRegistry->CheckUserPassword("user_with_empty_password", ""));
}


TEST_F(UsersAuthTest, AuthUserWithNormalPassword)
{
	ASSERT_TRUE(m_usersRegistry->AddUser(User{ "user_with_normal_password", UserRole::User }, "12345678"));
	ASSERT_TRUE(m_usersRegistry->IsExistUser("user_with_normal_password"));

	ASSERT_TRUE(m_usersRegistry->CheckUserPassword("user_with_normal_password", "12345678"));
}



TEST_F(UsersAuthTest, AuthUserWithLongPassword)
{
	ASSERT_TRUE(m_usersRegistry->AddUser(User{ "user_with_long_password", UserRole::User },
		"1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"));
	ASSERT_TRUE(m_usersRegistry->IsExistUser("user_with_long_password"));

	ASSERT_TRUE(m_usersRegistry->CheckUserPassword("user_with_long_password",
		"1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"));
	
	ASSERT_FALSE(m_usersRegistry->CheckUserPassword("user_with_long_password",
		"12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901"));

	ASSERT_FALSE(m_usersRegistry->CheckUserPassword("user_with_long_password",
		"12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"));
}
