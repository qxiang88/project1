//
// Created by wangqixiang on 2017/12/2.
//

#include "util/directory_sizer.h"
#include "corgi/env/env.h"
#include "corgi/io/path.h"

#include <stack>

namespace mms {


DirectorySizer::DirectorySizer(const std::string &path)
    : path_(path),
      pattern_("") {}

DirectorySizer::DirectorySizer(const std::string &path,
                             corgi::Slice pattern)
    : path_(path),
      pattern_(pattern.ToString()) {}


corgi::Status DirectorySizer::DoComputeSize(bool recurse,
                                           uint64_t *result) {
  uint64_t total_size = 0;
  std::stack<std::string> children;
  children.push(path_);
  corgi::Status s;

  // 广度遍历求出目录匹配文件的总大小
  do {
    std::vector<std::string> sub_files;
    std::string current_dir = children.top();
    children.pop();

    CG_RETURN_IF_ERROR(corgi::Env::Default()->GetChildren(current_dir, &sub_files));
    for (auto& p : sub_files) {
      std::string absolute_path = corgi::io::JoinPath(current_dir, p);
      s = corgi::Env::Default()->IsDirectory(absolute_path);
      corgi::uint64 size = 0;

      bool is_dir = s.ok();
      if (recurse && is_dir) {
        children.push(absolute_path);
      }
      if (!is_dir) {
        if (!pattern_.empty() && !corgi::Env::Default()->MatchPath(p, pattern_)) {
          ; //CG_RETURN_IF_ERROR(corgi::Env::Default()->GetFileSize(absolute_path, &size));
        } else {
          CG_RETURN_IF_ERROR(corgi::Env::Default()->GetFileSize(absolute_path, &size));
        }
        total_size += size;
      }
    }
  } while (recurse && !children.empty());

  *result = total_size;
  return corgi::Status::OK();
}

corgi::Status DirectorySizer::ComputeSize(uint64_t *result) {
  return DoComputeSize(false, result);
}

corgi::Status DirectorySizer::ComputeSizeRecursively(uint64_t *result) {
  return DoComputeSize(true, result);
}

} // namespace mms

