
#ifndef CORGI_PLATFORM_ENV_H_
#define CORGI_PLATFORM_ENV_H_

#include <stdint.h>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "corgi/base/errors.h"
#include "corgi/base/status.h"
#include "corgi/base/slice.h"
#include "corgi/base/macros.h"
#include "corgi/base/types.h"

#include "corgi/env/env_time.h"
#include "corgi/env/file_system.h"

#include "corgi/synchronization/mutex.h"

namespace corgi {

class Thread;
struct ThreadOptions;

class Env {
 public:
  Env();
  virtual ~Env() = default;

  static Env* Default();

  virtual Status GetFileSystemForFile(const string& fname, FileSystem** result);

  virtual Status GetRegisteredFileSystemSchemes(std::vector<string>* schemes);

  virtual Status RegisterFileSystem(const string& scheme,
                                    FileSystemRegistry::Factory factory);

  Status NewRandomAccessFile(const string& fname,
                             std::unique_ptr<RandomAccessFile>* result);

  Status NewWritableFile(const string& fname,
                         std::unique_ptr<WritableFile>* result);

  Status NewAppendableFile(const string& fname,
                           std::unique_ptr<WritableFile>* result);

  Status NewReadOnlyMemoryRegionFromFile(
      const string& fname, std::unique_ptr<ReadOnlyMemoryRegion>* result);

  Status FileExists(const string& fname);

  bool FilesExist(const std::vector<string>& files,
                  std::vector<Status>* status);

  Status GetChildren(const string& dir, std::vector<string>* result);

  virtual bool MatchPath(const string& path, const string& pattern) = 0;

  virtual Status GetMatchingPaths(const string& pattern,
                                  std::vector<string>* results);

  Status DeleteFile(const string& fname);

  Status DeleteRecursively(const string& dirname, int64* undeleted_files,
                           int64* undeleted_dirs);

  Status RecursivelyCreateDir(const string& dirname);

  Status CreateDir(const string& dirname);

  Status DeleteDir(const string& dirname);

  Status Stat(const string& fname, FileStatistics* stat);

  Status IsDirectory(const string& fname);

  Status GetFileSize(const string& fname, uint64* file_size);

  Status RenameFile(const string& src, const string& target);

  string GetExecutablePath();

  bool LocalTempFilename(string* filename);

  virtual uint64 NowMicros() { return envTime->NowMicros(); };

  virtual uint64 NowSeconds() { return envTime->NowSeconds(); }

  virtual void SleepForMicroseconds(int64 micros) = 0;

  virtual Thread* StartThread(const ThreadOptions& thread_options,
                              const string& name,
                              std::function<void()> fn) CG_MUST_USE_RESULT = 0;

  virtual void SchedClosure(std::function<void()> closure) = 0;

  virtual void SchedClosureAfter(int64 micros,
                                 std::function<void()> closure) = 0;

  virtual Status LoadLibrary(const char* library_filename, void** handle) = 0;

  virtual Status GetSymbolFromLibrary(void* handle, const char* symbol_name,
                                      void** symbol) = 0;

  virtual string FormatLibraryFileName(const string& name,
      const string& version) = 0;

 private:
  void GetLocalTempDirectories(std::vector<string>* list);

  std::unique_ptr<FileSystemRegistry> file_system_registry_;
  CG_DISALLOW_COPY_AND_ASSIGN(Env);
  EnvTime* envTime = EnvTime::Default();
};

class EnvWrapper : public Env {
 public:
  /// Initializes an EnvWrapper that delegates all calls to *t
  explicit EnvWrapper(Env* t) : target_(t) {}
  virtual ~EnvWrapper();

  /// Returns the target to which this Env forwards all calls
  Env* target() const { return target_; }

  Status GetFileSystemForFile(const string& fname,
                              FileSystem** result) override {
    return target_->GetFileSystemForFile(fname, result);
  }

  Status GetRegisteredFileSystemSchemes(std::vector<string>* schemes) override {
    return target_->GetRegisteredFileSystemSchemes(schemes);
  }

  Status RegisterFileSystem(const string& scheme,
                            FileSystemRegistry::Factory factory) override {
    return target_->RegisterFileSystem(scheme, factory);
  }

  bool MatchPath(const string& path, const string& pattern) override {
    return target_->MatchPath(path, pattern);
  }

  uint64 NowMicros() override { return target_->NowMicros(); }
  void SleepForMicroseconds(int64 micros) override {
    target_->SleepForMicroseconds(micros);
  }
  Thread* StartThread(const ThreadOptions& thread_options, const string& name,
                      std::function<void()> fn) override {
    return target_->StartThread(thread_options, name, fn);
  }
  void SchedClosure(std::function<void()> closure) override {
    target_->SchedClosure(closure);
  }
  void SchedClosureAfter(int64 micros, std::function<void()> closure) override {
    target_->SchedClosureAfter(micros, closure);
  }
  Status LoadLibrary(const char* library_filename, void** handle) override {
    return target_->LoadLibrary(library_filename, handle);
  }
  Status GetSymbolFromLibrary(void* handle, const char* symbol_name,
                              void** symbol) override {
    return target_->GetSymbolFromLibrary(handle, symbol_name, symbol);
  }
  string FormatLibraryFileName(const string& name,
                               const string& version) override {
    return target_->FormatLibraryFileName(name, version);
  }
 private:
  Env* target_;
};

/// Represents a thread used to run a function.
class Thread {
 public:
  Thread() {}

  /// Blocks until the thread of control stops running.
  virtual ~Thread();

 private:
  CG_DISALLOW_COPY_AND_ASSIGN(Thread);
};

/// \brief Options to configure a Thread.
///
/// Note that the options are all hints, and the
/// underlying implementation may choose to ignore it.
struct ThreadOptions {
  /// Thread stack size to use (in bytes).
  size_t stack_size = 0;  // 0: use system default value
  /// Guard area size to use near thread stacks to use (in bytes)
  size_t guard_size = 0;  // 0: use system default value
};

/// A utility routine: reads contents of named file into `*data`
Status ReadFileToString(Env* env, const string& fname, string* data);

/// A utility routine: write contents of `data` to file named `fname`
/// (overwriting existing contents, if any).
Status WriteStringToFile(Env* env, const string& fname,
                         const Slice& data);

// START_SKIP_DOXYGEN

namespace register_file_system {

template <typename Factory>
struct Register {
  Register(Env* env, const string& scheme) {
    env->RegisterFileSystem(scheme, []() -> FileSystem* { return new Factory; })
        .IgnoreError();
  }
};

}  // namespace register_file_system

// END_SKIP_DOXYGEN

}  // namespace corgi

#define REGISTER_FILE_SYSTEM_ENV(env, scheme, factory) \
  REGISTER_FILE_SYSTEM_UNIQ_HELPER(__COUNTER__, env, scheme, factory)
#define REGISTER_FILE_SYSTEM_UNIQ_HELPER(ctr, env, scheme, factory) \
  REGISTER_FILE_SYSTEM_UNIQ(ctr, env, scheme, factory)
#define REGISTER_FILE_SYSTEM_UNIQ(ctr, env, scheme, factory)   \
  static ::corgi::register_file_system::Register<factory> \
      register_ff##ctr CG_ATTRIBUTE_UNUSED =                   \
          ::corgi::register_file_system::Register<factory>(env, scheme)

#define REGISTER_FILE_SYSTEM(scheme, factory) \
  REGISTER_FILE_SYSTEM_ENV(Env::Default(), scheme, factory);

#endif  // CORGI_PLATFORM_ENV_H_
