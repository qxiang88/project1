//
// Created by wangqixiang on 2017/11/14.
//

#include <gtest/gtest.h>

#include "transcode/transcoder.h"
#include "corgi/strings/str_util.h"

namespace mms {

TEST(TranscoderTest, BuildCommandOk) {
  corgi::Status status;
  Transcoder transcoder;
  InputOptions in_options;
  in_options.input_file = "/tmp/test_target.mp4";

  OutputOptions out_options;
  out_options.output_file = "/tmp/hls/file_seg%d.ts";
  out_options.key_info_path = "/tmp/enc.keyinfo";
  out_options.m3u8_path = "/tmp/my_index.m3u8";

  std::vector<std::string> command;
  status = transcoder.BuildCommand(in_options, out_options, &command);
  EXPECT_TRUE(status.ok());
  std::string str =
      corgi::str_util::Join(command, " ");
  LOG(INFO) << "Command: " << str;
}

}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}