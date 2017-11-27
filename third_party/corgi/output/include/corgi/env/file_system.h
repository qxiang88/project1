
#ifndef CORGI_ENV_FILE_SYSTEM_H_
#define CORGI_ENV_FILE_SYSTEM_H_

#include <stdint.h>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

#include "corgi/base/errors.h"
#include "corgi/base/status.h"
#include "corgi/base/slice.h"
#include "corgi/base/macros.h"
#include "corgi/base/types.h"

#include "corgi/env/file_statistics.h"

#ifdef PLATFORM_WINDOWS
#undef DeleteFile
#endif

namespace corgi {

class RandomAccessFile;
class ReadOnlyMemoryRegion;
class WritableFile;

class FileSystem {
 public:
  virtual Status NewRandomAccessFile(
      const string& fname, std::unique_ptr<RandomAccessFile>* result) = 0;

  virtual Status NewWritableFile(const string& fname,
                                 std::unique_ptr<WritableFile>* result) = 0;

  virtual Status NewAppendableFile(const string& fname,
                                   std::unique_ptr<WritableFile>* result) = 0;

  virtual Status NewReadOnlyMemoryRegionFromFile(
      const string& fname, std::unique_ptr<ReadOnlyMemoryRegion>* result) = 0;

  virtual Status FileExists(const string& fname) = 0;

  virtual bool FilesExist(const std::vector<string>& files,
                          std::vector<Status>* status);

  virtual Status GetChildren(const string& dir,
                             std::vector<string>* result) = 0;

  virtual Status GetMatchingPaths(const string& pattern,
                                  std::vector<string>* results);

  virtual Status Stat(const string& fname, FileStatistics* stat) = 0;

  virtual Status DeleteFile(const string& fname) = 0;

  virtual Status CreateDir(const string& dirname) = 0;

  virtual Status RecursivelyCreateDir(const string& dirname);

  virtual Status DeleteDir(const string& dirname) = 0;

  virtual Status DeleteRecursively(const string& dirname,
                                   int64* undeleted_files,
                                   int64* undeleted_dirs);

  virtual Status GetFileSize(const string& fname, uint64* file_size) = 0;

  virtual Status RenameFile(const string& src, const string& target) = 0;

  virtual string TranslateName(const string& name) const;

  virtual Status IsDirectory(const string& fname);

  FileSystem() {}

  virtual ~FileSystem();
};

class RandomAccessFile {
 public:
  RandomAccessFile() {}
  virtual ~RandomAccessFile();

  virtual Status Read(uint64 offset, size_t n, 
                      Slice* result,
                      char* scratch) const = 0;

 private:
  CG_DISALLOW_COPY_AND_ASSIGN(RandomAccessFile);
};

class WritableFile {
 public:
  WritableFile() {}
  virtual ~WritableFile();

  virtual Status Append(const Slice& data) = 0;

  virtual Status Close() = 0;

  virtual Status Flush() = 0;

  virtual Status Sync() = 0;

 private:
  CG_DISALLOW_COPY_AND_ASSIGN(WritableFile);
};

class ReadOnlyMemoryRegion {
 public:
  ReadOnlyMemoryRegion() {}
  virtual ~ReadOnlyMemoryRegion() = default;

  virtual const void* data() = 0;

  virtual uint64 length() = 0;
};


#ifndef SWIG
class NullFileSystem : public FileSystem {
 public:
  NullFileSystem() {}

  ~NullFileSystem() override = default;

  Status NewRandomAccessFile(
      const string& fname, 
	  std::unique_ptr<RandomAccessFile>* result) override {
    (void)fname;
	(void)result;
    return errors::Unimplemented("NewRandomAccessFile unimplemented");
  }

  Status NewWritableFile(const string& fname,
                         std::unique_ptr<WritableFile>* result) override {
    (void)fname;
	(void)result;
    return errors::Unimplemented("NewWritableFile unimplemented");
  }

  Status NewAppendableFile(const string& fname,
                           std::unique_ptr<WritableFile>* result) override {
    (void)fname;
	(void)result;
    return errors::Unimplemented("NewAppendableFile unimplemented");
  }

  Status NewReadOnlyMemoryRegionFromFile(
      const string& fname,
      std::unique_ptr<ReadOnlyMemoryRegion>* result) override {
    (void)fname;
	(void)result;
    return errors::Unimplemented(
        "NewReadOnlyMemoryRegionFromFile unimplemented");
  }

  Status FileExists(const string& fname) override {
    (void)fname;
    return errors::Unimplemented("FileExists unimplemented");
  }

  Status GetChildren(const string& dir, std::vector<string>* result) override {
    (void)dir;
	(void)result;
    return errors::Unimplemented("GetChildren unimplemented");
  }

  Status DeleteFile(const string& fname) override {
    (void)fname;
    return errors::Unimplemented("DeleteFile unimplemented");
  }

  Status CreateDir(const string& dirname) override {
    (void)dirname;
    return errors::Unimplemented("CreateDir unimplemented");
  }

  Status DeleteDir(const string& dirname) override {
    (void)dirname;
    return errors::Unimplemented("DeleteDir unimplemented");
  }

  Status GetFileSize(const string& fname, uint64* file_size) override {
    (void)fname;
	(void)file_size;
    return errors::Unimplemented("GetFileSize unimplemented");
  }

  Status RenameFile(const string& src, const string& target) override {
    (void)src;
	(void)target;
    return errors::Unimplemented("RenameFile unimplemented");
  }

  Status Stat(const string& fname, FileStatistics* stat) override {
    (void)fname;
	(void)stat;
    return errors::Unimplemented("Stat unimplemented");
  }
};
#endif

class FileSystemRegistry {
 public:
  typedef std::function<FileSystem*()> Factory;

  virtual ~FileSystemRegistry();
  virtual Status Register(const string& scheme, Factory factory) = 0;
  virtual FileSystem* Lookup(const string& scheme) = 0;
  virtual Status GetRegisteredFileSystemSchemes(
      std::vector<string>* schemes) = 0;
};

}  // namespace corgi

#endif  // CORGI_ENV_FILE_SYSTEM_H_
