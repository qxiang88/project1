//
// Created by wangqixiang on 2017/11/18.
//

#ifndef MEDIA_MANAGE_SYSTEM_SERVER_PROXY_H
#define MEDIA_MANAGE_SYSTEM_SERVER_PROXY_H

#include "corgi/base/status.h"
#include "corgi/base/errors.h"

#include "config/conf.h"
#include "task/task_manager.h"

#include "proto/protocol.pb.h"

namespace mms {

class FileOutputStream {
 public:
  virtual ~FileOutputStream() {}

  virtual corgi::Status Write(const char* buf, size_t len) = 0;
};

class ServiceProxy {
 public:
  ServiceProxy(const std::string& conf_path);
  ~ServiceProxy();

  corgi::Status Init();

  // Handles
  corgi::Status HandleNewTask(const TranscodeRequestPB* request,
                              TranscodeResponsePB* response);
  corgi::Status HandleQueryTask(const QueryStateRequestPB* request,
                                QueryStateResponsePB* response);
  corgi::Status HandleAccessM3U8(const AccessM3U8RequestPB* request,
                                 AccessM3U8ResponsePB* response,
                                 FileOutputStream* output_stream);
  corgi::Status HandleAccessHLSKey(const AccessKeyRequestPB* request,
                                   AccessKeyResponsePB* response);

 private:
  std::string conf_path_;
  std::unique_ptr<TaskManager> task_manager_;

  CG_DISALLOW_COPY_AND_ASSIGN(ServiceProxy);
};

} // namespace mms
#endif //MEDIA_MANAGE_SYSTEM_SERVER_PROXY_H
