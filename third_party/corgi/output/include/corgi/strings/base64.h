//
// Created by wangqixiang on 2017/8/11.
//

#ifndef MPR_CHARGE_SYSTEM_BASE64_H
#define MPR_CHARGE_SYSTEM_BASE64_H

#include <stdlib.h>
#include <vector>

#include "corgi/base/slice.h"
#include "corgi/base/status.h"

namespace corgi {

Status Base64Encode(Slice src, std::string* dest);
Status Base64Decode(Slice src, std::string* dest);

} // namespace mcs
#endif //MPR_CHARGE_SYSTEM_BASE64_H
