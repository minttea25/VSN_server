// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_ITEMSPAWN_VSN_H_
#define FLATBUFFERS_GENERATED_ITEMSPAWN_VSN_H_

#include "flatbuffers/flatbuffers.h"

// Ensure the included flatbuffers.h is the same version as when this file was
// generated, otherwise it may not be compatible.
static_assert(FLATBUFFERS_VERSION_MAJOR == 24 &&
              FLATBUFFERS_VERSION_MINOR == 3 &&
              FLATBUFFERS_VERSION_REVISION == 7,
             "Non-compatible flatbuffers version included");

#include "Types_generated.h"

namespace VSN {

struct ItemSpawnInfo;

struct ItemSpawn;
struct ItemSpawnBuilder;

FLATBUFFERS_MANUALLY_ALIGNED_STRUCT(4) ItemSpawnInfo FLATBUFFERS_FINAL_CLASS {
 private:
  uint32_t item_nid_;
  uint8_t item_type_;
  int8_t padding0__;  int16_t padding1__;
  VSN::Vector2 pos_;

 public:
  ItemSpawnInfo()
      : item_nid_(0),
        item_type_(0),
        padding0__(0),
        padding1__(0),
        pos_() {
    (void)padding0__;
    (void)padding1__;
  }
  ItemSpawnInfo(uint32_t _item_nid, uint8_t _item_type, const VSN::Vector2 &_pos)
      : item_nid_(::flatbuffers::EndianScalar(_item_nid)),
        item_type_(::flatbuffers::EndianScalar(_item_type)),
        padding0__(0),
        padding1__(0),
        pos_(_pos) {
    (void)padding0__;
    (void)padding1__;
  }
  uint32_t item_nid() const {
    return ::flatbuffers::EndianScalar(item_nid_);
  }
  uint8_t item_type() const {
    return ::flatbuffers::EndianScalar(item_type_);
  }
  const VSN::Vector2 &pos() const {
    return pos_;
  }
};
FLATBUFFERS_STRUCT_END(ItemSpawnInfo, 16);

struct ItemSpawn FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef ItemSpawnBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_INFOS = 4
  };
  const ::flatbuffers::Vector<const VSN::ItemSpawnInfo *> *infos() const {
    return GetPointer<const ::flatbuffers::Vector<const VSN::ItemSpawnInfo *> *>(VT_INFOS);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_INFOS) &&
           verifier.VerifyVector(infos()) &&
           verifier.EndTable();
  }
};

struct ItemSpawnBuilder {
  typedef ItemSpawn Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_infos(::flatbuffers::Offset<::flatbuffers::Vector<const VSN::ItemSpawnInfo *>> infos) {
    fbb_.AddOffset(ItemSpawn::VT_INFOS, infos);
  }
  explicit ItemSpawnBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<ItemSpawn> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<ItemSpawn>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<ItemSpawn> CreateItemSpawn(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    ::flatbuffers::Offset<::flatbuffers::Vector<const VSN::ItemSpawnInfo *>> infos = 0) {
  ItemSpawnBuilder builder_(_fbb);
  builder_.add_infos(infos);
  return builder_.Finish();
}

inline ::flatbuffers::Offset<ItemSpawn> CreateItemSpawnDirect(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    const std::vector<VSN::ItemSpawnInfo> *infos = nullptr) {
  auto infos__ = infos ? _fbb.CreateVectorOfStructs<VSN::ItemSpawnInfo>(*infos) : 0;
  return VSN::CreateItemSpawn(
      _fbb,
      infos__);
}

inline const VSN::ItemSpawn *GetItemSpawn(const void *buf) {
  return ::flatbuffers::GetRoot<VSN::ItemSpawn>(buf);
}

inline const VSN::ItemSpawn *GetSizePrefixedItemSpawn(const void *buf) {
  return ::flatbuffers::GetSizePrefixedRoot<VSN::ItemSpawn>(buf);
}

inline bool VerifyItemSpawnBuffer(
    ::flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<VSN::ItemSpawn>(nullptr);
}

inline bool VerifySizePrefixedItemSpawnBuffer(
    ::flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<VSN::ItemSpawn>(nullptr);
}

inline void FinishItemSpawnBuffer(
    ::flatbuffers::FlatBufferBuilder &fbb,
    ::flatbuffers::Offset<VSN::ItemSpawn> root) {
  fbb.Finish(root);
}

inline void FinishSizePrefixedItemSpawnBuffer(
    ::flatbuffers::FlatBufferBuilder &fbb,
    ::flatbuffers::Offset<VSN::ItemSpawn> root) {
  fbb.FinishSizePrefixed(root);
}

}  // namespace VSN

#endif  // FLATBUFFERS_GENERATED_ITEMSPAWN_VSN_H_
