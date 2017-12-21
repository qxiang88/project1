//
// Created by wangqixiang on 2017/11/14.
//

#include "crypto/crypto.h"
#include "corgi/strings/base64.h"
#include "corgi/strings/hex_util.h"
#include <gtest/gtest.h>

namespace mms {

TEST(CryptoTest, GenerateKey) {
  std::string key;
  corgi::Status status = Crypto::Default()->GenerateKey128(&key);
  EXPECT_TRUE(status.ok());
  LOG(INFO) << "key: " << corgi::HexEncode(key);
}

} // namespace mms

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
