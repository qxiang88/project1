//
// Created by wangqixiang on 2017/11/17.
//

#include "crypto/key_info_file.h"
#include "corgi/env/env.h"

#include <gtest/gtest.h>

namespace mms {

namespace {

const std::string testExistFile = "/tmp/enc.keyinfo";
const std::string testNotExistFile = "/tmp/not_exist";

} // namespace

TEST(KeyInfoFileTest, Load) {
  std::unique_ptr<KeyInfoFile> key_file;
  corgi::Status status = KeyInfoFile::Load(testExistFile, &key_file);
  EXPECT_TRUE(status.ok());
  LOG(INFO) << key_file->media_key_url();
  LOG(INFO) << key_file->media_key_path();
  LOG(INFO) << key_file->media_key_iv();
  status = KeyInfoFile::Load(testNotExistFile, &key_file);
  EXPECT_FALSE(status.ok());
  LOG(INFO) << "Status: " << status.ToString();
}

TEST(KeyInfoFileTest, Write) {
  const std::string testFileName = "/tmp/test_keyinfofile";
  KeyInfoFile key_info_file(testFileName);
  corgi::Status status = key_info_file.Write();
  EXPECT_FALSE(status.ok());
  LOG(INFO) << status.ToString();
  key_info_file.set_media_key_url("https://localhost:8080/enc.key?token=111111111");
  key_info_file.set_media_key_path("/tmp/my.key");
  key_info_file.set_media_key_iv("000000000");
  status = key_info_file.Write();
  EXPECT_TRUE(status.ok());

  key_info_file.set_media_key_iv("1111111111");
  status = key_info_file.Write();
  EXPECT_FALSE(status.ok());
  status = key_info_file.Write(true);
  EXPECT_TRUE(status.ok());
  status = corgi::Env::Default()->DeleteFile(testFileName);
  EXPECT_TRUE(status.ok());
}

// Case 1 使用已经存在的 Key
// std::unique_ptr<KeyInfoFile> key_
// Case 2 创建新的 Key
} // namespace mms

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
