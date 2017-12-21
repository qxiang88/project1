//
// Created by wangqixiang on 2017/11/15.
//

#include "crypto/crypto.h"
#include "crypto/crypto_util.h"

#include "corgi/base/errors.h"

#include <gtest/gtest.h>

namespace mms {

TEST(CryptoTest, EncryptoAndDecrypt) {
  std::string iv;
  std::string key128;
  corgi::Status status;

  status = GenerateIV(&iv);
  EXPECT_TRUE(status.ok());
  status = Crypto::Default()->GenerateKey128(&key128);
  EXPECT_TRUE(status.ok());

  const std::string plaintext_path("/tmp/plaintext.txt");
  const std::string cipher_path("/tmp/ciphertext.txt");
  const std::string message_path("/tmp/message.txt");

  status = Crypto::Default()->Encrypt(plaintext_path, cipher_path, key128, iv);
  EXPECT_TRUE(status.ok());
  status = Crypto::Default()->Decrypt(cipher_path, message_path, key128, iv);
  EXPECT_TRUE(status.ok());
}

}
int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}