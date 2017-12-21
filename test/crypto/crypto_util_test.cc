//
// Created by wangqixiang on 2017/11/14.
//

#include "corgi/strings/hex_util.h"
#include "corgi/gtl/stl_util.h"

#include "crypto/crypto_util.h"

#include <gtest/gtest.h>

namespace mms {

bool createTestIV(std::string* iv) {
  corgi::gtl::STLStringResizeUninitialized(iv, kAESBlockSize);
  memset(const_cast<char*>(iv->data()), 0, kAESBlockSize);
  return iv->size() == kAESBlockSize;
}

void doCBCEncryptAndDecrypt(const std::string& plaintext) {
  std::string iv;
  EXPECT_TRUE(createTestIV(&iv));
  std::string key128;
  corgi::Status status = GenerateAESKey(kAES128KeySize, &key128);
  EXPECT_TRUE(status.ok());
  std::string cipher;
  // Cipher with padding
  status = AESCBCEncrypt(key128, plaintext, &cipher, iv, true);
  EXPECT_TRUE(status.ok());
  int expected_padding_size = 0;
  int remain = plaintext.size() % kAESBlockSize;
  if (remain == 0) {
    expected_padding_size = plaintext.size() + kAESBlockSize;
  } else {
    expected_padding_size = plaintext.size() + (kAESBlockSize - remain);
  }
  EXPECT_EQ(expected_padding_size, cipher.size());

  // Decrypt
  std::string message;
  status = AESCBCDecrypt(key128, cipher, &message, iv);
  EXPECT_TRUE(status.ok());
  EXPECT_EQ(plaintext, message);
}

TEST(CryptoUtilTest, CryptWithLessPadding) {
  const std::string plaintext("hello");
  doCBCEncryptAndDecrypt(plaintext);
}

TEST(CryptoUtilTest, CryptWithBlockSizePadding) {
  const std::string plaintext("0123456789ABCDEFG");
  doCBCEncryptAndDecrypt(plaintext);
}

TEST(CryptoUtilTest, CryptBlockByBlock) {
  std::string iv;
  createTestIV(&iv);
  std::string key128;
  corgi::Status status = GenerateAESKey(kAES128KeySize, &key128);
  EXPECT_TRUE(status.ok());
  #if 0
  std::vector<std::string> plaintexts = {
      "hello",
      "world",
  };
  #endif
}

} // namespace mms

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

