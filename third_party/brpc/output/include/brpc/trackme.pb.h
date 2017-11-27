// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: brpc/trackme.proto

#ifndef PROTOBUF_brpc_2ftrackme_2eproto__INCLUDED
#define PROTOBUF_brpc_2ftrackme_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2006000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2006001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/service.h>
#include <google/protobuf/unknown_field_set.h>
#include "brpc/options.pb.h"
// @@protoc_insertion_point(includes)

namespace brpc {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_brpc_2ftrackme_2eproto();
void protobuf_AssignDesc_brpc_2ftrackme_2eproto();
void protobuf_ShutdownFile_brpc_2ftrackme_2eproto();

class TrackMeRequest;
class TrackMeResponse;

enum TrackMeSeverity {
  TrackMeOK = 0,
  TrackMeWarning = 1,
  TrackMeFatal = 2
};
bool TrackMeSeverity_IsValid(int value);
const TrackMeSeverity TrackMeSeverity_MIN = TrackMeOK;
const TrackMeSeverity TrackMeSeverity_MAX = TrackMeFatal;
const int TrackMeSeverity_ARRAYSIZE = TrackMeSeverity_MAX + 1;

const ::google::protobuf::EnumDescriptor* TrackMeSeverity_descriptor();
inline const ::std::string& TrackMeSeverity_Name(TrackMeSeverity value) {
  return ::google::protobuf::internal::NameOfEnum(
    TrackMeSeverity_descriptor(), value);
}
inline bool TrackMeSeverity_Parse(
    const ::std::string& name, TrackMeSeverity* value) {
  return ::google::protobuf::internal::ParseNamedEnum<TrackMeSeverity>(
    TrackMeSeverity_descriptor(), name, value);
}
// ===================================================================

class TrackMeRequest : public ::google::protobuf::Message {
 public:
  TrackMeRequest();
  virtual ~TrackMeRequest();

  TrackMeRequest(const TrackMeRequest& from);

  inline TrackMeRequest& operator=(const TrackMeRequest& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const TrackMeRequest& default_instance();

  void Swap(TrackMeRequest* other);

  // implements Message ----------------------------------------------

  TrackMeRequest* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const TrackMeRequest& from);
  void MergeFrom(const TrackMeRequest& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional int64 rpc_version = 1;
  inline bool has_rpc_version() const;
  inline void clear_rpc_version();
  static const int kRpcVersionFieldNumber = 1;
  inline ::google::protobuf::int64 rpc_version() const;
  inline void set_rpc_version(::google::protobuf::int64 value);

  // optional string server_addr = 2;
  inline bool has_server_addr() const;
  inline void clear_server_addr();
  static const int kServerAddrFieldNumber = 2;
  inline const ::std::string& server_addr() const;
  inline void set_server_addr(const ::std::string& value);
  inline void set_server_addr(const char* value);
  inline void set_server_addr(const char* value, size_t size);
  inline ::std::string* mutable_server_addr();
  inline ::std::string* release_server_addr();
  inline void set_allocated_server_addr(::std::string* server_addr);

  // @@protoc_insertion_point(class_scope:brpc.TrackMeRequest)
 private:
  inline void set_has_rpc_version();
  inline void clear_has_rpc_version();
  inline void set_has_server_addr();
  inline void clear_has_server_addr();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::google::protobuf::int64 rpc_version_;
  ::std::string* server_addr_;
  friend void  protobuf_AddDesc_brpc_2ftrackme_2eproto();
  friend void protobuf_AssignDesc_brpc_2ftrackme_2eproto();
  friend void protobuf_ShutdownFile_brpc_2ftrackme_2eproto();

  void InitAsDefaultInstance();
  static TrackMeRequest* default_instance_;
};
// -------------------------------------------------------------------

class TrackMeResponse : public ::google::protobuf::Message {
 public:
  TrackMeResponse();
  virtual ~TrackMeResponse();

  TrackMeResponse(const TrackMeResponse& from);

