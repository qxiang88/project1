//
// Created by wangqixiang on 2017/11/18.
//

#include "proxy/service_proxy.h"
#include "util/m3u8_reader.h"
#include "util/guid.h"
#include "corgi/strings/substitute.h"

namespace mms {

ServiceProxy::ServiceProxy(const std::string& conf_path)
    : conf_path_(conf_path) {}

ServiceProxy::~ServiceProxy() {}

corgi::Status ServiceProxy::Init() {
  task_manager_.reset(new TaskManager(conf_path_));
  corgi::Status status = task_manager_->Init();
  if (status.ok()) {
    LOG(INFO) << "ServiceProxy start complete";
  }
  return status;
}

corgi::Status ServiceProxy::HandleNewTask(const TranscodeRequestPB *request,
                                          TranscodeResponsePB *response) {
  TaskMetadata medadata;
  std::string uuid = GenerateGUID();
  medadata.SetUUID(uuid);
  medadata.SetMediaSourcePath(request->media_source_path());
  medadata.SetTargetFormat(request->media_target_format());
  std::unique_ptr<Task> task;
  CG_RETURN_IF_ERROR(task_manager_->NewTask(medadata, &task));
  if (request->has_notify_url()) {
    task->set_notify_url(request->notify_url());
  }
  CG_RETURN_IF_ERROR(task_manager_->ScheduleTask(task.release()));
  response->set_uuid(uuid);
  return corgi::Status::OK();
}

corgi::Status ServiceProxy::HandleQueryTask(const QueryStateRequestPB *request,
                                            QueryStateResponsePB *response) {
  TaskMetaPB task_meta;
  CG_RETURN_IF_ERROR(task_manager_->RetrieveTaskMeta(request->uuid(), &task_meta));
  response->set_uuid(request->uuid());
  response->set_state(task_meta.state_str());
  return corgi::Status::OK();
}

corgi::Status ServiceProxy::HandleAccessHLSKey(const AccessKeyRequestPB *request,
                                               AccessKeyResponsePB *response) {
  std::string key_bin;
  CG_RETURN_IF_ERROR(task_manager_->RetrieveHLSKeyIfAllowed(request->uuid(), request->token(), &key_bin));
  response->set_uuid(request->uuid());
  response->set_key(key_bin);
  return corgi::Status::OK();
}

corgi::Status ServiceProxy::HandleAccessM3U8(const AccessM3U8RequestPB *request,
                                             AccessM3U8ResponsePB *response,
                                             FileOutputStream *output_stream) {
  TaskMetaPB task_meta;
  CG_RETURN_IF_ERROR(task_manager_->RetrieveTaskMeta(request->uuid(), &task_meta));
  if (task_meta.state_int() != kCopyDone) {
    return corgi::errors::Unavailable("resource uuid: ", request->uuid());
  }
  HLSMetaPB hls_meta;
  CG_RETURN_IF_ERROR(task_manager_->RetrieveHLSMeta(request->uuid(), &hls_meta));

  std::string gfs_m3u8_template_path = hls_meta.gfs_m3u8_path();
  M3U8Options m3u8_options;
  m3u8_options.m3u8_path_ = gfs_m3u8_template_path;
  std::unique_ptr<M3U8Reader> reader;
  CG_RETURN_IF_ERROR(M3U8Reader::Create(m3u8_options, &reader));
  std::string access_key_uri;
  CG_RETURN_IF_ERROR(task_manager_->CreateAccessKeyURI(request->uuid(), &access_key_uri));

  std::string line;
  while (true) {
    corgi::Status s = reader->ReadLine(&line);
    if (corgi::errors::IsOutOfRange(s)) {
      break;
    }
    if (!s.ok()) {
      LOG(ERROR) << "Stream uuid: " << request->uuid()
                 << " m3u8: " << gfs_m3u8_template_path
                 << " error: " << s.ToString();
      return corgi::errors::Internal("streaming read m3u8");
    }
    line += "\n";

    // Dynamic assign AccessKeyURI
    corgi::Slice slice(line);
    if (slice.starts_with("#EXT-X-KEY:")) {
      line = corgi::strings::Substitute(line.c_str(), access_key_uri);
    }

    s = output_stream->Write(line.c_str(), line.size());
    if (!s.ok()) {
      LOG(ERROR) << "Stream uuid: " << request->uuid()
                 << " m3u8: " << gfs_m3u8_template_path
                 << " error: " << s.ToString();
      return corgi::errors::Internal("streaming write m3u8");
    }
  }
  return corgi::Status::OK();
}

} // namespace mms

