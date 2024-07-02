// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_MONSTERSPAWN_VSN_H_
#define FLATBUFFERS_GENERATED_MONSTERSPAWN_VSN_H_

#include "flatbuffers/flatbuffers.h"

// Ensure the included flatbuffers.h is the same version as when this file was
// generated, otherwise it may not be compatible.
static_assert(FLATBUFFERS_VERSION_MAJOR == 24 &&
              FLATBUFFERS_VERSION_MINOR == 3 &&
              FLATBUFFERS_VERSION_REVISION == 7,
             "Non-compatible flatbuffers version included");

#include "Types_generated.h"

namespace VSN {

struct MonsterSpawnInfo;

struct SpawnMonsters;
struct SpawnMonstersBuilder;

FLATBUFFERS_MANUALLY_ALIGNED_STRUCT(4) MonsterSpawnInfo FLATBUFFERS_FINAL_CLASS {
 private:
  uint32_t nid_;
  uint16_t monster_type_;
  int16_t padding0__;
  VSN::Vector2 pos_;

 public:
  MonsterSpawnInfo()
      : nid_(0),
        monster_type_(0),
        padding0__(0),
        pos_() {
    (void)padding0__;
  }
  MonsterSpawnInfo(uint32_t _nid, uint16_t _monster_type, const VSN::Vector2 &_pos)
      : nid_(::flatbuffers::EndianScalar(_nid)),
        monster_type_(::flatbuffers::EndianScalar(_monster_type)),
        padding0__(0),
        pos_(_pos) {
    (void)padding0__;
  }
  uint32_t nid() const {
    return ::flatbuffers::EndianScalar(nid_);
  }
  uint16_t monster_type() const {
    return ::flatbuffers::EndianScalar(monster_type_);
  }
  const VSN::Vector2 &pos() const {
    return pos_;
  }
};
FLATBUFFERS_STRUCT_END(MonsterSpawnInfo, 16);

struct SpawnMonsters FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef SpawnMonstersBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_INFOS = 4
  };
  const ::flatbuffers::Vector<const VSN::MonsterSpawnInfo *> *infos() const {
    return GetPointer<const ::flatbuffers::Vector<const VSN::MonsterSpawnInfo *> *>(VT_INFOS);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_INFOS) &&
           verifier.VerifyVector(infos()) &&
           verifier.EndTable();
  }
};

struct SpawnMonstersBuilder {
  typedef SpawnMonsters Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_infos(::flatbuffers::Offset<::flatbuffers::Vector<const VSN::MonsterSpawnInfo *>> infos) {
    fbb_.AddOffset(SpawnMonsters::VT_INFOS, infos);
  }
  explicit SpawnMonstersBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<SpawnMonsters> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<SpawnMonsters>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<SpawnMonsters> CreateSpawnMonsters(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    ::flatbuffers::Offset<::flatbuffers::Vector<const VSN::MonsterSpawnInfo *>> infos = 0) {
  SpawnMonstersBuilder builder_(_fbb);
  builder_.add_infos(infos);
  return builder_.Finish();
}

inline ::flatbuffers::Offset<SpawnMonsters> CreateSpawnMonstersDirect(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    const std::vector<VSN::MonsterSpawnInfo> *infos = nullptr) {
  auto infos__ = infos ? _fbb.CreateVectorOfStructs<VSN::MonsterSpawnInfo>(*infos) : 0;
  return VSN::CreateSpawnMonsters(
      _fbb,
      infos__);
}

inline const VSN::SpawnMonsters *GetSpawnMonsters(const void *buf) {
  return ::flatbuffers::GetRoot<VSN::SpawnMonsters>(buf);
}

inline const VSN::SpawnMonsters *GetSizePrefixedSpawnMonsters(const void *buf) {
  return ::flatbuffers::GetSizePrefixedRoot<VSN::SpawnMonsters>(buf);
}

inline bool VerifySpawnMonstersBuffer(
    ::flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<VSN::SpawnMonsters>(nullptr);
}

inline bool VerifySizePrefixedSpawnMonstersBuffer(
    ::flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<VSN::SpawnMonsters>(nullptr);
}

inline void FinishSpawnMonstersBuffer(
    ::flatbuffers::FlatBufferBuilder &fbb,
    ::flatbuffers::Offset<VSN::SpawnMonsters> root) {
  fbb.Finish(root);
}

inline void FinishSizePrefixedSpawnMonstersBuffer(
    ::flatbuffers::FlatBufferBuilder &fbb,
    ::flatbuffers::Offset<VSN::SpawnMonsters> root) {
  fbb.FinishSizePrefixed(root);
}

}  // namespace VSN

#endif  // FLATBUFFERS_GENERATED_MONSTERSPAWN_VSN_H_
