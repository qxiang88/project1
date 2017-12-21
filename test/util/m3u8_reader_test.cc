//
// Created by wangqixiang on 2017/11/22.
//

#include "util/m3u8_reader.h"
#include "corgi/base/errors.h"

#include <gtest/gtest.h>

namespace mms {

TEST(M3U8ReaderTest, ReadLine) {
  const std::string test_m3u8_path = "/tmp/test.m3u8";
  M3U8Options options;
  options.m3u8_path_ = test_m3u8_path;
  std::unique_ptr<M3U8Reader> reader;
  corgi::Status s = M3U8Reader::Create(options, &reader);
  EXPECT_TRUE(s.ok());
  std::string line;
  while (true) {
    s = reader->ReadLine(&line);
    if (corgi::errors::IsOutOfRange(s)) {
      break;
    }

    if (!s.ok()) {
      // ERROR
      break;
    }

    LOG(INFO) << line;
  }
}

} // namespace mms

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
