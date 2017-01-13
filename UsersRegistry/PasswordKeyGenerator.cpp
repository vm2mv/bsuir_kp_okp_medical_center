#include "stdafx.h"
#include "PasswordKeyGenerator.h"


const std::string PasswordKeyGenerator::PBKDF2_SALT = "N}i82s7kU+W^mcS7hI9ec|0z9A4X+f";
const uint32_t PasswordKeyGenerator::PBKDF2_ITERATIONS_COUNT = 64000;
const uint32_t PasswordKeyGenerator::PBKDF2_KEY_SIZE = 64;


std::string PasswordKeyGenerator::GenerateKey_PBKDF2_HMAC(const std::string& password)
{
	std::vector<uint8_t> res(PBKDF2_KEY_SIZE);

	if (PKCS5_PBKDF2_HMAC(
		password.data(), (int)password.size(),
		(const unsigned char*)PBKDF2_SALT.data(), (int)PBKDF2_SALT.size(),
		PBKDF2_ITERATIONS_COUNT,
		EVP_sha512(),
		PBKDF2_KEY_SIZE,
		res.data()) == 0)
	{
		throw std::exception("PKCS5_PBKDF2_HMAC error");
	}

	BIO* b64 = BIO_new(BIO_f_base64());

	if (b64 == nullptr)
	{
		throw std::bad_alloc();
	}

	BIO* bmem = BIO_new(BIO_s_mem());

	if (bmem == nullptr)
	{
		BIO_free_all(b64);

		throw std::bad_alloc();
	}

	BIO_push(b64, bmem);
	BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);

	BIO_write(b64, res.data(), (int)res.size());

	BIO_flush(b64);

	int len = BIO_pending(bmem);

	std::vector<char> buf(len + 1);

	len = BIO_read(bmem, buf.data(), len);
	buf[len] = '\0';

	BIO_free_all(b64);

	return std::string(buf.data());
}
