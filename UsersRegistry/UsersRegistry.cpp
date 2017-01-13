#include "stdafx.h"
#include "UsersRegistry.h"
#include "UsersRegistryImpl.h"


UsersRegistry::UsersRegistry(const std::string& connectionString, bool createIfNotExist, bool readOnly) :
	m_usersRegistryImpl(std::make_unique<UsersRegistryImpl>(connectionString, createIfNotExist, readOnly))
{
}


std::shared_ptr<UsersRegistry> UsersRegistry::CreateUsersRegistry(const std::string& connectionString)
{
	return std::shared_ptr<UsersRegistry>(new UsersRegistry(connectionString, true, false));
}


std::shared_ptr<UsersRegistry> UsersRegistry::OpenUsersRegistry(const std::string& connectionString, bool readOnly)
{
	return std::shared_ptr<UsersRegistry>(new UsersRegistry(connectionString, false, readOnly));
}


uint64_t UsersRegistry::GetAllUsersCount() const
{
	return m_usersRegistryImpl->GetAllUsersCount();
}


uint64_t UsersRegistry::GetAdminUsersCount() const
{
	return m_usersRegistryImpl->GetAdminUsersCount();
}


const std::vector<UserRecord_t> UsersRegistry::GetAllUsers() const
{
	return m_usersRegistryImpl->GetAllUsers();
}


bool UsersRegistry::AddUser(const User& user, const std::string& password)
{
	return m_usersRegistryImpl->AddUser(user, password);
}


bool UsersRegistry::DeleteUser(const UserId_t userId)
{
	return m_usersRegistryImpl->DeleteUser(userId);
}


bool UsersRegistry::DeleteUserByLogin(const std::string& login)
{
	return m_usersRegistryImpl->DeleteUserByLogin(login);
}


bool UsersRegistry::UpdateUser(const UserId_t userId, const User& user)
{
	return m_usersRegistryImpl->UpdateUser(userId, user);
}


bool UsersRegistry::UpdateUserAndPassword(const UserId_t userId, const User& user, const std::string& password)
{
	return m_usersRegistryImpl->UpdateUserAndPassword(userId, user, password);
}


bool UsersRegistry::IsExistUser(const std::string& login) const
{
	return m_usersRegistryImpl->IsExistUser(login);
}


UserRecord_t UsersRegistry::GetUserByLogin(const std::string& login) const
{
	return m_usersRegistryImpl->GetUserByLogin(login);
}


UserRecord_t UsersRegistry::GetUserById(const UserId_t userId) const
{
	return m_usersRegistryImpl->GetUserById(userId);
}


bool UsersRegistry::CheckUserPassword(const std::string& login, const std::string& password) const
{
	return m_usersRegistryImpl->CheckUserPassword(login, password);
}
