//
// Created by wangqixiang on 2017/11/18.
//

#include "task/command_builder.h"
#include "corgi/strings/substitute.h"
#include "util/media_meta.h"


namespace mms {

namespace {

const std::string kFFmpegTemplate =
    "ffmpeg"
    " $0"         // Global options
    " -i $1"      // input file
    " $2 "        // Codec Options
    " $3 "        // AudioInfo
    " $4 "        // VideoSize    Size/Aspect
    " $5 "        // VideoFps     Fps, e.g. -r 25 -g 25
    " $6 "        // VideoBitrate Bit rate
    " $7 "        // H264Info
    " $8";        // Output File Format

const std::string kGlobalOptions =
    " -loglevel panic"
    " -hide_banner"
    " -stats"
    " -y";

const std::string kCodecTemplate =
    " -codec:v libx264"
    " -codec:a libfdk_aac";

const std::string kAudioSampleTemplate =
    " -ar $0";
const std::string kVideoSizeTemplate =
    " -s $0 "
    " -aspect $1";

const std::string kVideoBitrateTemplate =
    " -b:v $0"
    " -minrate $0"
    " -maxrate $0"
    " -bufsize $0";

const std::string kVideoFpsTemplate =
    " -r $0"
    " -g $0";

const std::string kH264Template =
    " -flags +loop"
    " -subq 5"
    " -trellis 1"
    " -refs 1"
    " -coder 0"
    " -me_range 16"
    " -qmax 51"
    " -level 30"
    " -async 2"
    " -map 0";

} // namespace
//////////////////// CommandBuilder

CommandBuilder::~CommandBuilder() {}

CommandBuilder::CommandBuilder(Conf *conf,
                               const std::string &media_source_path,
                               OutputFormatter *output_formater)
    : conf_(conf),
      media_source_path_(media_source_path) {
  output_formatter_.reset(output_formater);

}

CommandBuilder* CommandBuilder::Clone() {
  return new CommandBuilder(conf_,
                            media_source_path_,
                            output_formatter_->Clone());
}

corgi::Status CommandBuilder::BuildLowDefinitionCommand(Command *result) {
  std::string r;

  // 增加保留原始视频的比率
  std::string video_aspect = conf_->GetLDVideoAspect();
  std::string video_size = conf_->GetLDVideoSize();
  {
    MediaMeta media_meta(media_source_path_);
    corgi::Status status = media_meta.Init();
    if (!status.ok()) {
      LOG(ERROR) << "Get media meta data error: " << status.ToString();
      LOG(ERROR) << "We keep use Conf file data";
    }
    if (keep_aspect_) {
      video_aspect = media_meta.GetVideoAspectAsString() == "0" ?
                     video_aspect : media_meta.GetVideoAspectAsString();
    }
    // 原始视频是竖屏, 我们需要旋转配置文件中的 WxH
    if (media_meta.IsVerticalScreen()) {
      std::vector<std::string> s = corgi::str_util::Split(video_size, "x");
      video_size = s[1] + "x" + s[0];
    }
  }

  r = corgi::strings::Substitute(
    kFFmpegTemplate.c_str(),
    BuildGlobalOptions(),
    BuildInputFile(),
    BuildCodec(),
    BuildAudioSample(conf_->GetLDAudioSample()),
    BuildVideoSize(video_size, video_aspect),
    BuildVideoFps(conf_->GetLDVideoFps()),
    BuildVideoBitrate(conf_->GetLDVideoBitrate()),
    BuildH264Info(),
    output_formatter_->FormatOutput()
  );
  result->command_executable_path_ = conf_->GetFFmpegExecutablePath();
  result->command_ = r;
  result->command_vec_ = corgi::str_util::Split(r, ' ', corgi::str_util::SkipWhitespace());
  return corgi::Status::OK();
}

// Build parts
std::string CommandBuilder::BuildGlobalOptions() {
  return kGlobalOptions;
}
std::string CommandBuilder::BuildInputFile() {
  return media_source_path_;
}
std::string CommandBuilder::BuildCodec() {
  return kCodecTemplate;
}
std::string CommandBuilder::BuildAudioSample(const std::string &sample) {
  return corgi::strings::Substitute(kAudioSampleTemplate.c_str(), sample);
}
std::string CommandBuilder::BuildVideoSize(const std::string &size, const std::string& aspect) {
  return corgi::strings::Substitute(kVideoSizeTemplate.c_str(), size, aspect);
}
std::string CommandBuilder::BuildVideoBitrate(const std::string &bitrate) {
  return corgi::strings::Substitute(kVideoBitrateTemplate.c_str(), bitrate);
}
std::string CommandBuilder::BuildVideoFps(const std::string &fps) {
  return corgi::strings::Substitute(kVideoFpsTemplate.c_str(), fps);
}
std::string CommandBuilder::BuildH264Info() {
  return kH264Template;
}

////////////////////////////////// OutputFormatter
OutputFormatter::~OutputFormatter() {}

MP4OutputFormatter::MP4OutputFormatter(const std::string &target_mp4_path)
    : target_mp4_path_(target_mp4_path) {}

std::string MP4OutputFormatter::FormatOutput() {
  return target_mp4_path_;
}

OutputFormatter* MP4OutputFormatter::Clone() {
  return new MP4OutputFormatter(target_mp4_path_);
}

namespace {
const std::string kHLSTemplate =
  " -hls_base_url $0"
  " -hls_key_info_file $1"
  " -hls_playlist_type vod"
  " -hls_segment_filename $2"
  " $3";
} // namespace

HLSOutputFormatter::HLSOutputFormatter(const std::string &key_info_file_path,
                                       const std::string &target_file,
                                       const std::string &m3u8_path,
                                       const std::string &hls_base_url)
    : key_info_file_path_(key_info_file_path),
      target_file_(target_file),
      m3u8_path_(m3u8_path),
      hls_base_url_(hls_base_url) {}

std::string HLSOutputFormatter::FormatOutput() {
  return corgi::strings::Substitute(kHLSTemplate.c_str(),
                                    hls_base_url_,
                                    key_info_file_path_,
                                    target_file_,
                                    m3u8_path_);
}

OutputFormatter* HLSOutputFormatter::Clone() {
  return new HLSOutputFormatter(key_info_file_path_, target_file_, m3u8_path_, hls_base_url_);
}

} // namespace mms
