// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_RESPONSE_VSN_H_
#define FLATBUFFERS_GENERATED_RESPONSE_VSN_H_

#include "flatbuffers/flatbuffers.h"

// Ensure the included flatbuffers.h is the same version as when this file was
// generated, otherwise it may not be compatible.
static_assert(FLATBUFFERS_VERSION_MAJOR == 24 &&
              FLATBUFFERS_VERSION_MINOR == 3 &&
              FLATBUFFERS_VERSION_REVISION == 7,
             "Non-compatible flatbuffers version included");

namespace VSN {

struct Response;
struct ResponseBuilder;

struct Response FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef ResponseBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_REQ_ID = 4,
    VT_RES = 6
  };
  uint32_t req_id() const {
    return GetField<uint32_t>(VT_REQ_ID, 0);
  }
  int16_t res() const {
    return GetField<int16_t>(VT_RES, 0);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint32_t>(verifier, VT_REQ_ID, 4) &&
           VerifyField<int16_t>(verifier, VT_RES, 2) &&
           verifier.EndTable();
  }
};

struct ResponseBuilder {
  typedef Response Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_req_id(uint32_t req_id) {
    fbb_.AddElement<uint32_t>(Response::VT_REQ_ID, req_id, 0);
  }
  void add_res(int16_t res) {
    fbb_.AddElement<int16_t>(Response::VT_RES, res, 0);
  }
  explicit ResponseBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<Response> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<Response>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<Response> CreateResponse(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    uint32_t req_id = 0,
    int16_t res = 0) {
  ResponseBuilder builder_(_fbb);
  builder_.add_req_id(req_id);
  builder_.add_res(res);
  return builder_.Finish();
}

inline const VSN::Response *GetResponse(const void *buf) {
  return ::flatbuffers::GetRoot<VSN::Response>(buf);
}

inline const VSN::Response *GetSizePrefixedResponse(const void *buf) {
  return ::flatbuffers::GetSizePrefixedRoot<VSN::Response>(buf);
}

inline bool VerifyResponseBuffer(
    ::flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<VSN::Response>(nullptr);
}

inline bool VerifySizePrefixedResponseBuffer(
    ::flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<VSN::Response>(nullptr);
}

inline void FinishResponseBuffer(
    ::flatbuffers::FlatBufferBuilder &fbb,
    ::flatbuffers::Offset<VSN::Response> root) {
  fbb.Finish(root);
}

inline void FinishSizePrefixedResponseBuffer(
    ::flatbuffers::FlatBufferBuilder &fbb,
    ::flatbuffers::Offset<VSN::Response> root) {
  fbb.FinishSizePrefixed(root);
}

}  // namespace VSN

#endif  // FLATBUFFERS_GENERATED_RESPONSE_VSN_H_
