//
// Created by wangqixiang on 2017/11/23.
//

#include "task/http_notifier.h"
#include "util/http_client.h"
#include "corgi/base/errors.h"

namespace mms {

HttpNotifier::HttpNotifier(const std::string &url)
    : Notifier(url) {}

corgi::Status HttpNotifier::Notify(const std::string &data) {
  std::unique_ptr<HttpClient> http_client;
  HttpClientOptions options;
  options.headers["Content-Type"] = "application/json";
  options.url = url_;
  CG_RETURN_IF_ERROR(HttpClient::Create(options, &http_client));
  return http_client->POST(data);
}

} // namespace mms

