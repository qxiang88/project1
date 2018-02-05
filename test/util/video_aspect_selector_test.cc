//
// Created by wangqixiang on 2018/2/5.
//

#include "util/video_aspect_selector.h"
#include "corgi/base/logging.h"

#include <gtest/gtest.h>

namespace mms {

namespace {

bool ExactlyMatch(const std::string& width, const std::string& height) {
  VideoAspectSelector aspect_selector;
  VideoAspectSelector::AspectResolutionPair r;
  r = aspect_selector.GetLDAspectResolutionByNearlyMatch(width, height);
  LOG(INFO) << "aspect: " << r.first << ", size: " << r.second;
  return r.second == (width+"x"+height);
}

bool NearlyMatch(const std::string& width, const std::string& height) {
  VideoAspectSelector aspect_selector;
  VideoAspectSelector::AspectResolutionPair r;
  r = aspect_selector.GetLDAspectResolutionByNearlyMatch(width, height);
  LOG(INFO) << "aspect: " << r.first << ", size: " << r.second;
  return true;
}

} // namespace

TEST(VideoAspectSelectorTest, LDSelectTest) {
  EXPECT_TRUE(ExactlyMatch("320", "240"));
  EXPECT_TRUE(ExactlyMatch("320", "200"));
  EXPECT_TRUE(ExactlyMatch("480", "272"));
  EXPECT_TRUE(ExactlyMatch("480", "320"));
}

TEST(VideoAspectSelectorTest, LDSelectNearlyTest) {
  EXPECT_TRUE(NearlyMatch("320", "241"));
}

} // namespace mms

