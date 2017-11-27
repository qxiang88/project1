//
// Created by wangqixiang on 2017/11/23.
//

#include "util/http_client_impl.h"

#include "corgi/base/errors.h"
#include "corgi/gtl/stl_util.h"

namespace mms {

HttpClientImpl::HttpClientImpl(const HttpClientOptions &options)
    : HttpClient(options) {}

corgi::Status HttpClientImpl::GET(std::string *result) {
  return DoRequest(result);
}

corgi::Status HttpClientImpl::POST(const std::string &body,
                                   std::string *result) {
  return DoRequest(result, body);
}

corgi::Status HttpClientImpl::DoRequest(std::string* result,
                                        const std::string &body) {

  brpc::Channel channel;
  brpc::Controller controller;

  brpc::ChannelOptions channel_options;
  channel_options.protocol = options_.protocol;
  channel_options.max_retry = options_.max_retries;
  channel_options.timeout_ms = options_.timeout_ms;

  if (channel.Init(options_.url.c_str(), &channel_options) != 0) {
    return corgi::errors::InvalidArgument("Channel init");
  }

  controller.http_request().uri() = options_.url;
  for (auto& h : options_.headers) {
    controller.http_request().SetHeader(h.first, h.second);
  }
  for (auto& q : options_.queries) {
    controller.http_request().uri().SetQuery(q.first, q.second);
  }

  if (body.empty()) { // GET
    controller.http_request().set_method(brpc::HTTP_METHOD_GET);
  } else { // POST
    controller.http_request().set_method(brpc::HTTP_METHOD_POST);
    controller.request_attachment().append(body);
  }

  channel.CallMethod(NULL, &controller, NULL, NULL, NULL);
  if (controller.Failed()) {
    return corgi::errors::Internal(controller.ErrorText());
  }
  if (result != nullptr) {
    corgi::gtl::STLStringResizeUninitialized(result, controller.response_attachment().size());
    result->assign(controller.response_attachment().to_string());
  }
  return corgi::Status::OK();
}

////////
// static
corgi::Status HttpClient::Create(const HttpClientOptions &options,
                                 std::unique_ptr<HttpClient> *result) {
  HttpClientImpl* http_client = new HttpClientImpl(options);
  result->reset(http_client);
  return corgi::Status::OK();
}

} // namespace mms
