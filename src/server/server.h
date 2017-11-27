//
// Created by xiang88 on 11/21/17.
//

#ifndef MEDIA_MANAGE_SYSTEM_SERVER_H
#define MEDIA_MANAGE_SYSTEM_SERVER_H

#include "corgi/base/status.h"

#include "brpc/server.h"
#include "service/media_manager_service_impl.h"
#include "proxy/service_proxy.h"

#include <string>
#include <brpc/server.h>

namespace mms {


class Server final {
 public:
  Server(const std::string& conf_path);
  ~Server();

  corgi::Status Init();
  corgi::Status Start();
  void Run();

 private:
  std::string conf_path_;
  brpc::Server server_;
  brpc::ServerOptions server_options_;

  std::unique_ptr<MediaManagerServiceImpl> service_;
  std::string endpoint_;
  std::unique_ptr<ServiceProxy> service_proxy_;
};

} // namespace mms
#endif //MEDIA_MANAGE_SYSTEM_SERVER_H
