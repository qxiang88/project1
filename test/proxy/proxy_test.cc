//
// Created by wangqixiang on 2017/11/18.
//

#include "proxy/service_proxy.h"
#include <gtest/gtest.h>

namespace mms {

namespace {
const std::string testConfPath="/tmp/conf_d.json";
const std::string testMediaSourcePath = "/tmp/test.mp4";
} // namespace

TEST(ServiceProxyTest, Handles) {
  corgi::Status status = Conf::Default()->Init(testConfPath);
  EXPECT_TRUE(status.ok());

  ServiceProxy proxy(Conf::Default());

  status = proxy.Init();
  EXPECT_TRUE(status.ok());

  TranscodeRequestPB request;
  TranscodeResponsePB response;
  request.set_media_source_path(testMediaSourcePath);
  request.set_media_target_format("hls");

  status = proxy.HandleTranscode(&request, &response);
  LOG(INFO) << status.ToString();
  EXPECT_TRUE(status.ok());
}

} // namespace mms

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}


