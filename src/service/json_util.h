//
// Created by xiang88 on 11/21/17.
//

#ifndef MEDIA_MANAGE_SYSTEM_JSON_UTIL_H
#define MEDIA_MANAGE_SYSTEM_JSON_UTIL_H

#include "corgi/base/statusor.h"
#include "corgi/json/value.h"

namespace mms {

corgi::StatusOr<std::shared_ptr<corgi::json::Value>>
Deserilize(const std::string& json_str);

std::string Serialize(corgi::Status status,
                      const corgi::json::Value& value=corgi::json::Value::nullSingleton());

} // namespace mms
#endif //MEDIA_MANAGE_SYSTEM_JSON_UTIL_H
