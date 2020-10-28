// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: history.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_history_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_history_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3012000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3012002 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata_lite.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
#include <google/protobuf/timestamp.pb.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_history_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_history_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxillaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[1]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_history_2eproto;
namespace msg {
class History;
class HistoryDefaultTypeInternal;
extern HistoryDefaultTypeInternal _History_default_instance_;
}  // namespace msg
PROTOBUF_NAMESPACE_OPEN
template<> ::msg::History* Arena::CreateMaybeMessage<::msg::History>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace msg {

// ===================================================================

class History PROTOBUF_FINAL :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:msg.History) */ {
 public:
  inline History() : History(nullptr) {};
  virtual ~History();

  History(const History& from);
  History(History&& from) noexcept
    : History() {
    *this = ::std::move(from);
  }

  inline History& operator=(const History& from) {
    CopyFrom(from);
    return *this;
  }
  inline History& operator=(History&& from) noexcept {
    if (GetArena() == from.GetArena()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return GetMetadataStatic().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return GetMetadataStatic().reflection;
  }
  static const History& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const History* internal_default_instance() {
    return reinterpret_cast<const History*>(
               &_History_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(History& a, History& b) {
    a.Swap(&b);
  }
  inline void Swap(History* other) {
    if (other == this) return;
    if (GetArena() == other->GetArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(History* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline History* New() const final {
    return CreateMaybeMessage<History>(nullptr);
  }

  History* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<History>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const History& from);
  void MergeFrom(const History& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* _InternalSerialize(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(History* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "msg.History";
  }
  protected:
  explicit History(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_history_2eproto);
    return ::descriptor_table_history_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kRequestIdFieldNumber = 3,
    kTypeFieldNumber = 4,
    kToUserFieldNumber = 5,
    kStatusFieldNumber = 7,
    kMessageFieldNumber = 8,
    kCreateAtFieldNumber = 9,
    kTransactionIdFieldNumber = 1,
    kUserIdFieldNumber = 2,
    kAmountFieldNumber = 6,
  };
  // string request_id = 3;
  void clear_request_id();
  const std::string& request_id() const;
  void set_request_id(const std::string& value);
  void set_request_id(std::string&& value);
  void set_request_id(const char* value);
  void set_request_id(const char* value, size_t size);
  std::string* mutable_request_id();
  std::string* release_request_id();
  void set_allocated_request_id(std::string* request_id);
  GOOGLE_PROTOBUF_RUNTIME_DEPRECATED("The unsafe_arena_ accessors for"
  "    string fields are deprecated and will be removed in a"
  "    future release.")
  std::string* unsafe_arena_release_request_id();
  GOOGLE_PROTOBUF_RUNTIME_DEPRECATED("The unsafe_arena_ accessors for"
  "    string fields are deprecated and will be removed in a"
  "    future release.")
  void unsafe_arena_set_allocated_request_id(
      std::string* request_id);
  private:
  const std::string& _internal_request_id() const;
  void _internal_set_request_id(const std::string& value);
  std::string* _internal_mutable_request_id();
  public:

  // string type = 4;
  void clear_type();
  const std::string& type() const;
  void set_type(const std::string& value);
  void set_type(std::string&& value);
  void set_type(const char* value);
  void set_type(const char* value, size_t size);
  std::string* mutable_type();
  std::string* release_type();
  void set_allocated_type(std::string* type);
  GOOGLE_PROTOBUF_RUNTIME_DEPRECATED("The unsafe_arena_ accessors for"
  "    string fields are deprecated and will be removed in a"
  "    future release.")
  std::string* unsafe_arena_release_type();
  GOOGLE_PROTOBUF_RUNTIME_DEPRECATED("The unsafe_arena_ accessors for"
  "    string fields are deprecated and will be removed in a"
  "    future release.")
  void unsafe_arena_set_allocated_type(
      std::string* type);
  private:
  const std::string& _internal_type() const;
  void _internal_set_type(const std::string& value);
  std::string* _internal_mutable_type();
  public:

  // string to_user = 5;
  void clear_to_user();
  const std::string& to_user() const;
  void set_to_user(const std::string& value);
  void set_to_user(std::string&& value);
  void set_to_user(const char* value);
  void set_to_user(const char* value, size_t size);
  std::string* mutable_to_user();
  std::string* release_to_user();
  void set_allocated_to_user(std::string* to_user);
  GOOGLE_PROTOBUF_RUNTIME_DEPRECATED("The unsafe_arena_ accessors for"
  "    string fields are deprecated and will be removed in a"
  "    future release.")
  std::string* unsafe_arena_release_to_user();
  GOOGLE_PROTOBUF_RUNTIME_DEPRECATED("The unsafe_arena_ accessors for"
  "    string fields are deprecated and will be removed in a"
  "    future release.")
  void unsafe_arena_set_allocated_to_user(
      std::string* to_user);
  private:
  const std::string& _internal_to_user() const;
  void _internal_set_to_user(const std::string& value);
  std::string* _internal_mutable_to_user();
  public:

  // string status = 7;
  void clear_status();
  const std::string& status() const;
  void set_status(const std::string& value);
  void set_status(std::string&& value);
  void set_status(const char* value);
  void set_status(const char* value, size_t size);
  std::string* mutable_status();
  std::string* release_status();
  void set_allocated_status(std::string* status);
  GOOGLE_PROTOBUF_RUNTIME_DEPRECATED("The unsafe_arena_ accessors for"
  "    string fields are deprecated and will be removed in a"
  "    future release.")
  std::string* unsafe_arena_release_status();
  GOOGLE_PROTOBUF_RUNTIME_DEPRECATED("The unsafe_arena_ accessors for"
  "    string fields are deprecated and will be removed in a"
  "    future release.")
  void unsafe_arena_set_allocated_status(
      std::string* status);
  private:
  const std::string& _internal_status() const;
  void _internal_set_status(const std::string& value);
  std::string* _internal_mutable_status();
  public:

  // string message = 8;
  void clear_message();
  const std::string& message() const;
  void set_message(const std::string& value);
  void set_message(std::string&& value);
  void set_message(const char* value);
  void set_message(const char* value, size_t size);
  std::string* mutable_message();
  std::string* release_message();
  void set_allocated_message(std::string* message);
  GOOGLE_PROTOBUF_RUNTIME_DEPRECATED("The unsafe_arena_ accessors for"
  "    string fields are deprecated and will be removed in a"
  "    future release.")
  std::string* unsafe_arena_release_message();
  GOOGLE_PROTOBUF_RUNTIME_DEPRECATED("The unsafe_arena_ accessors for"
  "    string fields are deprecated and will be removed in a"
  "    future release.")
  void unsafe_arena_set_allocated_message(
      std::string* message);
  private:
  const std::string& _internal_message() const;
  void _internal_set_message(const std::string& value);
  std::string* _internal_mutable_message();
  public:

  // .google.protobuf.Timestamp create_at = 9;
  bool has_create_at() const;
  private:
  bool _internal_has_create_at() const;
  public:
  void clear_create_at();
  const PROTOBUF_NAMESPACE_ID::Timestamp& create_at() const;
  PROTOBUF_NAMESPACE_ID::Timestamp* release_create_at();
  PROTOBUF_NAMESPACE_ID::Timestamp* mutable_create_at();
  void set_allocated_create_at(PROTOBUF_NAMESPACE_ID::Timestamp* create_at);
  private:
  const PROTOBUF_NAMESPACE_ID::Timestamp& _internal_create_at() const;
  PROTOBUF_NAMESPACE_ID::Timestamp* _internal_mutable_create_at();
  public:
  void unsafe_arena_set_allocated_create_at(
      PROTOBUF_NAMESPACE_ID::Timestamp* create_at);
  PROTOBUF_NAMESPACE_ID::Timestamp* unsafe_arena_release_create_at();

  // uint64 transaction_id = 1;
  void clear_transaction_id();
  ::PROTOBUF_NAMESPACE_ID::uint64 transaction_id() const;
  void set_transaction_id(::PROTOBUF_NAMESPACE_ID::uint64 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint64 _internal_transaction_id() const;
  void _internal_set_transaction_id(::PROTOBUF_NAMESPACE_ID::uint64 value);
  public:

  // uint64 user_id = 2;
  void clear_user_id();
  ::PROTOBUF_NAMESPACE_ID::uint64 user_id() const;
  void set_user_id(::PROTOBUF_NAMESPACE_ID::uint64 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint64 _internal_user_id() const;
  void _internal_set_user_id(::PROTOBUF_NAMESPACE_ID::uint64 value);
  public:

  // int64 amount = 6;
  void clear_amount();
  ::PROTOBUF_NAMESPACE_ID::int64 amount() const;
  void set_amount(::PROTOBUF_NAMESPACE_ID::int64 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int64 _internal_amount() const;
  void _internal_set_amount(::PROTOBUF_NAMESPACE_ID::int64 value);
  public:

  // @@protoc_insertion_point(class_scope:msg.History)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr request_id_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr type_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr to_user_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr status_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr message_;
  PROTOBUF_NAMESPACE_ID::Timestamp* create_at_;
  ::PROTOBUF_NAMESPACE_ID::uint64 transaction_id_;
  ::PROTOBUF_NAMESPACE_ID::uint64 user_id_;
  ::PROTOBUF_NAMESPACE_ID::int64 amount_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_history_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// History

// uint64 transaction_id = 1;
inline void History::clear_transaction_id() {
  transaction_id_ = PROTOBUF_ULONGLONG(0);
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 History::_internal_transaction_id() const {
  return transaction_id_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 History::transaction_id() const {
  // @@protoc_insertion_point(field_get:msg.History.transaction_id)
  return _internal_transaction_id();
}
inline void History::_internal_set_transaction_id(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  
  transaction_id_ = value;
}
inline void History::set_transaction_id(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  _internal_set_transaction_id(value);
  // @@protoc_insertion_point(field_set:msg.History.transaction_id)
}

// uint64 user_id = 2;
inline void History::clear_user_id() {
  user_id_ = PROTOBUF_ULONGLONG(0);
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 History::_internal_user_id() const {
  return user_id_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 History::user_id() const {
  // @@protoc_insertion_point(field_get:msg.History.user_id)
  return _internal_user_id();
}
inline void History::_internal_set_user_id(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  
  user_id_ = value;
}
inline void History::set_user_id(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  _internal_set_user_id(value);
  // @@protoc_insertion_point(field_set:msg.History.user_id)
}

// string request_id = 3;
inline void History::clear_request_id() {
  request_id_.ClearToEmpty(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline const std::string& History::request_id() const {
  // @@protoc_insertion_point(field_get:msg.History.request_id)
  return _internal_request_id();
}
inline void History::set_request_id(const std::string& value) {
  _internal_set_request_id(value);
  // @@protoc_insertion_point(field_set:msg.History.request_id)
}
inline std::string* History::mutable_request_id() {
  // @@protoc_insertion_point(field_mutable:msg.History.request_id)
  return _internal_mutable_request_id();
}
inline const std::string& History::_internal_request_id() const {
  return request_id_.Get();
}
inline void History::_internal_set_request_id(const std::string& value) {
  
  request_id_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), value, GetArena());
}
inline void History::set_request_id(std::string&& value) {
  
  request_id_.Set(
    &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::move(value), GetArena());
  // @@protoc_insertion_point(field_set_rvalue:msg.History.request_id)
}
inline void History::set_request_id(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  
  request_id_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(value),
              GetArena());
  // @@protoc_insertion_point(field_set_char:msg.History.request_id)
}
inline void History::set_request_id(const char* value,
    size_t size) {
  
  request_id_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(
      reinterpret_cast<const char*>(value), size), GetArena());
  // @@protoc_insertion_point(field_set_pointer:msg.History.request_id)
}
inline std::string* History::_internal_mutable_request_id() {
  
  return request_id_.Mutable(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline std::string* History::release_request_id() {
  // @@protoc_insertion_point(field_release:msg.History.request_id)
  return request_id_.Release(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline void History::set_allocated_request_id(std::string* request_id) {
  if (request_id != nullptr) {
    
  } else {
    
  }
  request_id_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), request_id,
      GetArena());
  // @@protoc_insertion_point(field_set_allocated:msg.History.request_id)
}
inline std::string* History::unsafe_arena_release_request_id() {
  // @@protoc_insertion_point(field_unsafe_arena_release:msg.History.request_id)
  GOOGLE_DCHECK(GetArena() != nullptr);
  
  return request_id_.UnsafeArenaRelease(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      GetArena());
}
inline void History::unsafe_arena_set_allocated_request_id(
    std::string* request_id) {
  GOOGLE_DCHECK(GetArena() != nullptr);
  if (request_id != nullptr) {
    
  } else {
    
  }
  request_id_.UnsafeArenaSetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      request_id, GetArena());
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:msg.History.request_id)
}

// string type = 4;
inline void History::clear_type() {
  type_.ClearToEmpty(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline const std::string& History::type() const {
  // @@protoc_insertion_point(field_get:msg.History.type)
  return _internal_type();
}
inline void History::set_type(const std::string& value) {
  _internal_set_type(value);
  // @@protoc_insertion_point(field_set:msg.History.type)
}
inline std::string* History::mutable_type() {
  // @@protoc_insertion_point(field_mutable:msg.History.type)
  return _internal_mutable_type();
}
inline const std::string& History::_internal_type() const {
  return type_.Get();
}
inline void History::_internal_set_type(const std::string& value) {
  
  type_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), value, GetArena());
}
inline void History::set_type(std::string&& value) {
  
  type_.Set(
    &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::move(value), GetArena());
  // @@protoc_insertion_point(field_set_rvalue:msg.History.type)
}
inline void History::set_type(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  
  type_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(value),
              GetArena());
  // @@protoc_insertion_point(field_set_char:msg.History.type)
}
inline void History::set_type(const char* value,
    size_t size) {
  
  type_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(
      reinterpret_cast<const char*>(value), size), GetArena());
  // @@protoc_insertion_point(field_set_pointer:msg.History.type)
}
inline std::string* History::_internal_mutable_type() {
  
  return type_.Mutable(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline std::string* History::release_type() {
  // @@protoc_insertion_point(field_release:msg.History.type)
  return type_.Release(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline void History::set_allocated_type(std::string* type) {
  if (type != nullptr) {
    
  } else {
    
  }
  type_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), type,
      GetArena());
  // @@protoc_insertion_point(field_set_allocated:msg.History.type)
}
inline std::string* History::unsafe_arena_release_type() {
  // @@protoc_insertion_point(field_unsafe_arena_release:msg.History.type)
  GOOGLE_DCHECK(GetArena() != nullptr);
  
  return type_.UnsafeArenaRelease(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      GetArena());
}
inline void History::unsafe_arena_set_allocated_type(
    std::string* type) {
  GOOGLE_DCHECK(GetArena() != nullptr);
  if (type != nullptr) {
    
  } else {
    
  }
  type_.UnsafeArenaSetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      type, GetArena());
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:msg.History.type)
}

// string to_user = 5;
inline void History::clear_to_user() {
  to_user_.ClearToEmpty(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline const std::string& History::to_user() const {
  // @@protoc_insertion_point(field_get:msg.History.to_user)
  return _internal_to_user();
}
inline void History::set_to_user(const std::string& value) {
  _internal_set_to_user(value);
  // @@protoc_insertion_point(field_set:msg.History.to_user)
}
inline std::string* History::mutable_to_user() {
  // @@protoc_insertion_point(field_mutable:msg.History.to_user)
  return _internal_mutable_to_user();
}
inline const std::string& History::_internal_to_user() const {
  return to_user_.Get();
}
inline void History::_internal_set_to_user(const std::string& value) {
  
  to_user_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), value, GetArena());
}
inline void History::set_to_user(std::string&& value) {
  
  to_user_.Set(
    &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::move(value), GetArena());
  // @@protoc_insertion_point(field_set_rvalue:msg.History.to_user)
}
inline void History::set_to_user(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  
  to_user_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(value),
              GetArena());
  // @@protoc_insertion_point(field_set_char:msg.History.to_user)
}
inline void History::set_to_user(const char* value,
    size_t size) {
  
  to_user_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(
      reinterpret_cast<const char*>(value), size), GetArena());
  // @@protoc_insertion_point(field_set_pointer:msg.History.to_user)
}
inline std::string* History::_internal_mutable_to_user() {
  
  return to_user_.Mutable(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline std::string* History::release_to_user() {
  // @@protoc_insertion_point(field_release:msg.History.to_user)
  return to_user_.Release(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline void History::set_allocated_to_user(std::string* to_user) {
  if (to_user != nullptr) {
    
  } else {
    
  }
  to_user_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), to_user,
      GetArena());
  // @@protoc_insertion_point(field_set_allocated:msg.History.to_user)
}
inline std::string* History::unsafe_arena_release_to_user() {
  // @@protoc_insertion_point(field_unsafe_arena_release:msg.History.to_user)
  GOOGLE_DCHECK(GetArena() != nullptr);
  
  return to_user_.UnsafeArenaRelease(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      GetArena());
}
inline void History::unsafe_arena_set_allocated_to_user(
    std::string* to_user) {
  GOOGLE_DCHECK(GetArena() != nullptr);
  if (to_user != nullptr) {
    
  } else {
    
  }
  to_user_.UnsafeArenaSetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      to_user, GetArena());
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:msg.History.to_user)
}

// int64 amount = 6;
inline void History::clear_amount() {
  amount_ = PROTOBUF_LONGLONG(0);
}
inline ::PROTOBUF_NAMESPACE_ID::int64 History::_internal_amount() const {
  return amount_;
}
inline ::PROTOBUF_NAMESPACE_ID::int64 History::amount() const {
  // @@protoc_insertion_point(field_get:msg.History.amount)
  return _internal_amount();
}
inline void History::_internal_set_amount(::PROTOBUF_NAMESPACE_ID::int64 value) {
  
  amount_ = value;
}
inline void History::set_amount(::PROTOBUF_NAMESPACE_ID::int64 value) {
  _internal_set_amount(value);
  // @@protoc_insertion_point(field_set:msg.History.amount)
}

// string status = 7;
inline void History::clear_status() {
  status_.ClearToEmpty(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline const std::string& History::status() const {
  // @@protoc_insertion_point(field_get:msg.History.status)
  return _internal_status();
}
inline void History::set_status(const std::string& value) {
  _internal_set_status(value);
  // @@protoc_insertion_point(field_set:msg.History.status)
}
inline std::string* History::mutable_status() {
  // @@protoc_insertion_point(field_mutable:msg.History.status)
  return _internal_mutable_status();
}
inline const std::string& History::_internal_status() const {
  return status_.Get();
}
inline void History::_internal_set_status(const std::string& value) {
  
  status_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), value, GetArena());
}
inline void History::set_status(std::string&& value) {
  
  status_.Set(
    &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::move(value), GetArena());
  // @@protoc_insertion_point(field_set_rvalue:msg.History.status)
}
inline void History::set_status(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  
  status_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(value),
              GetArena());
  // @@protoc_insertion_point(field_set_char:msg.History.status)
}
inline void History::set_status(const char* value,
    size_t size) {
  
  status_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(
      reinterpret_cast<const char*>(value), size), GetArena());
  // @@protoc_insertion_point(field_set_pointer:msg.History.status)
}
inline std::string* History::_internal_mutable_status() {
  
  return status_.Mutable(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline std::string* History::release_status() {
  // @@protoc_insertion_point(field_release:msg.History.status)
  return status_.Release(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline void History::set_allocated_status(std::string* status) {
  if (status != nullptr) {
    
  } else {
    
  }
  status_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), status,
      GetArena());
  // @@protoc_insertion_point(field_set_allocated:msg.History.status)
}
inline std::string* History::unsafe_arena_release_status() {
  // @@protoc_insertion_point(field_unsafe_arena_release:msg.History.status)
  GOOGLE_DCHECK(GetArena() != nullptr);
  
  return status_.UnsafeArenaRelease(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      GetArena());
}
inline void History::unsafe_arena_set_allocated_status(
    std::string* status) {
  GOOGLE_DCHECK(GetArena() != nullptr);
  if (status != nullptr) {
    
  } else {
    
  }
  status_.UnsafeArenaSetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      status, GetArena());
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:msg.History.status)
}

// string message = 8;
inline void History::clear_message() {
  message_.ClearToEmpty(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline const std::string& History::message() const {
  // @@protoc_insertion_point(field_get:msg.History.message)
  return _internal_message();
}
inline void History::set_message(const std::string& value) {
  _internal_set_message(value);
  // @@protoc_insertion_point(field_set:msg.History.message)
}
inline std::string* History::mutable_message() {
  // @@protoc_insertion_point(field_mutable:msg.History.message)
  return _internal_mutable_message();
}
inline const std::string& History::_internal_message() const {
  return message_.Get();
}
inline void History::_internal_set_message(const std::string& value) {
  
  message_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), value, GetArena());
}
inline void History::set_message(std::string&& value) {
  
  message_.Set(
    &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::move(value), GetArena());
  // @@protoc_insertion_point(field_set_rvalue:msg.History.message)
}
inline void History::set_message(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  
  message_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(value),
              GetArena());
  // @@protoc_insertion_point(field_set_char:msg.History.message)
}
inline void History::set_message(const char* value,
    size_t size) {
  
  message_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(
      reinterpret_cast<const char*>(value), size), GetArena());
  // @@protoc_insertion_point(field_set_pointer:msg.History.message)
}
inline std::string* History::_internal_mutable_message() {
  
  return message_.Mutable(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline std::string* History::release_message() {
  // @@protoc_insertion_point(field_release:msg.History.message)
  return message_.Release(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline void History::set_allocated_message(std::string* message) {
  if (message != nullptr) {
    
  } else {
    
  }
  message_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), message,
      GetArena());
  // @@protoc_insertion_point(field_set_allocated:msg.History.message)
}
inline std::string* History::unsafe_arena_release_message() {
  // @@protoc_insertion_point(field_unsafe_arena_release:msg.History.message)
  GOOGLE_DCHECK(GetArena() != nullptr);
  
  return message_.UnsafeArenaRelease(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      GetArena());
}
inline void History::unsafe_arena_set_allocated_message(
    std::string* message) {
  GOOGLE_DCHECK(GetArena() != nullptr);
  if (message != nullptr) {
    
  } else {
    
  }
  message_.UnsafeArenaSetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      message, GetArena());
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:msg.History.message)
}

// .google.protobuf.Timestamp create_at = 9;
inline bool History::_internal_has_create_at() const {
  return this != internal_default_instance() && create_at_ != nullptr;
}
inline bool History::has_create_at() const {
  return _internal_has_create_at();
}
inline const PROTOBUF_NAMESPACE_ID::Timestamp& History::_internal_create_at() const {
  const PROTOBUF_NAMESPACE_ID::Timestamp* p = create_at_;
  return p != nullptr ? *p : *reinterpret_cast<const PROTOBUF_NAMESPACE_ID::Timestamp*>(
      &PROTOBUF_NAMESPACE_ID::_Timestamp_default_instance_);
}
inline const PROTOBUF_NAMESPACE_ID::Timestamp& History::create_at() const {
  // @@protoc_insertion_point(field_get:msg.History.create_at)
  return _internal_create_at();
}
inline void History::unsafe_arena_set_allocated_create_at(
    PROTOBUF_NAMESPACE_ID::Timestamp* create_at) {
  if (GetArena() == nullptr) {
    delete reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(create_at_);
  }
  create_at_ = create_at;
  if (create_at) {
    
  } else {
    
  }
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:msg.History.create_at)
}
inline PROTOBUF_NAMESPACE_ID::Timestamp* History::release_create_at() {
  auto temp = unsafe_arena_release_create_at();
  if (GetArena() != nullptr) {
    temp = ::PROTOBUF_NAMESPACE_ID::internal::DuplicateIfNonNull(temp);
  }
  return temp;
}
inline PROTOBUF_NAMESPACE_ID::Timestamp* History::unsafe_arena_release_create_at() {
  // @@protoc_insertion_point(field_release:msg.History.create_at)
  
  PROTOBUF_NAMESPACE_ID::Timestamp* temp = create_at_;
  create_at_ = nullptr;
  return temp;
}
inline PROTOBUF_NAMESPACE_ID::Timestamp* History::_internal_mutable_create_at() {
  
  if (create_at_ == nullptr) {
    auto* p = CreateMaybeMessage<PROTOBUF_NAMESPACE_ID::Timestamp>(GetArena());
    create_at_ = p;
  }
  return create_at_;
}
inline PROTOBUF_NAMESPACE_ID::Timestamp* History::mutable_create_at() {
  // @@protoc_insertion_point(field_mutable:msg.History.create_at)
  return _internal_mutable_create_at();
}
inline void History::set_allocated_create_at(PROTOBUF_NAMESPACE_ID::Timestamp* create_at) {
  ::PROTOBUF_NAMESPACE_ID::Arena* message_arena = GetArena();
  if (message_arena == nullptr) {
    delete reinterpret_cast< ::PROTOBUF_NAMESPACE_ID::MessageLite*>(create_at_);
  }
  if (create_at) {
    ::PROTOBUF_NAMESPACE_ID::Arena* submessage_arena =
      reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(create_at)->GetArena();
    if (message_arena != submessage_arena) {
      create_at = ::PROTOBUF_NAMESPACE_ID::internal::GetOwnedMessage(
          message_arena, create_at, submessage_arena);
    }
    
  } else {
    
  }
  create_at_ = create_at;
  // @@protoc_insertion_point(field_set_allocated:msg.History.create_at)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace msg

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_history_2eproto