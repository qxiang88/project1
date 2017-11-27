//
// Created by wangqixiang on 2017/11/14.
//

#ifndef MEDIA_MANAGE_SYSTEM_CRYPTO_UTIL_H
#define MEDIA_MANAGE_SYSTEM_CRYPTO_UTIL_H

#include <openssl/evp.h>

#include "corgi/base/status.h"

namespace mms {

enum {
  kAES128KeySize = 16,
  kAES256KeySize = 32,
  kAESBlockSize = 16
};

bool RandBytes(unsigned char* buf, size_t size);

corgi::Status GenerateAESKey(int key_size, std::string *result);
corgi::Status GenerateIV(std::string* result);

corgi::Status AESCBCEncrypt(const std::string &key,
                            const corgi::Slice &input,
                            std::string *output,
                            const std::string &iv,
                            bool do_padding = true);

corgi::Status AESCBCDecrypt(const std::string &key,
                            const corgi::Slice &input,
                            std::string *output,
                            const std::string &iv);

} // namespace mms
#endif //MEDIA_MANAGE_SYSTEM_CRYPTO_UTIL_H
