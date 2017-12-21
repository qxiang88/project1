//
// Created by wangqixiang on 2017/11/20.
//

#include "util/hls_key_access_url.h"
#include <gtest/gtest.h>

namespace mms {

TEST(HLSKeyAccessURLTest, CoDec) {
  std::string server = "http://key.fm.net/key/v1";
  std::string uuid = "123-uuid";
  std::string token = "123token";
  corgi::Status s;
  std::string url;
  s = HLSKeyAccessURL::Encode(server, uuid, token, &url);
  EXPECT_TRUE(s.ok());
  LOG(INFO) << "encoded url: " << url;

  std::string en_token = "MTIzdG9rZW4%3D";
  std::string token1;
  s = HLSKeyAccessURL::Decode(uuid, en_token, &token1);
  EXPECT_TRUE(s.ok());
  EXPECT_EQ(token, token1);
}

} // namespace mms

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}