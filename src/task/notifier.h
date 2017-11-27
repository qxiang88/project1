//
// Created by wangqixiang on 2017/11/23.
//

#ifndef MEDIA_MANAGE_SYSTEM_NOTIFY_H
#define MEDIA_MANAGE_SYSTEM_NOTIFY_H

#include "corgi/base/status.h"
#include "task/task_metadata.h"
#include "corgi/json/json.h"

#include <memory>

namespace mms {

class Notifier {
 public:
  virtual ~Notifier();
  static corgi::Status Create(const std::string& url,
                              std::unique_ptr<Notifier>* result);

  corgi::Status Serialize(const corgi::json::Value& v, std::string* result);
  corgi::Status NotifyTaskState(const std::string& uuid,
                                TaskState task_state);

 protected:
  virtual corgi::Status Notify(const std::string& data) = 0;

 protected:
  std::string url_;
  Notifier(const std::string& url);
};

} // namespace mms

#endif //MEDIA_MANAGE_SYSTEM_NOTIFY_H
