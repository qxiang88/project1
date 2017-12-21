//
// Created by wangqixiang on 2017/11/16.
//


#include "config/conf.h"
#include "task/task_manager.h"
#include "util/guid.h"
#include <gtest/gtest.h>

namespace mms {

TEST(TaskManagerTest, ScheduleTask) {
  TaskManager task_manager("/tmp/conf_d.json");
  corgi::Status status = task_manager.Init();
  std::this_thread::sleep_for(std::chrono::seconds(1));
  EXPECT_TRUE(status.ok());
  TaskMetadata metadata;
  metadata.SetUUID(GenerateGUID()),
  metadata.SetMediaSourcePath("/tmp/test.mp4");
  metadata.SetTargetFormat("hls");
  std::unique_ptr<Task> task;
  status = task_manager.NewTask(metadata, &task);
  EXPECT_TRUE(status.ok());

  // task.release() 释放所有权给 TaskManager.
  // 要在Schedule之前先取出 uuid.
  std::string uuid = metadata.uuid();
  status = task_manager.ScheduleTask(task.release());

  int counter = 7;
  TaskMetaPB task_meta_db_entry;
  while (true && counter) {
    status = task_manager.RetrieveTaskMeta(uuid, &task_meta_db_entry);
    if (status.ok()) {
      LOG(INFO) << task_meta_db_entry.DebugString();
    }
    HLSMetaPB hls_meta;
    if (task_meta_db_entry.state_int() == kTranscodeAndEncryptDone) {
      status = task_manager.RetrieveHLSMeta(uuid, &hls_meta);
      EXPECT_TRUE(status.ok());
      LOG(INFO) << hls_meta.DebugString();
    }
    if (task_meta_db_entry.state_int() == kCopyDone) {
      status = task_manager.RetrieveHLSMeta(uuid, &hls_meta);
      EXPECT_TRUE(status.ok());
      LOG(INFO) << "Check GFS m3u8 path: " << hls_meta.gfs_m3u8_path();
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));
    counter--;
  }
}

} // namespace mms

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

