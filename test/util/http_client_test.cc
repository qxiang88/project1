//
// Created by wangqixiang on 2017/11/23.
//

#include "util/http_client.h"

#include <gtest/gtest.h>

namespace mms {

TEST(HttpClientTest, Get) {
  HttpClientOptions options;
  options.url = "http://172.16.2.134:8080/api/isli_storage_system/hls/notifier/receiver";
  std::unique_ptr<HttpClient> http_client;
  corgi::Status s = HttpClient::Create(options, &http_client);
  LOG(INFO) << s.ToString();
  EXPECT_TRUE(s.ok());
  std::string result;
  s = http_client->POST("data", &result);
  LOG(INFO) << s.ToString();
  // LOG(INFO) << result;
}

} // namespace mms

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
