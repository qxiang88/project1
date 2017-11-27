//
// Created by xiang88 on 11/21/17.
//

#include "corgi/json/json.h"
#include "corgi/base/errors.h"
#include "service/json_util.h"

namespace mms {

corgi::StatusOr<std::shared_ptr<corgi::json::Value>>
Deserilize(const std::string& json_str) {
  corgi::json::CharReaderBuilder builder;
  std::unique_ptr<corgi::json::CharReader> reader(builder.newCharReader());
  std::shared_ptr<corgi::json::Value> r(new corgi::json::Value);
  std::string err;

  bool b = reader->parse(json_str.data(),
                         json_str.data() + json_str.size(),
                         r.get(), &err);
  if (!b) {
    return corgi::errors::InvalidArgument("json error: ", err);
  }
  return r;
}

std::string Serialize(corgi::Status status,
                      const corgi::json::Value& value) {
  corgi::json::StreamWriterBuilder builder;
  const std::unique_ptr<corgi::json::StreamWriter>
    writer(builder.newStreamWriter());
  corgi::json::Value root;
  std::ostringstream os;

  root["return_code"] = status.code();
  root["return_status"] = status.ToString();
  root["data"] = value;
  writer->write(root, &os);
  return os.str();
}

} // namespace mms
