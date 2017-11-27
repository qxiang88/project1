# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: media_service.proto

import sys
_b=sys.version_info[0]<3 and (lambda x:x) or (lambda x:x.encode('latin1'))
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from google.protobuf import reflection as _reflection
from google.protobuf import symbol_database as _symbol_database
from google.protobuf import descriptor_pb2
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()




DESCRIPTOR = _descriptor.FileDescriptor(
  name='media_service.proto',
  package='mms',
  serialized_pb=_b('\n\x13media_service.proto\x12\x03mms\"\x14\n\x12MediaHttpRequestPB\"\x15\n\x13MediaHttpResponsePB2R\n\x13MediaManagerService\x12;\n\x06Handle\x12\x17.mms.MediaHttpRequestPB\x1a\x18.mms.MediaHttpResponsePBB\x03\x80\x01\x01')
)
_sym_db.RegisterFileDescriptor(DESCRIPTOR)




_MEDIAHTTPREQUESTPB = _descriptor.Descriptor(
  name='MediaHttpRequestPB',
  full_name='mms.MediaHttpRequestPB',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=28,
  serialized_end=48,
)


_MEDIAHTTPRESPONSEPB = _descriptor.Descriptor(
  name='MediaHttpResponsePB',
  full_name='mms.MediaHttpResponsePB',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=50,
  serialized_end=71,
)

DESCRIPTOR.message_types_by_name['MediaHttpRequestPB'] = _MEDIAHTTPREQUESTPB
DESCRIPTOR.message_types_by_name['MediaHttpResponsePB'] = _MEDIAHTTPRESPONSEPB

MediaHttpRequestPB = _reflection.GeneratedProtocolMessageType('MediaHttpRequestPB', (_message.Message,), dict(
  DESCRIPTOR = _MEDIAHTTPREQUESTPB,
  __module__ = 'media_service_pb2'
  # @@protoc_insertion_point(class_scope:mms.MediaHttpRequestPB)
  ))
_sym_db.RegisterMessage(MediaHttpRequestPB)

MediaHttpResponsePB = _reflection.GeneratedProtocolMessageType('MediaHttpResponsePB', (_message.Message,), dict(
  DESCRIPTOR = _MEDIAHTTPRESPONSEPB,
  __module__ = 'media_service_pb2'
  # @@protoc_insertion_point(class_scope:mms.MediaHttpResponsePB)
  ))
_sym_db.RegisterMessage(MediaHttpResponsePB)


DESCRIPTOR.has_options = True
DESCRIPTOR._options = _descriptor._ParseOptions(descriptor_pb2.FileOptions(), _b('\200\001\001'))
# @@protoc_insertion_point(module_scope)
