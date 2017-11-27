//
// Created by wangqixiang on 2017/8/10.
//

#ifndef MPR_CHARGE_SYSTEM_URL_CODING_H
#define MPR_CHARGE_SYSTEM_URL_CODING_H

#include <string>

namespace corgi {

class URLCoding {
 public:
  static std::string Encode(const std::string& str);
  static std::string Decode(const std::string& str);
};

} // namespace URLCoding 

#endif //MPR_CHARGE_SYSTEM_URL_CODING_H
