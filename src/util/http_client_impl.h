//
// Created by wangqixiang on 2017/11/23.
//

#ifndef MEDIA_MANAGE_SYSTEM_HTTP_CLIENT_IMPL_H
#define MEDIA_MANAGE_SYSTEM_HTTP_CLIENT_IMPL_H

#include "util/http_client.h"
#include "brpc/channel.h"

namespace mms {

class HttpClientImpl : public HttpClient {
 public:
  ~HttpClientImpl() {}

  HttpClientImpl(const HttpClientOptions& options);

  virtual corgi::Status GET(std::string* result=nullptr) override;
  virtual corgi::Status POST(const std::string& body,
                             std::string* result=nullptr) override;
 private:
  corgi::Status DoRequest(std::string* result=nullptr,
                          const std::string& body="");
};

} // namespace mms

#endif //MEDIA_MANAGE_SYSTEM_HTTP_CLIENT_IMPL_H
