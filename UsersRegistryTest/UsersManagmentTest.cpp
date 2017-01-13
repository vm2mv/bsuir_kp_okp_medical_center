#include "stdafx.h"
#include "../UsersRegistry/UsersRegistry.h"


class UsersManagmentTest : public ::testing::Test
{
protected:
	void SetUp()
	{
		m_usersRegistry = UsersRegistry::CreateUsersRegistry(R"(:memory:)");
	}

	void TearDown()
	{
	}


protected:
	std::shared_ptr<UsersRegistry> m_usersRegistry;
};


TEST_F(UsersManagmentTest, FindNotExistUser)
{
	ASSERT_FALSE(m_usersRegistry->IsExistUser("not_exist_user"));
}


TEST_F(UsersManagmentTest, FindExistUser)
{
	ASSERT_TRUE(m_usersRegistry->AddUser(User{ "exist_user_FindExistUser", UserRole::User }, "1"));
	ASSERT_TRUE(m_usersRegistry->IsExistUser("exist_user_FindExistUser"));
}


TEST_F(UsersManagmentTest, AddNewAdminUser)
{
	ASSERT_TRUE(m_usersRegistry->AddUser(User{ "admin", UserRole::Admin }, "1"));
	ASSERT_TRUE(m_usersRegistry->IsExistUser("admin"));
}


TEST_F(UsersManagmentTest, AddNewRegularUser)
{
	ASSERT_TRUE(m_usersRegistry->AddUser(User{ "user", UserRole::User }, "1"));
	ASSERT_TRUE(m_usersRegistry->IsExistUser("user"));
}


TEST_F(UsersManagmentTest, AddNewUserWithEmptyPassword)
{
	ASSERT_TRUE(m_usersRegistry->AddUser(User{ "user_with_empty_password", UserRole::User }, ""));
	ASSERT_TRUE(m_usersRegistry->IsExistUser("user_with_empty_password"));
}


TEST_F(UsersManagmentTest, DeleteUserByLogin)
{
	ASSERT_TRUE(m_usersRegistry->AddUser(User{ "admin_to_delete", UserRole::Admin }, "1"));
	ASSERT_TRUE(m_usersRegistry->IsExistUser("admin_to_delete"));
	ASSERT_TRUE(m_usersRegistry->DeleteUserByLogin("admin_to_delete"));
}


TEST_F(UsersManagmentTest, DeleteUserById)
{
	ASSERT_TRUE(m_usersRegistry->AddUser(User{ "admin_to_delete_by_id", UserRole::Admin }, "1"));
	ASSERT_TRUE(m_usersRegistry->IsExistUser("admin_to_delete_by_id"));
	
	UserRecord_t userRecord = m_usersRegistry->GetUserByLogin("admin_to_delete_by_id");

	ASSERT_NE(userRecord.second, User::GetEmptyUser());

	ASSERT_TRUE(m_usersRegistry->DeleteUser(userRecord.first));
}


TEST_F(UsersManagmentTest, DeleteNotExistUser)
{
	ASSERT_FALSE(m_usersRegistry->IsExistUser("not_exist_user"));
	ASSERT_FALSE(m_usersRegistry->DeleteUserByLogin("not_exist_user"));
}


TEST_F(UsersManagmentTest, AddExistUser)
{
	ASSERT_TRUE(m_usersRegistry->AddUser(User{ "exist_user_AddExistUser", UserRole::Admin }, "1"));
	ASSERT_TRUE(m_usersRegistry->IsExistUser("exist_user_AddExistUser"));
	ASSERT_FALSE(m_usersRegistry->AddUser(User{ "exist_user_AddExistUser", UserRole::Admin }, "1"));
	ASSERT_TRUE(m_usersRegistry->IsExistUser("exist_user_AddExistUser"));
}


