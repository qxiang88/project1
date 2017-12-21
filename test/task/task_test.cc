//
// Created by wangqixiang on 2017/11/14.
//

#include "task/task.h"
#include <gtest/gtest.h>

namespace mms {

TEST(TaskTest, Ctor) {
  #if 0
  Task task(TaskID::Create());
  EXPECT_EQ(kNew, task.state());
  EXPECT_EQ(nullptr, task.handler());
  LOG(INFO) << "task_id: " << task.id().ToString();
  #endif
}

} // namespace mms

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