  inline TrackMeResponse& operator=(const TrackMeResponse& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const TrackMeResponse& default_instance();

  void Swap(TrackMeResponse* other);

  // implements Message ----------------------------------------------

  TrackMeResponse* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const TrackMeResponse& from);
  void MergeFrom(const TrackMeResponse& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional .brpc.TrackMeSeverity severity = 1;
  inline bool has_severity() const;
  inline void clear_severity();
  static const int kSeverityFieldNumber = 1;
  inline ::brpc::TrackMeSeverity severity() const;
  inline void set_severity(::brpc::TrackMeSeverity value);

  // optional string error_text = 2;
  inline bool has_error_text() const;
  inline void clear_error_text();
  static const int kErrorTextFieldNumber = 2;
  inline const ::std::string& error_text() const;
  inline void set_error_text(const ::std::string& value);
  inline void set_error_text(const char* value);
  inline void set_error_text(const char* value, size_t size);
  inline ::std::string* mutable_error_text();
  inline ::std::string* release_error_text();
  inline void set_allocated_error_text(::std::string* error_text);

  // optional int32 new_interval = 3;
  inline bool has_new_interval() const;
  inline void clear_new_interval();
  static const int kNewIntervalFieldNumber = 3;
  inline ::google::protobuf::int32 new_interval() const;
  inline void set_new_interval(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:brpc.TrackMeResponse)
 private:
  inline void set_has_severity();
  inline void clear_has_severity();
  inline void set_has_error_text();
  inline void clear_has_error_text();
  inline void set_has_new_interval();
  inline void clear_has_new_interval();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::std::string* error_text_;
  int severity_;
  ::google::protobuf::int32 new_interval_;
  friend void  protobuf_AddDesc_brpc_2ftrackme_2eproto();
  friend void protobuf_AssignDesc_brpc_2ftrackme_2eproto();
  friend void protobuf_ShutdownFile_brpc_2ftrackme_2eproto();

  void InitAsDefaultInstance();
  static TrackMeResponse* default_instance_;
};
// ===================================================================

class TrackMeService_Stub;

class TrackMeService : public ::google::protobuf::Service {
 protected:
  // This class should be treated as an abstract interface.
  inline TrackMeService() {};
 public:
  virtual ~TrackMeService();

  typedef TrackMeService_Stub Stub;

  static const ::google::protobuf::ServiceDescriptor* descriptor();

  virtual void TrackMe(::google::protobuf::RpcController* controller,
                       const ::brpc::TrackMeRequest* request,
                       ::brpc::TrackMeResponse* response,
                       ::google::protobuf::Closure* done);

  // implements Service ----------------------------------------------

  const ::google::protobuf::ServiceDescriptor* GetDescriptor();
  void CallMethod(const ::google::protobuf::MethodDescriptor* method,
                  ::google::protobuf::RpcController* controller,
                  const ::google::protobuf::Message* request,
                  ::google::protobuf::Message* response,
                  ::google::protobuf::Closure* done);
  const ::google::protobuf::Message& GetRequestPrototype(
    const ::google::protobuf::MethodDescriptor* method) const;
  const ::google::protobuf::Message& GetResponsePrototype(
    const ::google::protobuf::MethodDescriptor* method) const;

 private:
  GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(TrackMeService);
};

class TrackMeService_Stub : public TrackMeService {
 public:
  TrackMeService_Stub(::google::protobuf::RpcChannel* channel);
  TrackMeService_Stub(::google::protobuf::RpcChannel* channel,
                   ::google::protobuf::Service::ChannelOwnership ownership);
  ~TrackMeService_Stub();

  inline ::google::protobuf::RpcChannel* channel() { return channel_; }

  // implements TrackMeService ------------------------------------------

