//
// Created by wangqixiang on 2017/12/2.
//

#include "util/directory_sizer.h"
#include "corgi/env/env.h"
#include "gtest/gtest.h"

namespace mms {

TEST(DirectorySizerTest, TestComputeSize) {
  const std::string kTestDirectory = "/tmp/test_directory_sizer";
  const std::string kSubTestDirectory = kTestDirectory + "/sub";

  corgi::Status s;
  s = corgi::Env::Default()->RecursivelyCreateDir(kTestDirectory);
  EXPECT_TRUE(s.ok());
  s = corgi::Env::Default()->RecursivelyCreateDir(kSubTestDirectory);
  EXPECT_TRUE(s.ok());

  s = corgi::WriteStringToFile(corgi::Env::Default(), kTestDirectory+"/a.ts", "1");
  EXPECT_TRUE(s.ok());
  s = corgi::WriteStringToFile(corgi::Env::Default(), kSubTestDirectory+"/a.ts", "1");
  EXPECT_TRUE(s.ok());
  s = corgi::WriteStringToFile(corgi::Env::Default(), kTestDirectory+"/b.mp4", "1");
  EXPECT_TRUE(s.ok());

  uint64_t total_size;
  DirectorySizer sizer(kTestDirectory);
  s = sizer.ComputeSize(&total_size);
  EXPECT_TRUE(s.ok());
  EXPECT_EQ(2, total_size);

  DirectorySizer sizer1(kTestDirectory, "*.ts");
  s = sizer1.ComputeSize(&total_size);
  EXPECT_TRUE(s.ok());
  EXPECT_EQ(1, total_size);

  s = sizer1.ComputeSizeRecursively(&total_size);
  EXPECT_TRUE(s.ok());
  EXPECT_EQ(2, total_size);

  corgi::int64 n1, n2;
  corgi::Env::Default()->DeleteRecursively(kTestDirectory, &n1, &n2);
  EXPECT_TRUE(s.ok());
}

} // namespace mms

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
