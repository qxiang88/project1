//
// Created by wangqixiang on 2017/11/19.
//

#ifndef MEDIA_MANAGE_SYSTEM_FILESYSTEM_H
#define MEDIA_MANAGE_SYSTEM_FILESYSTEM_H

#include "filesystem/copier.h"
#include "filesystem/cleaner.h"

namespace mms {

struct FilesystemOptions {
  std::string local_workspace_root;
  std::string gfs_mount_point_root;

  Copier* copier;
  Cleaner* cleaner;

  FilesystemOptions() {
    copier = nullptr;
    cleaner = nullptr;
  }

};

class Filesystem {
 public:
  static Filesystem* Default();
  Filesystem();
  corgi::Status Init(const FilesystemOptions& options);

  std::string GetLocalMediaRoot(const std::string& uuid);
  std::string GetGFSMediaRoot(const std::string& uuid);
  std::string GetLocalMediaKeyPath(const std::string& uuid);
  std::string GetLocalMediaKeyInfoFilePath(const std::string& uuid);
  std::string GetLocalMediaM3U8Path(const std::string& uuid);
  std::string GetGFSM3U8Path(const std::string& uuid);
  std::string GetLocalHLSTargetFilename(const std::string& uuid);
  std::string GetLocalMP4TargetFilename(const std::string& uuid);

  corgi::Status CreateLocalMediaRootIfNotExist(const std::string& uuid);
  corgi::Status CreateGFSMediaRootIfNotExist(const std::string& uuid);

  corgi::Status Copy(CopierParameter parameter);
  corgi::Status Cleanup(std::string path);

 private:
  FilesystemOptions options_;
  CG_DISALLOW_COPY_AND_ASSIGN(Filesystem);
};

} // namespace mms
#endif //MEDIA_MANAGE_SYSTEM_FILESYSTEM_H
