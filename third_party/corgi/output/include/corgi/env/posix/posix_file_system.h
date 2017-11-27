
#ifndef CORGI_PLATFORM_POSIX_POSIX_FILE_SYSTEM_H_
#define CORGI_PLATFORM_POSIX_POSIX_FILE_SYSTEM_H_

#include "corgi/io/path.h"
#include "corgi/env/env.h"

namespace corgi {

class PosixFileSystem : public FileSystem {
 public:
  PosixFileSystem() {}

  ~PosixFileSystem() {}

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

  Status Stat(const string& fname, FileStatistics* stats) override;

  Status DeleteFile(const string& fname) override;

  Status CreateDir(const string& name) override;

  Status DeleteDir(const string& name) override;

  Status GetFileSize(const string& fname, uint64* size) override;

  Status RenameFile(const string& src, const string& target) override;
};

Status IOError(const string& context, int err_number);

class LocalPosixFileSystem : public PosixFileSystem {
 public:
  string TranslateName(const string& name) const override {
    Slice scheme, host, path;
    io::ParseURI(name, &scheme, &host, &path);
    return path.ToString();
  }
};

}  // namespace corgi

#endif  // CORGI_PLATFORM_POSIX_POSIX_FILE_SYSTEM_H_
