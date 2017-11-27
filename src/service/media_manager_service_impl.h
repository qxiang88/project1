//
// Created by xiang88 on 11/21/17.
//

#ifndef MEDIA_MANAGE_SYSTEM_MEDIA_MANAGER_SERVICE_IMPL_H
#define MEDIA_MANAGE_SYSTEM_MEDIA_MANAGER_SERVICE_IMPL_H

#include "corgi/base/status.h"
#include "corgi/base/statusor.h"
#include "proto/media_service.pb.h"
#include "proxy/service_proxy.h"

namespace mms {

class MediaManagerServiceImpl
    : public MediaManagerService {
 public:
  MediaManagerServiceImpl(ServiceProxy* proxy);

  virtual ~MediaManagerServiceImpl() override;

  virtual void Handle(::google::protobuf::RpcController *controller,
                      const ::mms::MediaHttpRequestPB *request,
                      ::mms::MediaHttpResponsePB *response,
                      ::google::protobuf::Closure *done) override;

  std::string HandleNewTask(const std::map<std::string, std::string>& queries,
                            const std::string& json_body);
  std::string HandleQueryTask(const std::map<std::string, std::string>& queries);

  corgi::Status HandleAccessM3U8(const std::map<std::string, std::string>& queries,
                                 FileOutputStream* output_stream);

  std::string HandleAccessHLSKey(const std::map<std::string, std::string>& queries);

 private:
  ServiceProxy* proxy_;

  CG_DISALLOW_COPY_AND_ASSIGN(MediaManagerServiceImpl);
};

} // namespace mms
#endif //MEDIA_MANAGE_SYSTEM_MEDIA_MANAGER_SERVICE_IMPL_H
