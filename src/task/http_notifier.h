//
// Created by wangqixiang on 2017/11/23.
//

#ifndef MEDIA_MANAGE_SYSTEM_HTTP_NOTIFIER_H
#define MEDIA_MANAGE_SYSTEM_HTTP_NOTIFIER_H

#include "task/notifier.h"

namespace mms {

class HttpNotifier : public Notifier {
 public:
  HttpNotifier(const std::string& url);

  virtual corgi::Status Notify(const std::string& data) override;

};

} // namespace mms
#endif //MEDIA_MANAGE_SYSTEM_HTTP_NOTIFIER_H
