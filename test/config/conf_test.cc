//
// Created by wangqixiang on 2017/11/17.
//

#include "config/conf.h"

#include <gtest/gtest.h>


namespace mms {

namespace {

const std::string testGoodConfPath = "/tmp/conf_d.json";
const std::string testBadConfPath = "/tmp/not_exist_file.json";

} // namespace

TEST(ConfTest, Init) {
  corgi::Status status = Conf::Default()->Init(testBadConfPath);
//  LOG(INFO) << status.ToString();
  EXPECT_FALSE(status.ok());
  status = Conf::Default()->Init(testGoodConfPath);
//  LOG(INFO) << status.ToString();
  EXPECT_TRUE(status.ok());
}

TEST(ConfTest, Get) {
  corgi::Status status = Conf::Default()->Init(testGoodConfPath);
  EXPECT_TRUE(status.ok());
  LOG(INFO) << Conf::Default()->GetListenAddress();
  LOG(INFO) << Conf::Default()->GetPort();
  LOG(INFO) << Conf::Default()->GetDBPath();
  LOG(INFO) << Conf::Default()->GetMediaServerPrefix();
  LOG(INFO) << Conf::Default()->GetKeyServerPrefix();
  LOG(INFO) << Conf::Default()->GetLDAudioSample();
  LOG(INFO) << Conf::Default()->GetLDVideoSize();
  LOG(INFO) << Conf::Default()->GetLDVideoAspect();
  LOG(INFO) << Conf::Default()->GetLDVideoBitrate();
  LOG(INFO) << Conf::Default()->GetLDVideoFps();
}

} // namespace mms

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

