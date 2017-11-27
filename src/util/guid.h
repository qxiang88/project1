//
// Created by wangqixiang on 2017/11/18.
//

#ifndef MEDIA_MANAGE_SYSTEM_GUID_H
#define MEDIA_MANAGE_SYSTEM_GUID_H

#include <string>

#include "corgi/base/slice.h"

namespace mms {

std::string GenerateGUID();
bool IsValidGUID(const corgi::Slice& guid);
bool IsValidGUIDOutputString(const corgi::Slice& guid);
std::string RandomDataToGUIDString(const corgi::uint64 bytes[2]);

} // namespace mms
#endif //MEDIA_MANAGE_SYSTEM_GUID_H
