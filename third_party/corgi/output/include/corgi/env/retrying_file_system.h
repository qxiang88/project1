
#ifndef CORGI_ENV_RETRYING_FILE_SYSTEM_H_
#define CORGI_ENV_RETRYING_FILE_SYSTEM_H_

#include <string>
#include <vector>

#include "corgi/base/status.h"
#include "corgi/env/file_system.h"

namespace corgi {

/// A wrapper to add retry logic to another file system.
class RetryingFileSystem : public FileSystem {
 public:
  RetryingFileSystem(std::unique_ptr<FileSystem> base_file_system,
                     int64 delay_microseconds = 1000000)
      : base_file_system_(std::move(base_file_system)),
        initial_delay_microseconds_(delay_microseconds) {}

  Status NewRandomAccessFile(
      const string& filename,
      std::unique_ptr<RandomAccessFile>* result) override;

  Status NewWritableFile(const string& fname,
                         std::unique_ptr<WritableFile>* result) override;

  Status NewAppendableFile(const string& fname,
                           std::unique_ptr<WritableFile>* result) override;

  Status NewReadOnlyMemoryRegionFromFile(
      const string& filename,
      std::unique_ptr<ReadOnlyMemoryRegion>* result) override;

  Status FileExists(const string& fname) override;

  Status GetChildren(const string& dir, std::vector<string>* result) override;

  Status GetMatchingPaths(const string& dir,
                          std::vector<string>* result) override;

  Status Stat(const string& fname, FileStatistics* stat) override;

  Status DeleteFile(const string& fname) override;

  Status CreateDir(const string& dirname) override;

  Status DeleteDir(const string& dirname) override;

  Status GetFileSize(const string& fname, uint64* file_size) override;

  Status RenameFile(const string& src, const string& target) override;

  Status IsDirectory(const string& dir) override;

  Status DeleteRecursively(const string& dirname, int64* undeleted_files,
                           int64* undeleted_dirs) override;

 private:
  std::unique_ptr<FileSystem> base_file_system_;
  const int64 initial_delay_microseconds_;

  CG_DISALLOW_COPY_AND_ASSIGN(RetryingFileSystem);
};

}  // namespace corgi

#endif  // CORGI_ENV_RETRYING_FILE_SYSTEM_H_
