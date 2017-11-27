//
// Created by wangqixiang on 2017/11/23.
//

#ifndef MEDIA_MANAGE_SYSTEM_HTTP_CLIENT_H
#define MEDIA_MANAGE_SYSTEM_HTTP_CLIENT_H

#include "corgi/base/status.h"
#include <map>

namespace mms {

struct HttpClientOptions {
  std::string url;
  std::string protocol = "http";
  corgi::uint64 timeout_ms = 3 * 1000;
  uint32_t max_retries = 3;
  std::map<std::string, std::string> headers;
  std::map<std::string, std::string> queries;
};

class HttpClient {
 public:
  virtual ~HttpClient();

  static corgi::Status Create(const HttpClientOptions& options,
                              std::unique_ptr<HttpClient>* result);

  virtual corgi::Status GET(std::string* result=nullptr) = 0;

  virtual corgi::Status POST(const std::string& body,
                             std::string* result=nullptr) = 0;

 protected:
  HttpClientOptions options_;
  HttpClient(const HttpClientOptions& options);
};

} // namespace mms
#endif //MEDIA_MANAGE_SYSTEM_HTTP_CLIENT_H
