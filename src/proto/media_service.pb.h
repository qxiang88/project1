// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: media_service.proto

#ifndef PROTOBUF_media_5fservice_2eproto__INCLUDED
#define PROTOBUF_media_5fservice_2eproto__INCLUDED

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
#include <google/protobuf/service.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

namespace mms {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_media_5fservice_2eproto();
void protobuf_AssignDesc_media_5fservice_2eproto();
void protobuf_ShutdownFile_media_5fservice_2eproto();

class MediaHttpRequestPB;
class MediaHttpResponsePB;

// ===================================================================

class MediaHttpRequestPB : public ::google::protobuf::Message {
 public:
  MediaHttpRequestPB();
  virtual ~MediaHttpRequestPB();

  MediaHttpRequestPB(const MediaHttpRequestPB& from);

  inline MediaHttpRequestPB& operator=(const MediaHttpRequestPB& from) {
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
  static const MediaHttpRequestPB& default_instance();

  void Swap(MediaHttpRequestPB* other);

  // implements Message ----------------------------------------------

  MediaHttpRequestPB* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const MediaHttpRequestPB& from);
  void MergeFrom(const MediaHttpRequestPB& from);
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

  // @@protoc_insertion_point(class_scope:mms.MediaHttpRequestPB)
 private:

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  friend void  protobuf_AddDesc_media_5fservice_2eproto();
  friend void protobuf_AssignDesc_media_5fservice_2eproto();
  friend void protobuf_ShutdownFile_media_5fservice_2eproto();

  void InitAsDefaultInstance();
  static MediaHttpRequestPB* default_instance_;
};
// -------------------------------------------------------------------

class MediaHttpResponsePB : public ::google::protobuf::Message {
 public:
  MediaHttpResponsePB();
  virtual ~MediaHttpResponsePB();

  MediaHttpResponsePB(const MediaHttpResponsePB& from);

  inline MediaHttpResponsePB& operator=(const MediaHttpResponsePB& from) {
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
  static const MediaHttpResponsePB& default_instance();

  void Swap(MediaHttpResponsePB* other);

  // implements Message ----------------------------------------------

  MediaHttpResponsePB* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const MediaHttpResponsePB& from);
  void MergeFrom(const MediaHttpResponsePB& from);
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

  // @@protoc_insertion_point(class_scope:mms.MediaHttpResponsePB)
 private:

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  friend void  protobuf_AddDesc_media_5fservice_2eproto();
  friend void protobuf_AssignDesc_media_5fservice_2eproto();
  friend void protobuf_ShutdownFile_media_5fservice_2eproto();

  void InitAsDefaultInstance();
  static MediaHttpResponsePB* default_instance_;
};
// ===================================================================

class MediaManagerService_Stub;

class MediaManagerService : public ::google::protobuf::Service {
 protected:
  // This class should be treated as an abstract interface.
  inline MediaManagerService() {};
 public:
  virtual ~MediaManagerService();

  typedef MediaManagerService_Stub Stub;

  static const ::google::protobuf::ServiceDescriptor* descriptor();

  virtual void Handle(::google::protobuf::RpcController* controller,
                       const ::mms::MediaHttpRequestPB* request,
                       ::mms::MediaHttpResponsePB* response,
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
  GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(MediaManagerService);
};

class MediaManagerService_Stub : public MediaManagerService {
 public:
  MediaManagerService_Stub(::google::protobuf::RpcChannel* channel);
  MediaManagerService_Stub(::google::protobuf::RpcChannel* channel,
                   ::google::protobuf::Service::ChannelOwnership ownership);
  ~MediaManagerService_Stub();

  inline ::google::protobuf::RpcChannel* channel() { return channel_; }

  // implements MediaManagerService ------------------------------------------

  void Handle(::google::protobuf::RpcController* controller,
                       const ::mms::MediaHttpRequestPB* request,
                       ::mms::MediaHttpResponsePB* response,
                       ::google::protobuf::Closure* done);
 private:
  ::google::protobuf::RpcChannel* channel_;
  bool owns_channel_;
  GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(MediaManagerService_Stub);
};


// ===================================================================


// ===================================================================

// MediaHttpRequestPB

// -------------------------------------------------------------------

// MediaHttpResponsePB


// @@protoc_insertion_point(namespace_scope)

}  // namespace mms

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_media_5fservice_2eproto__INCLUDED
