//
// Created by wangqixiang on 2017/11/19.
//

#include "filesystem/filesystem.h"
#include "config/conf.h"
#include "util/guid.h"

#include <gtest/gtest.h>

namespace mms {

TEST(FilesystemTest, Get) {
  corgi::Status status = Conf::Default()->Init("/tmp/conf_d.json");
  EXPECT_TRUE(status.ok());
  FilesystemOptions options;
  options.local_workspace_root = Conf::Default()->GetFilesystemLocalWorkspace();
  options.gfs_mount_point_root = Conf::Default()->GetFilesystemGFSMountPoint();
  status = Filesystem::Default()->Init(options);
  EXPECT_TRUE(status.ok());
  EXPECT_TRUE(status.ok());

  std::string uuid = GenerateGUID();
  LOG(INFO) << "LocalMediaRoot: " << Filesystem::Default()->GetLocalMediaRoot(uuid);
  LOG(INFO) << "GFSMediaRoot: " << Filesystem::Default()->GetGFSMediaRoot(uuid);
  LOG(INFO) << "LocalMediaKeyPath: " << Filesystem::Default()->GetLocalMediaKeyPath(uuid);
  LOG(INFO) << "LocalMediaKeyInfoFilePath: " << Filesystem::Default()->GetLocalMediaKeyInfoFilePath(uuid);
  LOG(INFO) << "LocalMediaM3U8Path: " << Filesystem::Default()->GetLocalMediaM3U8Path(uuid);
}

} // namespace mms

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
