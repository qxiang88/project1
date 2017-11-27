//
// Created by wangqixiang on 2017/11/19.
//

#include "filesystem/filesystem.h"
#include "corgi/env/env.h"
#include "corgi/io/path.h"

namespace mms {

namespace {

///////// Helpers
corgi::Status CreateDirIfNotExist(const std::string& root) {
  corgi::Status status = corgi::Env::Default()->FileExists(root);
  if (status.ok()) {
    return status;
  }
  return corgi::Env::Default()->RecursivelyCreateDir(root);
}

} // namespace

Filesystem *Filesystem::Default() {
  static Filesystem instance;
  return &instance;
}

Filesystem::Filesystem() {}

corgi::Status Filesystem::Init(const FilesystemOptions &options) {
  options_ = options;
  if (options_.copier == nullptr) {
    options_.copier = new SimpleCopier;
  }
  if (options_.cleaner == nullptr) {
    options_.cleaner = new SimpleCleaner;
  }

  CG_RETURN_IF_ERROR(CreateDirIfNotExist(options.local_workspace_root));
  CG_RETURN_IF_ERROR(CreateDirIfNotExist(options.gfs_mount_point_root));
  return corgi::Status::OK();
}

std::string Filesystem::GetLocalMediaRoot(const std::string &uuid) {
  return corgi::io::JoinPath(options_.local_workspace_root,
                             uuid);
}

std::string Filesystem::GetGFSMediaRoot(const std::string &uuid) {
  return corgi::io::JoinPath(options_.gfs_mount_point_root,
  uuid);
}

std::string Filesystem::GetLocalMediaKeyPath(const std::string &uuid) {
  return corgi::io::JoinPath(GetLocalMediaRoot(uuid),
  uuid.substr(0,8)+".key");
}

std::string Filesystem::GetLocalMediaKeyInfoFilePath(const std::string &uuid) {
  return corgi::io::JoinPath(GetLocalMediaRoot(uuid),
  uuid.substr(0,8)+".keyinfo");
}

std::string Filesystem::GetLocalMediaM3U8Path(const std::string &uuid) {
  return corgi::io::JoinPath(GetLocalMediaRoot(uuid),
  uuid.substr(0,8)+"_templ.m3u8");
}

std::string Filesystem::GetGFSM3U8Path(const std::string &uuid) {
  return corgi::io::JoinPath(GetGFSMediaRoot(uuid),
  uuid.substr(0,8)+"_templ.m3u8");
}

std::string Filesystem::GetLocalHLSTargetFilename(const std::string &uuid) {
  return corgi::io::JoinPath(GetLocalMediaRoot(uuid),
    uuid.substr(0, 8)+"%d.ts");
}

std::string Filesystem::GetLocalMP4TargetFilename(const std::string &uuid) {
  return corgi::io::JoinPath(GetLocalMediaRoot(uuid),
    uuid.substr(0, 8)+".mp4");
}



corgi::Status Filesystem::CreateLocalMediaRootIfNotExist(const std::string &uuid) {
  return CreateDirIfNotExist(GetLocalMediaRoot(uuid));
}

corgi::Status Filesystem::CreateGFSMediaRootIfNotExist(const std::string &uuid) {
  return CreateDirIfNotExist(GetGFSMediaRoot(uuid));
}

/////////
corgi::Status Filesystem::Copy(CopierParameter parameter) {
  if (options_.copier) {
    return options_.copier->Copy(parameter);
  }
  return corgi::errors::NotFound("Filesystem not found Copier");
}

corgi::Status Filesystem::Cleanup(std::string path) {
  if (options_.cleaner) {
    return options_.cleaner->Cleanup(path);
  }
  return corgi::errors::NotFound("Filesystem not found Cleaner");
}

} // namespace mms
