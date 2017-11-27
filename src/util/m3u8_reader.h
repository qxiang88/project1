//
// Created by wangqixiang on 2017/11/22.
//

#ifndef MEDIA_MANAGE_SYSTEM_M3U8_READER_H
#define MEDIA_MANAGE_SYSTEM_M3U8_READER_H

#include "corgi/base/status.h"

namespace mms {

struct M3U8Options {
  std::string m3u8_path_;

  M3U8Options() {}
};

// TODO(xiang88):
//
// 我们使用两阶段解析的方法, 增加 Parser 类.
// 目前项目太赶, 我们主要是想替换掉 EXT-X-KEY 属性.

class M3U8Reader {
 public:
  virtual ~M3U8Reader();

  static corgi::Status Create(const M3U8Options& options, std::unique_ptr<M3U8Reader>* result);

  virtual corgi::Status ReadLine(std::string *result) = 0;

 protected:
  M3U8Options options_;
  M3U8Reader(const M3U8Options& options);

 private:
  CG_DISALLOW_COPY_AND_ASSIGN(M3U8Reader);
};

} // namespace mms

#endif //MEDIA_MANAGE_SYSTEM_M3U8_READER_H
