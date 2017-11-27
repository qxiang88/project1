//
// Created by wangqixiang on 2017/11/18.
//

#ifndef MEDIA_MANAGE_SYSTEM_COMMAND_BUILDER_H
#define MEDIA_MANAGE_SYSTEM_COMMAND_BUILDER_H

#include "config/conf.h"
#include "task/command.h"
#include "crypto/key_info_file.h"

namespace mms {

class OutputFormatter {
 public:
  OutputFormatter() {}
  virtual ~OutputFormatter();
  virtual std::string FormatOutput() = 0;
  virtual OutputFormatter* Clone() = 0;
 private:
  CG_DISALLOW_COPY_AND_ASSIGN(OutputFormatter);
};

class HLSOutputFormatter : public OutputFormatter {
 public:
  HLSOutputFormatter(
      const std::string& key_info_file_path,
      const std::string& target_file,
      const std::string& m3u8_path,
      const std::string& hls_base_url);
  virtual std::string FormatOutput() override;
  virtual OutputFormatter* Clone() override;

 private:
  std::string key_info_file_path_;
  std::string target_file_;
  std::string m3u8_path_;
  std::string hls_base_url_;
};

class MP4OutputFormatter : public OutputFormatter {
 public:
  MP4OutputFormatter(const std::string& target_mp4_path);
  virtual std::string FormatOutput() override;
  virtual OutputFormatter* Clone() override;

 private:
  std::string target_mp4_path_;
};

/////////////////////////////////

class CommandBuilder {
 public:
  CommandBuilder(Conf* conf,
                 const std::string& media_source_path,
                 OutputFormatter* output_formater);
  virtual ~CommandBuilder();

  virtual CommandBuilder* Clone();

  corgi::Status BuildLowDefinitionCommand(Command* result);
  // TODO(xiang88):
  // 还未开启高清转码
  // corgi::Status BuildNormalDefinitionCommand(Command* result);
  // corgi::Status BuildHighDefinitionCommand(Command* result);

 protected:
  CG_DISALLOW_COPY_AND_ASSIGN(CommandBuilder);

  Conf* conf_;
  std::string media_source_path_;
  std::unique_ptr<OutputFormatter> output_formatter_;

  // Build Parts
  std::string BuildGlobalOptions();
  std::string BuildInputFile();
  std::string BuildCodec();
  std::string BuildAudioSample(const std::string& sample);
  std::string BuildVideoSize(const std::string& size, const std::string& aspect);
  std::string BuildVideoBitrate(const std::string& bitrate);
  std::string BuildVideoFps(const std::string& fps);
  std::string BuildH264Info();
};

} // namespace mms
#endif //MEDIA_MANAGE_SYSTEM_COMMAND_BUILDER_H
