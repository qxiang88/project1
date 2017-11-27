//
// Created by wangqixiang on 2017/11/22.
//

#include "util/m3u8_reader.h"
#include "corgi/env/env.h"
#include "corgi/io/inputbuffer.h"

namespace mms {

class M3U8EnvReader : public M3U8Reader {
 public:
  M3U8EnvReader(const M3U8Options& options)
      : M3U8Reader(options) {}

  corgi::Status Init() {
    CG_RETURN_IF_ERROR(corgi::Env::Default()->NewRandomAccessFile(options_.m3u8_path_, &file_));
    input_buffer_.reset(new corgi::io::InputBuffer(file_.get(), kBufferSize));
    return corgi::Status::OK();
  }

  virtual corgi::Status ReadLine(std::string *result) override;

 private:
  static const size_t kBufferSize = 1024;
  std::unique_ptr<corgi::RandomAccessFile> file_;
  std::unique_ptr<corgi::io::InputBuffer> input_buffer_;
};

const size_t M3U8EnvReader::kBufferSize;

corgi::Status M3U8EnvReader::ReadLine(std::string *result) {
  return input_buffer_->ReadLine(result);
}

///// M3U8Reader

M3U8Reader::M3U8Reader(const M3U8Options &options)
    : options_(options) {}

M3U8Reader::~M3U8Reader() {}

// static
corgi::Status M3U8Reader::Create(const M3U8Options &options,
                                 std::unique_ptr<M3U8Reader> *result) {
  M3U8EnvReader* r = new M3U8EnvReader(options);
  corgi::Status s = r->Init();
  if (!s.ok()) {
    delete r;
    return s;
  }
  result->reset(r);
  return corgi::Status::OK();
}

} // namespace mms