//
// Created by wangqixiang on 2017/11/15.
//



#include "transcode/transcode_manager.h"

#include <gtest/gtest.h>

namespace mms {

TEST(TranscodeManageTest, StartAndStop) {
  std::unique_ptr<TranscodeManager> tm;
  TranscodeManagerOptions options = TranscodeManagerOptions::Default();
  options.enable_hd_task = true;
  options.enable_nd_task = true;
  corgi::Status status = TranscodeManager::Create(options, &tm);
  EXPECT_TRUE(status.ok());
  Task task("ld:id:12344");
  status = tm->SubmitToLowDefinitionQueue(&task);
  EXPECT_TRUE(status.ok());
  status = tm->SubmitToHighDefinitionQueue(&task);
  EXPECT_TRUE(status.ok());
  status = tm->SubmitToNormalDefinitionQueue(&task);
  EXPECT_TRUE(status.ok());

  std::this_thread::sleep_for(std::chrono::seconds(5));
}

} // namespace mms

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}