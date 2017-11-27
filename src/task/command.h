//
// Created by wangqixiang on 2017/11/18.
//

#ifndef MEDIA_MANAGE_SYSTEM_COMMAND_H
#define MEDIA_MANAGE_SYSTEM_COMMAND_H

#include <vector>
#include <string>

namespace mms {

class Command {
  friend class CommandBuilder;
 public:
  Command();

  std::vector<std::string> ToVector() const {
    return command_vec_;
  }
  std::string ToString() const {
    return command_;
  }

  std::string GetExecutablePath() const {
    return command_executable_path_;
  }

 private:
  std::string command_executable_path_;
  std::vector<std::string> command_vec_;
  std::string command_;
};

} // namespace mms
#endif //MEDIA_MANAGE_SYSTEM_COMMAND_H
