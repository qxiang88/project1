//
// Created by wangqixiang on 2017/11/16.
//

#include "task/task_manager.h"
#include "util/hls_key_access_url.h"
#include "corgi/gtl/stl_util.h"
#include "corgi/strings/hex_util.h"

namespace mms {

TaskManager::TaskManager(const std::string& conf_path)
    : conf_path_(conf_path),
      conf_(Conf::Default()),
      filesystem_(Filesystem::Default()) {
}

corgi::Status TaskManager::Init() {
  TranscodeManagerOptions tm_options;
  tm_options.enable_ld_task = true;
  tm_options.enable_nd_task = false;
  tm_options.enable_hd_task = false;
  CG_RETURN_IF_ERROR(conf_->Init(conf_path_));
  FilesystemOptions fs_options;
  fs_options.local_workspace_root = conf_->GetFilesystemLocalWorkspace();
  fs_options.gfs_mount_point_root = conf_->GetFilesystemGFSMountPoint();
  CG_RETURN_IF_ERROR(filesystem_->Init(fs_options));

  // Init DB database
  CG_RETURN_IF_ERROR(NewDatabase(conf_->GetDBEngine(), &database_));
  CG_RETURN_IF_ERROR(database_->Open(conf_->GetDBUri()));
  CG_RETURN_IF_ERROR(NewDBKeyGenerator(conf_->GetDBEngine(), &db_key_generator_));
  acl_.reset(new ACL(database_.get(), db_key_generator_.get()));

  // Init Task Storage
  task_storage_.reset(new TaskStorage(db_key_generator_.get(),
                                      database_.get()));

  // Init Transcoder Module
  CG_RETURN_IF_ERROR(TranscodeManager::Create(tm_options, &transcode_manager_));
  return corgi::Status::OK();
}

TaskManager::~TaskManager() {}

corgi::Status TaskManager::NewTask(const TaskMetadata &metadata,
                                   std::unique_ptr<Task> *result) {
  Task *task;
  std::string uuid = metadata.uuid();

  // TODO(xiang88):
  // 要把创建Task 这段逻辑代理给别的类型实现(TaskFactory)
  //
  if (metadata.target_format() == "hls") {
    CG_RETURN_IF_ERROR(filesystem_->CreateLocalMediaRootIfNotExist(uuid));
    KeyFile key_file(filesystem_->GetLocalMediaKeyPath(uuid));
    CG_RETURN_IF_ERROR(key_file.CreateKey());
    KeyInfoFile key_info_file(filesystem_->GetLocalMediaKeyInfoFilePath(uuid));

    // ACL 生成 KeyToken Template
    // std::string media_key_token;
    // CG_RETURN_IF_ERROR(acl_->GenerateMediaKeyToken(uuid, &media_key_token));
    // std::string media_key_uri;
    // CG_RETURN_IF_ERROR(HLSKeyAccessURL::Encode(conf_->GetKeyServerPrefix(), uuid, media_key_token, &media_key_uri));
    key_info_file.set_media_key_url("$0");

    key_info_file.set_media_key_path(filesystem_->GetLocalMediaKeyPath(uuid));
    key_info_file.set_media_key_iv(key_file.iv_as_hex_string());
    CG_RETURN_IF_ERROR(key_file.Write());
    CG_RETURN_IF_ERROR(key_info_file.Write());

    std::string hl_base_url = conf_->GetMediaServerPrefix() + "/" + uuid + "/";
    HLSOutputFormatter out_formatter(key_info_file.key_info_path(),
                                     filesystem_->GetLocalHLSTargetFilename(uuid),
                                     filesystem_->GetLocalMediaM3U8Path(uuid),
                                     hl_base_url);
    CommandBuilder command_builder(Conf::Default(),
                                   metadata.media_source_path(),
                                   out_formatter.Clone());

    // 创建HLS元数据, 转码完成后这些数据存入数据库中
    MediaKeyPB* media_key_pb = new MediaKeyPB;
    media_key_pb->set_key(key_file.key_as_hex_string());
    media_key_pb->set_iv(key_file.iv_as_hex_string());
    HLSMetaPB hls_meta_pb;
    hls_meta_pb.set_allocated_media_key(media_key_pb);
    hls_meta_pb.set_gfs_m3u8_path(filesystem_->GetGFSM3U8Path(uuid));
    hls_meta_pb.set_uuid(uuid);
    task = new HLSTranscodeTask(metadata,
                                command_builder.Clone(),
                                task_storage_,
                                hls_meta_pb);
  } else {
    // TODO(xiang88):
    // supported MP4 transcode soon.
    return corgi::errors::Unimplemented("not supported target format ", metadata.target_format(), " now");
  }

  // 存储元数据
  corgi::Status s = task->Init();
  if (!s.ok()) {
    delete task;
    return s;
  }

  result->reset(task);
  return corgi::Status::OK();
}

corgi::Status TaskManager::ScheduleTask(Task *task) {
  // TODO(xiang88):
  // 目前只做低清转码
  return transcode_manager_->SubmitToLowDefinitionQueue(task);
}

corgi::Status TaskManager::RetrieveTaskMeta(const std::string &guid,
                                            TaskMetaPB *task_meta) {
  return task_storage_->RetrieveTaskMetadata(guid, task_meta);
}

corgi::Status TaskManager::RetrieveHLSMeta(const std::string &guid,
                                           HLSMetaPB *hls_meta) {
  return task_storage_->RetrieveHLSMetadata(guid, hls_meta);
}

corgi::Status TaskManager::CreateAccessKeyURI(const std::string &guid,
                                                std::string *result) {
  // ACL 生成 KeyToken
  std::string media_key_token;
  CG_RETURN_IF_ERROR(acl_->GenerateMediaKeyToken(guid, &media_key_token));
  std::string media_key_uri;
  CG_RETURN_IF_ERROR(HLSKeyAccessURL::Encode(conf_->GetKeyServerPrefix(), guid, media_key_token, &media_key_uri));
  corgi::gtl::STLStringResizeUninitialized(result, media_key_uri.size());
  result->swap(media_key_uri);
  return corgi::Status::OK();
}

corgi::Status TaskManager::RetrieveHLSKeyIfAllowed(const std::string &guid,
                                                   const std::string &token,
                                                   std::string *result) {
  std::string raw_token;
  CG_RETURN_IF_ERROR(HLSKeyAccessURL::Decode(guid, token, &raw_token));
  CG_RETURN_IF_ERROR(acl_->CheckKeyMediaToken(guid, raw_token));
  HLSMetaPB hls_meta;
  CG_RETURN_IF_ERROR(RetrieveHLSMeta(guid, &hls_meta));

  std::string key_bin = corgi::HexDecode(hls_meta.media_key().key());
  corgi::gtl::STLStringResizeUninitialized(result, key_bin.size());
  result->swap(key_bin);
  return corgi::Status::OK();
}

} // namespace mms
