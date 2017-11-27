//
// Created by wangqixiang on 2017/11/20.
//

#include "util/hls_key_access_url.h"

#include "corgi/strings/base64.h"
#include "corgi/strings/url_coding.h"
#include "corgi/base/errors.h"
#include "corgi/strings/strcat.h"
#include "corgi/gtl/stl_util.h"

namespace mms {

// 返回:
// key_server?uuid=....
//           &token=url_encode(base64(token))
corgi::Status HLSKeyAccessURL::Encode(const std::string &key_server,
                                      const std::string &uuid,
                                      const std::string &token,
                                      std::string* result) {
  std::string b64;
  CG_RETURN_IF_ERROR(corgi::Base64Encode(token, &b64));
  std::string token_b64url = corgi::URLCoding::Encode(b64);
  std::string r = corgi::strings::StrCat(
      key_server, "&", "uuid=", uuid, "&", "token=", token_b64url);
  corgi::gtl::STLStringResizeUninitialized(result, r.size());
  result->swap(r);
  return corgi::Status::OK();
}

corgi::Status HLSKeyAccessURL::Decode(const std::string &uuid,
                                      const std::string &b64url_token,
                                      std::string* result) {
  // not use `uuid` yet.
  std::string token;
  std::string b64token = corgi::URLCoding::Decode(b64url_token);
  CG_RETURN_IF_ERROR(corgi::Base64Decode(b64token, &token));
  corgi::gtl::STLStringResizeUninitialized(result, token.size());
  result->swap(token);
  return corgi::Status::OK();
}

} // namespace mms
