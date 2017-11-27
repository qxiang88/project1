//
// Created by wangqixiang on 2017/11/17.
//

#ifndef MEDIA_MANAGE_SYSTEM_KEY_INFO_H
#define MEDIA_MANAGE_SYSTEM_KEY_INFO_H

#include <memory>
#include "corgi/base/status.h"

namespace mms {

// 表示FFmpeg KeyInfoFile
// KeyInfo 文件布局如下:
//  Key URI
//  Key File Path
//  IV

class KeyFile {
 public:
  explicit KeyFile(const std::string& file_path);

  corgi::Status CreateKey();
  std::string path() const { return path_; }
  std::string key_bin() const { return key_bin_; }
  std::string key_as_hex_string() const;
  std::string iv_as_hex_string() const;
  corgi::Status Write(bool force=false);

 private:
  std::string path_;
  std::string key_bin_; // key hex bin
  std::string iv_bin_;  // iv hex bin
  CG_DISALLOW_COPY_AND_ASSIGN(KeyFile);
};

class KeyInfoFile {
 private:
  struct Content {
    std::string media_key_url_;
    std::string media_key_path_;
    std::string media_key_iv_;
  };

 public:
  static corgi::Status Load(const std::string& path,
                            std::unique_ptr<KeyInfoFile>* result);

  explicit KeyInfoFile(const std::string& path);
  void set_media_key_url(const std::string& url) { content_.media_key_url_ = url; }
  void set_media_key_path(const std::string& key_path) { content_.media_key_path_ = key_path; }
  void set_media_key_iv(const std::string& key_iv) { content_.media_key_iv_ = key_iv; }
  std::string media_key_url() const { return content_.media_key_url_;}
  std::string media_key_path() const { return content_.media_key_path_;}
  std::string media_key_iv() const { return content_.media_key_iv_; }

  std::string key_info_path() const { return key_info_path_; }

  corgi::Status Write(bool force=false);
  corgi::Status Read();

 private:
  std::string key_info_path_;
  Content content_;
  CG_DISALLOW_COPY_AND_ASSIGN(KeyInfoFile);
};

} // namespace mms
#endif //MEDIA_MANAGE_SYSTEM_KEY_INFO_H
