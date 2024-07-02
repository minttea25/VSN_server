// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_REQPLAYERDAMAGED_VSN_H_
#define FLATBUFFERS_GENERATED_REQPLAYERDAMAGED_VSN_H_

#include "flatbuffers/flatbuffers.h"

// Ensure the included flatbuffers.h is the same version as when this file was
// generated, otherwise it may not be compatible.
static_assert(FLATBUFFERS_VERSION_MAJOR == 24 &&
              FLATBUFFERS_VERSION_MINOR == 3 &&
              FLATBUFFERS_VERSION_REVISION == 7,
             "Non-compatible flatbuffers version included");

namespace VSN {

struct Req_PlayerDamaged;
struct Req_PlayerDamagedBuilder;

struct Req_PlayerDamaged FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef Req_PlayerDamagedBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_REQ_ID = 4,
    VT_DAMAGE = 6,
    VT_DEAD = 8
  };
  uint32_t req_id() const {
    return GetField<uint32_t>(VT_REQ_ID, 0);
  }
  int32_t damage() const {
    return GetField<int32_t>(VT_DAMAGE, 0);
  }
  bool dead() const {
    return GetField<uint8_t>(VT_DEAD, 0) != 0;
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint32_t>(verifier, VT_REQ_ID, 4) &&
           VerifyField<int32_t>(verifier, VT_DAMAGE, 4) &&
           VerifyField<uint8_t>(verifier, VT_DEAD, 1) &&
           verifier.EndTable();
  }
};

struct Req_PlayerDamagedBuilder {
  typedef Req_PlayerDamaged Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_req_id(uint32_t req_id) {
    fbb_.AddElement<uint32_t>(Req_PlayerDamaged::VT_REQ_ID, req_id, 0);
  }
  void add_damage(int32_t damage) {
    fbb_.AddElement<int32_t>(Req_PlayerDamaged::VT_DAMAGE, damage, 0);
  }
  void add_dead(bool dead) {
    fbb_.AddElement<uint8_t>(Req_PlayerDamaged::VT_DEAD, static_cast<uint8_t>(dead), 0);
  }
  explicit Req_PlayerDamagedBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<Req_PlayerDamaged> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<Req_PlayerDamaged>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<Req_PlayerDamaged> CreateReq_PlayerDamaged(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    uint32_t req_id = 0,
    int32_t damage = 0,
    bool dead = false) {
  Req_PlayerDamagedBuilder builder_(_fbb);
  builder_.add_damage(damage);
  builder_.add_req_id(req_id);
  builder_.add_dead(dead);
  return builder_.Finish();
}

inline const VSN::Req_PlayerDamaged *GetReq_PlayerDamaged(const void *buf) {
  return ::flatbuffers::GetRoot<VSN::Req_PlayerDamaged>(buf);
}

inline const VSN::Req_PlayerDamaged *GetSizePrefixedReq_PlayerDamaged(const void *buf) {
  return ::flatbuffers::GetSizePrefixedRoot<VSN::Req_PlayerDamaged>(buf);
}

inline bool VerifyReq_PlayerDamagedBuffer(
    ::flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<VSN::Req_PlayerDamaged>(nullptr);
}

inline bool VerifySizePrefixedReq_PlayerDamagedBuffer(
    ::flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<VSN::Req_PlayerDamaged>(nullptr);
}

inline void FinishReq_PlayerDamagedBuffer(
    ::flatbuffers::FlatBufferBuilder &fbb,
    ::flatbuffers::Offset<VSN::Req_PlayerDamaged> root) {
  fbb.Finish(root);
}

inline void FinishSizePrefixedReq_PlayerDamagedBuffer(
    ::flatbuffers::FlatBufferBuilder &fbb,
    ::flatbuffers::Offset<VSN::Req_PlayerDamaged> root) {
  fbb.FinishSizePrefixed(root);
}

}  // namespace VSN

#endif  // FLATBUFFERS_GENERATED_REQPLAYERDAMAGED_VSN_H_
