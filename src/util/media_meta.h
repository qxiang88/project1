//
// Created by wangqixiang on 2017/12/20.
//

#ifndef MEDIA_MANAGE_SYSTEM_MEDIA_INFO_H
#define MEDIA_MANAGE_SYSTEM_MEDIA_INFO_H

#include "corgi/base/status.h"
#include <memory>

namespace mms {

class MediaMeta {
 public:
  explicit MediaMeta(const std::string& path);
  ~MediaMeta();

  corgi::Status Init();

  int64_t GetVideoWidth() const;
  int64_t GetVideoHeight() const;
  double GetVideoAspect() const;
  bool IsVerticalScreen() const;

  std::string GetVideoAspectAsString() const;

  std::string GetDuration() const;
  std::string GetSize() const;

  // TODO(xiang88):
  // 按照业务需求增加需要的 Get* 方法.

 private:
  class Rep;
  std::unique_ptr<Rep> rep_;

  CG_DISALLOW_COPY_AND_ASSIGN(MediaMeta);
};

} // namespace mms
#endif //MEDIA_MANAGE_SYSTEM_MEDIA_INFO_H
