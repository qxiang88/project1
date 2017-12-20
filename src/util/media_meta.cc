//
// Created by wangqixiang on 2017/12/20.
//

#include "util/media_meta.h"

#include "config/conf.h"
#include "corgi/base/errors.h"
#include "corgi/json/json.h"
#include "corgi/subprocess/subprocess.h"

#include <limits>
#include <cmath>

namespace mms {

class MediaMeta::Rep {
 public:
  Rep(const std::string& path)
      : media_path_(path) {}

  ~Rep() {}

  corgi::Status Parse();

  std::string GetVideoInfoAsStr(const std::string& key) const {
    return video_stream_[key].asString();
  }

  int64_t GetVideoInfoAsInt(const std::string& key) const {
    return video_stream_[key].asInt64();
  }

  std::string GetFormat(const std::string& key) const {
    return format_[key].asString();
  }

 private:
  std::string media_path_;
  corgi::json::Value root_;
  corgi::json::Value video_stream_;
  corgi::json::Value format_;
};

// ffprobe -v quiet -hide_banner -print_format json -show_
// ormat -show_streams  t.mp4

corgi::Status MediaMeta::Rep::Parse() {
  corgi::SubProcess ffprobe;
  ffprobe.SetProgram(Conf::Default()->GetFFprobeExecutablePath(),
                     {"ffprobe", "-v", "quiet",
                      "-hide_banner", "-print_format", "json",
                     "-show_format", "-show_streams", media_path_});
  ffprobe.SetChannelAction(corgi::CHAN_STDOUT, corgi::ACTION_PIPE);
  ffprobe.SetChannelAction(corgi::CHAN_STDERR, corgi::ACTION_PIPE);

  std::string result;
  std::string err;

  if (!ffprobe.Start()) {
    return corgi::errors::Internal("ffprobe start error");
  }
  int status = ffprobe.Communicate(nullptr, &result, &err);
  if (WEXITSTATUS(status) != 0) {
    return corgi::errors::Internal("ffprobe exist code: ", status, " error: ",
      err);
  }

  // Deserialize Json
  corgi::json::CharReaderBuilder builder;
  std::unique_ptr<corgi::json::CharReader> reader(builder.newCharReader());
  bool b = reader->parse(result.data(),
                         result.data() + result.size(),
                         &root_, &err);
  if (!b) {
    return corgi::errors::InvalidArgument("ffprobe result json error: ", err);
  }

  // 查找 Video Stream
  for (auto s = root_["streams"].begin(); s != root_["streams"].end(); ++s) {
    if (s->isMember("codec_type") &&
        s->get("codec_type", "").asString() == "video") {
      video_stream_ = *s;
      break;
    }
  }
  format_ = root_["format"];
  return corgi::Status::OK();
}

//////////////////////////////

MediaMeta::MediaMeta(const std::string &path)
    : rep_(new MediaMeta::Rep(path)) {}

corgi::Status MediaMeta::Init() {
  return rep_->Parse();
}

MediaMeta::~MediaMeta() {}

// Get*

int64_t MediaMeta::GetVideoWidth() const {
  return rep_->GetVideoInfoAsInt("width");
}

int64_t MediaMeta::GetVideoHeight() const {
  return rep_->GetVideoInfoAsInt("height");
}

double MediaMeta::GetVideoAspect() const {
  if (GetVideoHeight() == 0) return 0.0;
  return (double) GetVideoWidth() / (double)GetVideoHeight();
}

std::string MediaMeta::GetVideoAspectAsString() const {
  // 支持 FFmpeg 识别
  double aspect = GetVideoAspect();
  double k16x9Aspect = 1.7777;
  double k4x3Aspsect = 1.3333;
  double kEpsilon = 0.0001;

  LOG(INFO) << "Delta1: " << std::fabs(aspect - k4x3Aspsect);
  LOG(INFO) << "Delta2: " << std::fabs(aspect - k16x9Aspect);
  LOG(INFO) << "Epsilon: " << kEpsilon;

  if (std::fabs(aspect - k4x3Aspsect) < kEpsilon) {
    return "1.3333";
  }
  if (std::fabs(aspect - k16x9Aspect) < kEpsilon) {
    return "1.7777";
  }
  return "0";
}

std::string MediaMeta::GetDuration() const {
  return rep_->GetFormat("duration");
}

std::string MediaMeta::GetSize() const {
  return rep_->GetFormat("size");
}

} // namespace mms