  void TrackMe(::google::protobuf::RpcController* controller,
                       const ::brpc::TrackMeRequest* request,
                       ::brpc::TrackMeResponse* response,
                       ::google::protobuf::Closure* done);
 private:
  ::google::protobuf::RpcChannel* channel_;
  bool owns_channel_;
  GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(TrackMeService_Stub);
};


// ===================================================================


// ===================================================================

// TrackMeRequest

// optional int64 rpc_version = 1;
inline bool TrackMeRequest::has_rpc_version() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void TrackMeRequest::set_has_rpc_version() {
  _has_bits_[0] |= 0x00000001u;
}
inline void TrackMeRequest::clear_has_rpc_version() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void TrackMeRequest::clear_rpc_version() {
  rpc_version_ = GOOGLE_LONGLONG(0);
  clear_has_rpc_version();
}
inline ::google::protobuf::int64 TrackMeRequest::rpc_version() const {
  // @@protoc_insertion_point(field_get:brpc.TrackMeRequest.rpc_version)
  return rpc_version_;
}
inline void TrackMeRequest::set_rpc_version(::google::protobuf::int64 value) {
  set_has_rpc_version();
  rpc_version_ = value;
  // @@protoc_insertion_point(field_set:brpc.TrackMeRequest.rpc_version)
}

// optional string server_addr = 2;
inline bool TrackMeRequest::has_server_addr() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void TrackMeRequest::set_has_server_addr() {
  _has_bits_[0] |= 0x00000002u;
}
inline void TrackMeRequest::clear_has_server_addr() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void TrackMeRequest::clear_server_addr() {
  if (server_addr_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    server_addr_->clear();
  }
  clear_has_server_addr();
}
inline const ::std::string& TrackMeRequest::server_addr() const {
  // @@protoc_insertion_point(field_get:brpc.TrackMeRequest.server_addr)
  return *server_addr_;
}
inline void TrackMeRequest::set_server_addr(const ::std::string& value) {
  set_has_server_addr();
  if (server_addr_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    server_addr_ = new ::std::string;
  }
  server_addr_->assign(value);
  // @@protoc_insertion_point(field_set:brpc.TrackMeRequest.server_addr)
}
inline void TrackMeRequest::set_server_addr(const char* value) {
  set_has_server_addr();
  if (server_addr_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    server_addr_ = new ::std::string;
  }
  server_addr_->assign(value);
  // @@protoc_insertion_point(field_set_char:brpc.TrackMeRequest.server_addr)
}
inline void TrackMeRequest::set_server_addr(const char* value, size_t size) {
  set_has_server_addr();
  if (server_addr_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    server_addr_ = new ::std::string;
  }
  server_addr_->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:brpc.TrackMeRequest.server_addr)
}
inline ::std::string* TrackMeRequest::mutable_server_addr() {
  set_has_server_addr();
  if (server_addr_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    server_addr_ = new ::std::string;
  }
  // @@protoc_insertion_point(field_mutable:brpc.TrackMeRequest.server_addr)
  return server_addr_;
}
inline ::std::string* TrackMeRequest::release_server_addr() {
  clear_has_server_addr();
  if (server_addr_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    return NULL;
  } else {
    ::std::string* temp = server_addr_;
    server_addr_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    return temp;
  }
}
inline void TrackMeRequest::set_allocated_server_addr(::std::string* server_addr) {
  if (server_addr_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete server_addr_;
  }
  if (server_addr) {
    set_has_server_addr();
    server_addr_ = server_addr;
  } else {
    clear_has_server_addr();
    server_addr_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  // @@protoc_insertion_point(field_set_allocated:brpc.TrackMeRequest.server_addr)
}

// -------------------------------------------------------------------

// TrackMeResponse

// optional .brpc.TrackMeSeverity severity = 1;
inline bool TrackMeResponse::has_severity() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void TrackMeResponse::set_has_severity() {
  _has_bits_[0] |= 0x00000001u;
}
inline void TrackMeResponse::clear_has_severity() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void TrackMeResponse::clear_severity() {
  severity_ = 0;
  clear_has_severity();
}
inline ::brpc::TrackMeSeverity TrackMeResponse::severity() const {
  // @@protoc_insertion_point(field_get:brpc.TrackMeResponse.severity)
  return static_cast< ::brpc::TrackMeSeverity >(severity_);
}
inline void TrackMeResponse::set_severity(::brpc::TrackMeSeverity value) {
  assert(::brpc::TrackMeSeverity_IsValid(value));
  set_has_severity();
  severity_ = value;
  // @@protoc_insertion_point(field_set:brpc.TrackMeResponse.severity)
}

// optional string error_text = 2;
inline bool TrackMeResponse::has_error_text() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void TrackMeResponse::set_has_error_text() {
  _has_bits_[0] |= 0x00000002u;
}
inline void TrackMeResponse::clear_has_error_text() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void TrackMeResponse::clear_error_text() {
  if (error_text_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    error_text_->clear();
  }
  clear_has_error_text();
}
inline const ::std::string& TrackMeResponse::error_text() const {
  // @@protoc_insertion_point(field_get:brpc.TrackMeResponse.error_text)
  return *error_text_;
}
inline void TrackMeResponse::set_error_text(const ::std::string& value) {
  set_has_error_text();
  if (error_text_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    error_text_ = new ::std::string;
  }
  error_text_->assign(value);
  // @@protoc_insertion_point(field_set:brpc.TrackMeResponse.error_text)
}
inline void TrackMeResponse::set_error_text(const char* value) {
  set_has_error_text();
  if (error_text_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    error_text_ = new ::std::string;
  }
  error_text_->assign(value);
  // @@protoc_insertion_point(field_set_char:brpc.TrackMeResponse.error_text)
}
inline void TrackMeResponse::set_error_text(const char* value, size_t size) {
  set_has_error_text();
  if (error_text_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    error_text_ = new ::std::string;
  }
  error_text_->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:brpc.TrackMeResponse.error_text)
}
inline ::std::string* TrackMeResponse::mutable_error_text() {
  set_has_error_text();
  if (error_text_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    error_text_ = new ::std::string;
  }
  // @@protoc_insertion_point(field_mutable:brpc.TrackMeResponse.error_text)
  return error_text_;
}
inline ::std::string* TrackMeResponse::release_error_text() {
  clear_has_error_text();
  if (error_text_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    return NULL;
  } else {
    ::std::string* temp = error_text_;
    error_text_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    return temp;
  }
}
inline void TrackMeResponse::set_allocated_error_text(::std::string* error_text) {
  if (error_text_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete error_text_;
  }
  if (error_text) {
    set_has_error_text();
    error_text_ = error_text;
  } else {
    clear_has_error_text();
    error_text_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  // @@protoc_insertion_point(field_set_allocated:brpc.TrackMeResponse.error_text)
}

// optional int32 new_interval = 3;
inline bool TrackMeResponse::has_new_interval() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void TrackMeResponse::set_has_new_interval() {
  _has_bits_[0] |= 0x00000004u;
}
inline void TrackMeResponse::clear_has_new_interval() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void TrackMeResponse::clear_new_interval() {
  new_interval_ = 0;
  clear_has_new_interval();
}
inline ::google::protobuf::int32 TrackMeResponse::new_interval() const {
  // @@protoc_insertion_point(field_get:brpc.TrackMeResponse.new_interval)
  return new_interval_;
}
inline void TrackMeResponse::set_new_interval(::google::protobuf::int32 value) {
  set_has_new_interval();
  new_interval_ = value;
  // @@protoc_insertion_point(field_set:brpc.TrackMeResponse.new_interval)
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace brpc

#ifndef SWIG
namespace google {
namespace protobuf {

template <> struct is_proto_enum< ::brpc::TrackMeSeverity> : ::google::protobuf::internal::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::brpc::TrackMeSeverity>() {
  return ::brpc::TrackMeSeverity_descriptor();
}

}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_brpc_2ftrackme_2eproto__INCLUDED
