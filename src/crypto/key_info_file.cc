//
// Created by wangqixiang on 2017/11/17.
//

#include "crypto/key_info_file.h"
#include "corgi/env/env.h"
#include "corgi/strings/str_util.h"
#include "crypto/crypto.h"
#include "corgi/strings/hex_util.h"

namespace mms {

KeyFile::KeyFile(const std::string &file_path)
    : path_(file_path) {}

corgi::Status KeyFile::CreateKey() {
  CG_RETURN_IF_ERROR(Crypto::Default()->GenerateKey128(&key_bin_));
  CG_RETURN_IF_ERROR(Crypto::Default()->GenerateIV(&iv_bin_));
  return corgi::Status::OK();
}

std::string KeyFile::key_as_hex_string() const {
  return corgi::HexEncode(key_bin_);
}

std::string KeyFile::iv_as_hex_string() const {
  return corgi::HexEncode(iv_bin_);
}

corgi::Status KeyFile::Write(bool force) {
  corgi::Status status =
      corgi::Env::Default()->FileExists(path_);
  if (status.ok()) {
    if (!force) {
      return corgi::errors::AlreadyExists("path: ", path_);
    }
  }
  return corgi::WriteStringToFile(corgi::Env::Default(),
                                  path_,
                                  key_bin_);
}

/////////////////////////////

KeyInfoFile::KeyInfoFile(const std::string &path)
    : key_info_path_(path) {}

corgi::Status KeyInfoFile::Write(bool force) {
  if (content_.media_key_url_.empty()  ||
      content_.media_key_path_.empty() ||
      content_.media_key_iv_.empty()) {
    return corgi::errors::InvalidArgument(
        "media_key_url, media_key_path, media_key_iv MUST be set");
  }

  corgi::Status status = corgi::Env::Default()->FileExists(key_info_path_);
  if (status.ok()) {
    if (!force) {
      return corgi::errors::AlreadyExists(key_info_path_, " already exist");
    }
  }
  std::unique_ptr<corgi::WritableFile> file;
  if (status.ok()) { // 覆盖已有的文件
    CG_RETURN_IF_ERROR(corgi::Env::Default()->NewWritableFile(key_info_path_, &file));
  } else {
    CG_RETURN_IF_ERROR(corgi::Env::Default()->NewAppendableFile(key_info_path_, &file));
  }

  std::string data =
      corgi::strings::StrCat(
        content_.media_key_url_, "\n",
        content_.media_key_path_,"\n",
        content_.media_key_iv_, "\n"
      );
  CG_RETURN_IF_ERROR(file->Append(data));
  CG_RETURN_IF_ERROR(file->Close());
  return corgi::Status::OK();
}

corgi::Status KeyInfoFile::Read() {
  std::unique_ptr<corgi::RandomAccessFile> file;
  CG_RETURN_IF_ERROR(corgi::Env::Default()->NewRandomAccessFile(key_info_path_, &file));
  std::string result = "";
  corgi::Slice input_buf;
  char scratch[1024];
  corgi::Status status;
  corgi::uint64 offset = 0;

  while (true) {
    status = file->Read(offset, 1024, &input_buf, scratch);
    if (!status.ok()) {
      if (status.code() != corgi::Code::OUT_OF_RANGE) {
        return status;
      }
    }
    if (input_buf.size() == 0) {
      break;
    }
    offset += input_buf.size();
    result = corgi::strings::StrCat(result, input_buf);
  }
  std::vector<std::string> strs = corgi::str_util::Split(result, '\n', corgi::str_util::SkipWhitespace());

  if (strs.size() != 3) { // key_info_file 总共只有三个域
    return corgi::errors::DataLoss("key info file data loss");
  }
  content_.media_key_url_ = strs[0];
  content_.media_key_path_ = strs[1];
  content_.media_key_iv_ = strs[2];

  return corgi::Status::OK();
}

corgi::Status KeyInfoFile::Load(const std::string &path,
                                std::unique_ptr<KeyInfoFile> *result) {
  KeyInfoFile* file(new KeyInfoFile(path));
  corgi::Status status = file->Read();
  if (!status.ok()) {
    delete file;
    return status;
  }
  result->reset(file);
  return status;
}

} // namespace mms
