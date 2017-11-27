//
// Created by wangqixiang on 2017/11/14.
//

#include "crypto/crypto_util.h"
#include "corgi/base/errors.h"
#include "corgi/gtl/stl_util.h"

#include <openssl/rand.h>

namespace mms {

namespace {

const EVP_CIPHER *GetCBCCipherByKey(const std::string &key) {
  switch (key.length()) {
    case kAES128KeySize:
      return EVP_aes_128_cbc();
    case kAES256KeySize:
      return EVP_aes_256_cbc();
    default:
      return nullptr;
  }
}

class ScopedCipherCTX {
 public:
  explicit ScopedCipherCTX() {
    EVP_CIPHER_CTX_init(&ctx_);
  }

  ~ScopedCipherCTX() {
    EVP_CIPHER_CTX_cleanup(&ctx_);
    //TODO(xiang88):
    //ClearOpenSSLERRStack(FROM_HERE);
  }

  EVP_CIPHER_CTX *get() { return &ctx_; }

 private:
  EVP_CIPHER_CTX ctx_;
};

enum CryptoModel {
  kDecrypt = 0,
  kEncrypt = 1,
};

corgi::Status Crypt(CryptoModel model,
           const std::string &raw_key,
           const EVP_CIPHER *cipher,
           const corgi::Slice &input,
           std::string *output,
           const std::string& iv = "",
           bool do_padding = true) {
  DCHECK(!raw_key.empty());
  DCHECK(cipher);

  std::string result;
  output->clear();
  bool do_cbc = false;

  if (!iv.empty() && iv.size() == 16) { // CBC
    do_cbc = true;
    DCHECK_EQ(EVP_CIPHER_iv_length(cipher), iv.length());
  } else {
    do_cbc = false;
  }

  ScopedCipherCTX ctx;
  if (!EVP_CipherInit_ex(ctx.get(),
                         cipher,
                         nullptr,
                         reinterpret_cast<const uint8_t *>(raw_key.data()),
                         do_cbc ? reinterpret_cast<const uint8_t *>(iv.data()) : nullptr,
                         model == kEncrypt)) {
    return corgi::errors::Internal("EVP_CipherInit_ex: error");
  }

  // When padding && encrypting, add other block size of space to allow for any padding.
  const size_t output_size = input.size() + (do_padding ? (model == kEncrypt ? kAESBlockSize : 0)
                                                        : 0);
  CHECK_GT(output_size, 0u);
  CHECK_GT(output_size + 1, input.size());
  corgi::gtl::STLStringResizeUninitialized(&result, output_size + 1);
  uint8_t *out_ptr = reinterpret_cast<uint8_t *>(const_cast<char *>(result.data()));
  int out_len;
  if (!EVP_CipherUpdate(ctx.get(),
                        out_ptr,
                        &out_len,
                        reinterpret_cast<const uint8_t *>(input.data()),
                        (int) input.size())) {
    return corgi::errors::Internal("EVP_CipherUpdate: error");
  }

  // Write out the final block plus padding (if any) to the end of the data
  // just written.
  int tail_len;
  if (!EVP_CipherFinal_ex(ctx.get(), out_ptr + out_len, &tail_len))
    return corgi::errors::Internal("EVP_CipherFinal_ex: error");

  out_len += tail_len;
  DCHECK_LE(out_len, static_cast<int>(output_size));
  result.resize(out_len);

  output->swap(result);
  return corgi::Status::OK();
}


} // namespace

////////////////////////////

corgi::Status GenerateAESKey(int key_size, std::string *result) {
  if (key_size != kAES128KeySize && key_size != kAES256KeySize) {
    return corgi::errors::InvalidArgument("key size not supported: ", key_size);
  }
  std::unique_ptr<unsigned char> buf(new unsigned char[key_size]);
  memset(buf.get(), 0, key_size);
  int rc = RAND_bytes(buf.get(), key_size);
  if (rc != 1) {
    return corgi::errors::Internal("RAND_bytes error");
  }
  corgi::gtl::STLStringResizeUninitialized(result, key_size);
  result->assign(buf.get(), buf.get() + key_size);
  return corgi::Status::OK();
}

bool RandBytes(unsigned char* buf, size_t size) {
  return RAND_bytes(buf, size) == 1;
}

corgi::Status GenerateIV(std::string* result) {
  return GenerateAESKey(kAESBlockSize, result);
}

corgi::Status AESCBCEncrypt(const std::string &key,
                            const corgi::Slice &input,
                            std::string *output,
                            const std::string &iv,
                            bool do_padding) {
  return Crypt(kEncrypt, key, GetCBCCipherByKey(key),
               input, output, iv, do_padding);
}

corgi::Status AESCBCDecrypt(const std::string &key,
                            const corgi::Slice &input,
                            std::string *output,
                            const std::string &iv) {
  return Crypt(kDecrypt, key, GetCBCCipherByKey(key),
               input, output, iv);
}

} // namespace mms
