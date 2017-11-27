//
// Created by wangqixiang on 2017/11/19.
//

#include "acl/acl.h"
#include "corgi/base/errors.h"
#include "crypto/crypto_util.h"
#include "corgi/gtl/stl_util.h"
#include "corgi/env/env.h"
#include "corgi/strings/hex_util.h"

namespace mms {

ACL::ACL(Database *database,
         DBKeyGenerator *db_key_generator)
    : database_(database),
      db_key_generator_(db_key_generator) {}

// Token 生成算法:
//   [16bytes 随机数]-[时间戳]
namespace {

corgi::Status CreateToken(std::string* result) {
  std::unique_ptr<unsigned char> buf(new unsigned char[16]);
  bool b = RandBytes(buf.get(), 16);
  if (!b) {
    return corgi::errors::Internal("RandBytes to CreateToken");
  }
  std::string rnd((const char*)buf.get(), 16);
  std::string r =
      corgi::strings::StrCat(
          corgi::HexEncode(rnd), "-", corgi::Env::Default()->NowSeconds());
  corgi::gtl::STLStringResizeUninitialized(result, r.size());
  result->swap(r);
  return corgi::Status::OK();
}

} // namespace

corgi::Status ACL::GenerateMediaKeyToken(const std::string &uuid,
                                         std::string *result) {
  std::string token;
  CG_RETURN_IF_ERROR(CreateToken(&token));
  std::string token_key = db_key_generator_->CreateTokenKey(token);
  CG_RETURN_IF_ERROR(database_->Put(token_key, "1"));
  result->assign(token);
  return corgi::Status::OK();
}

corgi::Status ACL::CheckKeyMediaToken(const std::string &uuid,
                                      const std::string &token) {
  std::string token_key = db_key_generator_->CreateTokenKey(token);
  std::string v;
  corgi::Status s = database_->Get(token_key, &v);
  if (!s.ok()) {
    return corgi::errors::Unauthenticated("unauthenticated token: ", token);
  }
  s = database_->Delete(token_key);
  if (!s.ok()) {
    LOG(WARNING) << "Delete token: " << token_key;
  }
  return corgi::Status::OK();
}

} // namespace mms