TEST_F(UsersManagmentTest, AddUsersWithDifCase)
{
	ASSERT_TRUE(m_usersRegistry->AddUser(User{ "admin_user_AddUserWithDifCase", UserRole::Admin }, "1"));
	ASSERT_TRUE(m_usersRegistry->IsExistUser("admin_user_AddUserWithDifCase"));
	ASSERT_FALSE(m_usersRegistry->IsExistUser("Admin_user_AddUserWithDifCase"));

	ASSERT_TRUE(m_usersRegistry->AddUser(User{ "Admin_user_AddUserWithDifCase", UserRole::Admin }, "1"));
	ASSERT_TRUE(m_usersRegistry->IsExistUser("Admin_user_AddUserWithDifCase"));

	ASSERT_TRUE(m_usersRegistry->AddUser(User{ "админ_user_AddUserWithDifCase", UserRole::Admin }, "1"));
	ASSERT_TRUE(m_usersRegistry->IsExistUser("админ_user_AddUserWithDifCase"));
	ASSERT_FALSE(m_usersRegistry->IsExistUser("Админ_user_AddUserWithDifCase"));

	ASSERT_TRUE(m_usersRegistry->AddUser(User{ "Админ_user_AddUserWithDifCase", UserRole::Admin }, "1"));
	ASSERT_TRUE(m_usersRegistry->IsExistUser("Админ_user_AddUserWithDifCase"));
}


TEST_F(UsersManagmentTest, UpdateUser)
{
	ASSERT_TRUE(m_usersRegistry->AddUser(User{ "update_user_1", UserRole::User }, "1"));
	ASSERT_TRUE(m_usersRegistry->IsExistUser("update_user_1"));

	UserRecord_t userRecordUpdate1 = m_usersRegistry->GetUserByLogin("update_user_1");
	const User& userUpdate1(userRecordUpdate1.second);
	ASSERT_EQ(userUpdate1.GetLogin(), "update_user_1");
	ASSERT_EQ(userUpdate1.GetRole(), UserRole::User);

	User newUserUpdate2("update_user_2", UserRole::Admin);
	ASSERT_TRUE(m_usersRegistry->UpdateUser(userRecordUpdate1.first, newUserUpdate2));

	UserRecord_t userRecordUpdate2 = m_usersRegistry->GetUserByLogin("update_user_2");
	const User& userUpdate2(userRecordUpdate2.second);
	ASSERT_EQ(userUpdate2.GetLogin(), "update_user_2");
	ASSERT_EQ(userUpdate2.GetRole(), UserRole::Admin);
}


TEST_F(UsersManagmentTest, UpdateUserWithPassword)
{
	ASSERT_TRUE(m_usersRegistry->AddUser(User{ "update_user_with_password_1", UserRole::User }, "1"));
	ASSERT_TRUE(m_usersRegistry->IsExistUser("update_user_with_password_1"));

	UserRecord_t userRecordUpdate1 = m_usersRegistry->GetUserByLogin("update_user_with_password_1");
	const User& userUpdate1(userRecordUpdate1.second);
	ASSERT_EQ(userUpdate1.GetLogin(), "update_user_with_password_1");
	ASSERT_EQ(userUpdate1.GetRole(), UserRole::User);

	User newUserUpdate2("update_user_with_password_2", UserRole::Admin);
	ASSERT_TRUE(m_usersRegistry->UpdateUserAndPassword(userRecordUpdate1.first, newUserUpdate2, "2"));

	UserRecord_t userRecordUpdate2 = m_usersRegistry->GetUserByLogin("update_user_with_password_2");
	const User& userUpdate2(userRecordUpdate2.second);
	ASSERT_EQ(userUpdate2.GetLogin(), "update_user_with_password_2");
	ASSERT_EQ(userUpdate2.GetRole(), UserRole::Admin);

	ASSERT_FALSE(m_usersRegistry->CheckUserPassword("update_user_with_password_2", "1"));
	ASSERT_TRUE(m_usersRegistry->CheckUserPassword("update_user_with_password_2", "2"));
}
