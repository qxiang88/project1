//
// Created by wangqixiang on 2017/11/20.
//

#ifndef MEDIA_MANAGE_SYSTEM_HLS_KEY_ACCESS_URL_H
#define MEDIA_MANAGE_SYSTEM_HLS_KEY_ACCESS_URL_H

#include "corgi/base/status.h"

namespace mms {

// TODO(xiang88):
// Decode 只是针对 token 的解码, 名字起得引起混淆.
class HLSKeyAccessURL {
 public:
  static corgi::Status Encode(const std::string& key_server,
                              const std::string& uuid,
                              const std::string& token,
                              std::string* result);
  static corgi::Status Decode(const std::string& uuid,
                              const std::string& b64url_token,
                              std::string* result);

};

} // namespace mms
#endif //MEDIA_MANAGE_SYSTEM_HLS_KEY_ACCESS_URL_H
