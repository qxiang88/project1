//
// Created by wangqixiang on 2017/11/23.
//

#include "task/notifier.h"
#include "corgi/base/errors.h"
#include "corgi/gtl/stl_util.h"
#include "corgi/strings/str_util.h"
#include "http_notifier.h"

namespace mms {

Notifier::Notifier(const std::string& url)
    : url_(url) {}

Notifier::~Notifier() {}

corgi::Status Notifier::Serialize(const corgi::json::Value &v,
                                  std::string* result) {
  corgi::json::StreamWriterBuilder builder;
  std::unique_ptr<corgi::json::StreamWriter>
      writer(builder.newStreamWriter());
  std::ostringstream os;
  int r = writer->write(v, &os);
  if (r != 0) {
    return corgi::errors::Internal("Notifier::Serialize got wrong");
  }
  std::string str = os.str();
  corgi::gtl::STLStringResizeUninitialized(result, str.size());
  result->swap(str);
  return corgi::Status::OK();
}

corgi::Status Notifier::NotifyTaskState(const std::string &uuid,
                                        TaskState task_state) {
  std::string data;
  corgi::json::Value root;
  root["uuid"] = uuid;
  root["state"] = task_state;
  CG_RETURN_IF_ERROR(Serialize(root, &data));
  return Notify(data);
}

corgi::Status Notifier::Create(const std::string &url,
                               std::unique_ptr<Notifier> *result) {
  std::string lower_case_url = corgi::str_util::Lowercase(url);
  corgi::Slice slice(lower_case_url);
  if (slice.starts_with("http")) {
    HttpNotifier* notifier = new HttpNotifier(url);
    result->reset(notifier);
  } else {
    return corgi::errors::Unimplemented("Scheme unsupported ", url);
  }
  return corgi::Status::OK();
}

} // namespace mms

