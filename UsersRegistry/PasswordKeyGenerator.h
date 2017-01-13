#pragma once


class PasswordKeyGenerator
{
	static const std::string PBKDF2_SALT;
	static const uint32_t PBKDF2_ITERATIONS_COUNT;
	static const uint32_t PBKDF2_KEY_SIZE;

public:
	static std::string GenerateKey_PBKDF2_HMAC(const std::string& password);
};

