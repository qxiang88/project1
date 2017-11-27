//
// Created by xiang88 on 11/21/17.
//

#include "server/server.h"
#include "config/conf.h"
#include "corgi/base/errors.h"

namespace mms {

Server::Server(const std::string &conf_path)
    : conf_path_(conf_path) {}

Server::~Server() {}

corgi::Status Server::Init() {
  // Init config file
  CG_RETURN_IF_ERROR(Conf::Default()->Init(conf_path_));
  std::string listen_addr = Conf::Default()->GetListenAddress();
  int port = Conf::Default()->GetPort();

  endpoint_ = corgi::strings::StrCat(
    listen_addr, ":", port
  );

  service_proxy_.reset(new ServiceProxy(conf_path_));
  CG_RETURN_IF_ERROR(service_proxy_->Init());

  service_.reset(new MediaManagerServiceImpl(service_proxy_.get()));
  if (server_.AddService(service_.get(),
                         brpc::SERVER_DOESNT_OWN_SERVICE,
                         "/task/v1 => Handle") != 0) {
    return corgi::errors::Internal("AddService error");
  }

  //TODO(xiang88):
  // Remove to conf file
  server_options_.idle_timeout_sec = 30;
//  server_options_.
  return corgi::Status::OK();
}

corgi::Status Server::Start() {
  if (server_.Start(endpoint_.c_str(), &server_options_) != 0) {
    return corgi::errors::Internal("Server Start error");
  }
  return corgi::Status::OK();
}

void Server::Run() {
  server_.RunUntilAskedToQuit();
}

} // namespace mms
