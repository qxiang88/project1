//
// Created by wangqixiang on 2017/11/17.
//

#ifndef MEDIA_MANAGE_SYSTEM_CONF_FILE_H
#define MEDIA_MANAGE_SYSTEM_CONF_FILE_H

#include "corgi/base/status.h"
#include "corgi/synchronization/mutex.h"
#include "corgi/json/json.h"

#include <memory>
#include "util/video_aspect_selector.h"

namespace mms {

class Conf {
 public:
  static Conf* Default();
  corgi::Status Init(const std::string& conf_path);
  corgi::Status Reconfigure();

  // Server's info
  std::string GetListenAddress() const;
  int GetPort() const;

  // DB's info
  std::string GetDBUri() const;
  std::string GetDBEngine() const;

  // Media's info
  std::string GetMediaServerPrefix() const;

  // Transcoder's info
  std::string GetFFmpegExecutablePath() const;
  std::string GetFFprobeExecutablePath() const;
  std::string GetLDAudioSample() const;
  std::string GetLDVideoSize() const;
  std::string GetLDVideoAspect() const;
  std::string GetLDVideoBitrate() const;
  std::string GetLDVideoFps() const;

  void GetLDAspectAndSizes(VideoAspectSelector::AspectResolutionList& result) const;

  // KeyManager's info
  std::string GetKeyServerPrefix() const;

  // Filesystem's Info
  std::string GetFilesystemLocalWorkspace() const;
  std::string GetFilesystemGFSMountPoint() const;

 private:
  Conf();

  std::string conf_path_;
  corgi::json::Value root_;

  mutable corgi::mutex mu_;
  CG_DISALLOW_COPY_AND_ASSIGN(Conf);
};

} // namespace mms
#endif //MEDIA_MANAGE_SYSTEM_CONF_FILE_H
