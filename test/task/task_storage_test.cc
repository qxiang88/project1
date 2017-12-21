//
// Created by wangqixiang on 2017/11/20.
//

#include "task/task_metadata.h"
#include "task/task_storage.h"
#include <gtest/gtest.h>

namespace mms {

TEST(TaskStorageTest, TaskMetadata) {
  corgi::Status status;
  std::unique_ptr<Database> database;
  std::unique_ptr<DBKeyGenerator> key_generator;
  status = NewDatabase("leveldb", &database);
  EXPECT_TRUE(status.ok());
  status = database->Open("/tmp/test_db");
  EXPECT_TRUE(status.ok());
  status = NewDBKeyGenerator("leveldb", &key_generator);
  EXPECT_TRUE(status.ok());

  TaskStorage storage(key_generator.get(),
                      database.get());
  // Store
  TaskMetaPB meta_pb;
  meta_pb.set_media_source_path("/path/to/source");
  meta_pb.set_uuid("uuid123");
  meta_pb.set_target_format("hls");
  meta_pb.set_state_int(kNew);
  meta_pb.set_state_str(TaskStateToString(kNew));
  status = storage.StoreTaskMetadata("uuid123", &meta_pb);
  EXPECT_TRUE(status.ok());

  // Retrieve
  TaskMetaPB meta_pb1;
  status = storage.RetrieveTaskMetadata("uuid123", &meta_pb1);
  EXPECT_TRUE(status.ok());

  EXPECT_EQ(meta_pb.uuid(), meta_pb1.uuid());
  EXPECT_EQ(meta_pb.media_source_path(), meta_pb1.media_source_path());
  EXPECT_EQ(meta_pb.state_int(), meta_pb1.state_int());
  EXPECT_EQ(meta_pb.state_str(), meta_pb1.state_str());
  EXPECT_EQ(meta_pb.target_format(), meta_pb1.target_format());
}


} // namespace mms

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
