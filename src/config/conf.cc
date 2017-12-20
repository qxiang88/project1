//
// Created by wangqixiang on 2017/11/17.
//

#include "config/conf.h"
#include "corgi/base/errors.h"

#include <fstream>

namespace mms {

namespace {

const std::string kDefListenAddress = "127.0.0.1";
const int kDefPort = 8090;

const std::string kDefDBUri = "/tmp/mms/mms_db";
const std::string kDefDBEngine = "leveldb";

const std::string kDefMediaServerPrefix = "http://localhost";

const std::string kDefFFmpegExecutablePath = "/usr/local/bin/ffmpeg";
const std::string kDefFFprobeExecutablePath = "/usr/local/bin/ffprobe";

const std::string kDefLDAudioSample = "48000";
const std::string kDefLDVideoSize = "320x240";
const std::string kDefLDVideoAspect = "4:3";
const std::string kDefLDVideoBitrate = "800k";
const std::string kDefLDVideoFps = "25";

const std::string kDefKeyServerPrefix = "http://localhost:8090";

const std::string kDefFilesystemLocalWorkspace = "/tmp/mms/workspace";
const std::string kDefFilesystemGFSMountPoint = "/data/media/";

} //

// static
Conf *Conf::Default() {
  static Conf conf_instance;
  return &conf_instance;
}

Conf::Conf() {}

corgi::Status Conf::Init(const std::string &conf_path) {
  corgi::json::CharReaderBuilder builder;
  JSONCPP_STRING errs;
  std::fstream in;

  in.open(conf_path, std::ios_base::in);
  if (!in.is_open()) {
    return corgi::errors::Internal("open file error: ", conf_path);
  }
  bool ok = corgi::json::parseFromStream(builder, in, &root_, &errs);
  if (!ok) {
    return corgi::errors::Internal("parse json file ", conf_path, " error: ", errs);
  }
  conf_path_ = conf_path;
  return corgi::Status::OK();
}

corgi::Status Conf::Reconfigure() {
  corgi::mutex_lock l(mu_);
  return Init(conf_path_);
}

std::string Conf::GetListenAddress() const {
  corgi::mutex_lock l(mu_);
  std::string addr = root_["server"]["listen_address"].asString();
  return addr.empty() ? kDefListenAddress : addr;
}

int Conf::GetPort() const {
  corgi::mutex_lock l(mu_);
  int port = root_["server"]["port"].asInt();
  return port ? port : kDefPort;
}

std::string Conf::GetDBUri() const {
  corgi::mutex_lock l(mu_);
  std::string v = root_["db"]["uri"].asString();
  return v.empty() ? kDefDBUri : v;
}

std::string Conf::GetDBEngine() const {
  corgi::mutex_lock l(mu_);
  std::string v = root_["db"]["engine"].asString();
  return v.empty() ? kDefDBEngine : v;
}

std::string Conf::GetMediaServerPrefix() const {
  corgi::mutex_lock l(mu_);
  std::string v = root_["media"]["media_server_prefix"].asString();
  return v.empty() ? kDefMediaServerPrefix : v;
}

std::string Conf::GetKeyServerPrefix() const {
  corgi::mutex_lock l(mu_);
  std::string v = root_["key_mgr"]["key_server_prefix"].asString();
  return v.empty() ? kDefKeyServerPrefix : v;
}

std::string Conf::GetFilesystemLocalWorkspace() const {
  corgi::mutex_lock l(mu_);
  std::string v = root_["fs"]["local_workspace"].asString();
  return v.empty() ? kDefFilesystemLocalWorkspace : v;
}

std::string Conf::GetFilesystemGFSMountPoint() const {
  corgi::mutex_lock l(mu_);
  std::string v = root_["fs"]["gfs_mount_point"].asString();
  return v.empty() ? kDefFilesystemGFSMountPoint : v;
}

////////////////////

std::string Conf::GetFFmpegExecutablePath() const {
  corgi::mutex_lock l(mu_);
  std::string v = root_["transcoder"]["ffmpeg_executable_path"].asString();
  return v.empty() ? kDefFFmpegExecutablePath : v;
}

std::string Conf::GetFFprobeExecutablePath() const {
  corgi::mutex_lock l(mu_);
  std::string v = root_["transcoder"]["ffprobe_executable_path"].asString();
  return v.empty() ? kDefFFprobeExecutablePath : v;
}

std::string Conf::GetLDAudioSample() const {
  corgi::mutex_lock l(mu_);
  std::string sample = root_["transcoder"]["ld"]["audio"]["sample"].asString();
  return sample.empty() ? kDefLDAudioSample : sample;
}

std::string Conf::GetLDVideoSize() const {
  corgi::mutex_lock l(mu_);
  std::string v = root_["transcoder"]["ld"]["video"]["size"].asString();
  return v.empty() ? kDefLDVideoSize : v;
}

std::string Conf::GetLDVideoAspect() const {
  corgi::mutex_lock l(mu_);
  std::string v = root_["transcoder"]
    ["ld"]["video"]["aspect"].asString();
  return v.empty() ? kDefLDVideoAspect : v;
}

std::string Conf::GetLDVideoBitrate() const {
  corgi::mutex_lock l(mu_);
  std::string v = root_["transcoder"]
  ["ld"]["video"]["bitrate"].asString();
  return v.empty() ? kDefLDVideoBitrate : v;
}

std::string Conf::GetLDVideoFps() const {
  corgi::mutex_lock l(mu_);
  std::string v = root_["transcoder"]
  ["ld"]["video"]["fps"].asString();
  return v.empty() ? kDefLDVideoFps : v;
}

} // namespace mms
