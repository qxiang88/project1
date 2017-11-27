//
// Created by wangqixiang on 2017/11/14.
//

#include "crypto/crypto.h"
#include "corgi/base/errors.h"

#include "crypto/crypto_util.h"
#include "util/pipeline.h"

namespace mms {

Crypto::Crypto() {
  EVP_add_cipher(EVP_aes_128_cbc());
}

corgi::Status Crypto::GenerateKey128(std::string *result) {
  return GenerateAESKey(kAES128KeySize, result);
}

corgi::Status Crypto::GenerateIV(std::string *result) {
  return mms::GenerateIV(result);
}

corgi::Status Crypto::Encrypt(const std::string &plain_file_path,
                              const std::string &cipher_file_path,
                              const std::string &key128,
                              const std::string &iv) {
  Pipeline pipeline(plain_file_path, cipher_file_path);
  CG_RETURN_IF_ERROR(pipeline.Init());
  pipeline.AddFilter([=](const std::string &message, bool is_last_block, std::string *result) -> corgi::Status {
    CG_RETURN_IF_ERROR(AESCBCEncrypt(key128, message, result, iv, is_last_block));
    return corgi::Status::OK();
  });
  CG_RETURN_IF_ERROR(pipeline.Process());
  return corgi::Status::OK();
}

corgi::Status Crypto::Decrypt(const std::string &cipher_file_path,
                              const std::string &plain_file_path,
                              const std::string &key128,
                              const std::string &iv) {
  Pipeline pipeline(cipher_file_path, plain_file_path);
  CG_RETURN_IF_ERROR(pipeline.Init());
  pipeline.AddFilter([=](const std::string &message, bool is_last_block, std::string *result) -> corgi::Status {
    CG_RETURN_IF_ERROR(AESCBCDecrypt(key128, message, result, iv));
    return corgi::Status::OK();
  });
  CG_RETURN_IF_ERROR(pipeline.Process());
  return corgi::Status::OK();
}

} // namespace
