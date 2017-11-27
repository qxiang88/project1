//
// Created by wangqixiang on 2017/11/21.
//

#include "util/time_util.h"
#include "corgi/env/env.h"
#include <time.h>

namespace mms {


std::string TimeUtil::FormatNow() {
  char buf[64] = {0};
  corgi::uint64 now = corgi::Env::Default()->NowSeconds();
  tm* tm1 = localtime(reinterpret_cast<time_t *>(&now));
  strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tm1);
  return buf;
}

} // namespace mms
