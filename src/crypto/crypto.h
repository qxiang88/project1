//
// Created by wangqixiang on 2017/11/14.
//

#ifndef MEDIA_MANAGE_SYSTEM_CRYPTO_H
#define MEDIA_MANAGE_SYSTEM_CRYPTO_H

#include <string>
#include <memory>

#include "corgi/base/status.h"


namespace mms {

class Crypto {
 public:
  static Crypto *Default() {
    static Crypto crypto_;
    return &crypto_;
  }

  corgi::Status GenerateKey128(std::string *result);
  corgi::Status GenerateIV(std::string* result);

  corgi::Status Encrypt(const std::string &plain_file_path,
                        const std::string &cipher_file_path,
                        const std::string &key128,
                        const std::string &iv);

  corgi::Status Decrypt(const std::string &cipher_file_path,
                        const std::string &plain_file_path,
                        const std::string &key128,
                        const std::string &iv);

 private:
  Crypto();
  CG_DISALLOW_COPY_AND_ASSIGN(Crypto);
};

} // namespace mms
#endif //MEDIA_MANAGE_SYSTEM_CRYPTO_H
