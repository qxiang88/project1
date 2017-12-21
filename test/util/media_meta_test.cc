//
// Created by wangqixiang on 2017/12/20.
//

#include "util/media_meta.h"

#include <gtest/gtest.h>

namespace mms {

namespace {

const std::string kTestMediaPath = "/home/xdesk/Videos/t.mp4";

} // namespace


TEST(MediaMetaTest, TestInit) {
  corgi::Status status;
  MediaMeta media_meta(kTestMediaPath);
  status = media_meta.Init();
  EXPECT_TRUE(status.ok());
  LOG(INFO) << status.ToString();

  LOG(INFO) << "Duration: " << media_meta.GetDuration();
  LOG(INFO) << "Size: " << media_meta.GetSize();
  LOG(INFO) << "Width: " << media_meta.GetVideoWidth();
  LOG(INFO) << "Height: " << media_meta.GetVideoHeight();
  LOG(INFO) << "W/H: " << media_meta.GetVideoAspect();
  LOG(INFO) << "W/H: " << media_meta.GetVideoAspectAsString();
}

} // namespace mms

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
