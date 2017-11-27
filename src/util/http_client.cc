//
// Created by wangqixiang on 2017/11/23.
//

#include "util/http_client.h"

namespace mms {

HttpClient::HttpClient(const HttpClientOptions &options)
    : options_(options) {}

HttpClient::~HttpClient() {}

} // namespace mms
