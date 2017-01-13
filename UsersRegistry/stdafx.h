#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers


// stl
#include <string>
#include <vector>
#include <memory>
#include <thread>
#include <mutex>
#include <map>
#include <algorithm>


// sqlite
#include <sqlite3.h>


// openssl
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/hmac.h>
#include <openssl/buffer.h>
