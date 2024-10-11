/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/MueFab/genie for more details.
 */

#include "genie/entropy/paramcabac/decoder.h"
#include <memory>
#include <utility>
#include "genie/util/bit-writer.h"

// ---------------------------------------------------------------------------------------------------------------------

namespace genie::entropy::paramcabac {

// ---------------------------------------------------------------------------------------------------------------------

DecoderRegular::DecoderRegular() : core::parameter::desc_pres::DecoderRegular(MODE_CABAC) {}

// ---------------------------------------------------------------------------------------------------------------------

DecoderRegular::DecoderRegular(core::GenDesc desc)
    : core::parameter::desc_pres::DecoderRegular(MODE_CABAC), descriptor_subsequence_cfgs() {
    for (size_t i = 0; i < core::getDescriptors()[static_cast<uint8_t>(desc)].subseqs.size(); ++i) {
        descriptor_subsequence_cfgs.emplace_back(static_cast<uint16_t>(i), false);
    }
}

// ---------------------------------------------------------------------------------------------------------------------

DecoderRegular::DecoderRegular(core::GenDesc desc, util::BitReader &reader)
    : core::parameter::desc_pres::DecoderRegular(MODE_CABAC) {
    uint8_t num_descriptor_subsequence_cfgs = reader.read<uint8_t>() + 1;
    for (size_t i = 0; i < num_descriptor_subsequence_cfgs; ++i) {
        descriptor_subsequence_cfgs.emplace_back(false, desc, reader);
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void DecoderRegular::setSubsequenceCfg(uint8_t index, Subsequence &&cfg) {
    descriptor_subsequence_cfgs[static_cast<uint8_t>(index)] = cfg;
}

// ---------------------------------------------------------------------------------------------------------------------

const Subsequence &DecoderRegular::getSubsequenceCfg(uint8_t index) const {
    return descriptor_subsequence_cfgs[static_cast<uint8_t>(index)];
}

// ---------------------------------------------------------------------------------------------------------------------

std::unique_ptr<core::parameter::desc_pres::Decoder> DecoderRegular::clone() const {
    return std::make_unique<DecoderRegular>(*this);
}

// ---------------------------------------------------------------------------------------------------------------------

Subsequence &DecoderRegular::getSubsequenceCfg(uint8_t index) { return descriptor_subsequence_cfgs[index]; }

// ---------------------------------------------------------------------------------------------------------------------

std::unique_ptr<core::parameter::desc_pres::DecoderRegular> DecoderRegular::create(genie::core::GenDesc desc,
                                                                                   util::BitReader &reader) {
    return std::make_unique<DecoderRegular>(desc, reader);
}

// ---------------------------------------------------------------------------------------------------------------------

void DecoderRegular::write(util::BitWriter &writer) const {
    core::parameter::desc_pres::Decoder::write(writer);
    writer.writeBits(descriptor_subsequence_cfgs.size() - 1, 8);
    for (auto &i : descriptor_subsequence_cfgs) {
        i.write(writer);
    }
}

// ---------------------------------------------------------------------------------------------------------------------

bool DecoderRegular::equals(const Decoder *dec) const {
    return core::parameter::desc_pres::Decoder::equals(dec) &&
           dynamic_cast<const DecoderRegular *>(dec)->descriptor_subsequence_cfgs == descriptor_subsequence_cfgs;
}

// ---------------------------------------------------------------------------------------------------------------------

DecoderTokenType::DecoderTokenType()
    : core::parameter::desc_pres::DecoderTokentype(MODE_CABAC), rle_guard_tokentype(0), descriptor_subsequence_cfgs() {
    for (uint16_t i = 0; i < 2; ++i) {
        descriptor_subsequence_cfgs.emplace_back(i, true);
    }
}

// ---------------------------------------------------------------------------------------------------------------------

DecoderTokenType::DecoderTokenType(core::GenDesc desc, util::BitReader &reader)
    : core::parameter::desc_pres::DecoderTokentype(MODE_CABAC) {
    uint8_t num_descriptor_subsequence_cfgs = 2;
    rle_guard_tokentype = reader.read<uint8_t>();
    for (size_t i = 0; i < num_descriptor_subsequence_cfgs; ++i) {
        descriptor_subsequence_cfgs.emplace_back(true, desc, reader);
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void DecoderTokenType::setSubsequenceCfg(uint8_t index, Subsequence &&cfg) {
    descriptor_subsequence_cfgs[static_cast<uint8_t>(index)] = cfg;
}

// ---------------------------------------------------------------------------------------------------------------------

const Subsequence &DecoderTokenType::getSubsequenceCfg(uint8_t index) const {
    return descriptor_subsequence_cfgs[static_cast<uint8_t>(index)];
}

// ---------------------------------------------------------------------------------------------------------------------

std::unique_ptr<core::parameter::desc_pres::Decoder> DecoderTokenType::clone() const {
    return std::make_unique<DecoderTokenType>(*this);
}

// ---------------------------------------------------------------------------------------------------------------------

Subsequence &DecoderTokenType::getSubsequenceCfg(uint8_t index) { return descriptor_subsequence_cfgs[index]; }

// ---------------------------------------------------------------------------------------------------------------------

std::unique_ptr<core::parameter::desc_pres::DecoderTokentype> DecoderTokenType::create(genie::core::GenDesc desc,
                                                                                       util::BitReader &reader) {
    return std::make_unique<DecoderTokenType>(desc, reader);
}

// ---------------------------------------------------------------------------------------------------------------------

void DecoderTokenType::write(util::BitWriter &writer) const {
    core::parameter::desc_pres::Decoder::write(writer);
    writer.writeBits(rle_guard_tokentype, 8);
    for (auto &i : descriptor_subsequence_cfgs) {
        i.write(writer);
    }
}

// ---------------------------------------------------------------------------------------------------------------------

uint8_t DecoderTokenType::getRleGuardTokentype() const { return rle_guard_tokentype; }

// ---------------------------------------------------------------------------------------------------------------------

bool DecoderTokenType::equals(const Decoder *dec) const {
    return core::parameter::desc_pres::Decoder::equals(dec) &&
           dynamic_cast<const DecoderTokenType *>(dec)->rle_guard_tokentype == rle_guard_tokentype &&
           dynamic_cast<const DecoderTokenType *>(dec)->rle_guard_tokentype == rle_guard_tokentype &&
           dynamic_cast<const DecoderTokenType *>(dec)->descriptor_subsequence_cfgs == descriptor_subsequence_cfgs;
}

// ---------------------------------------------------------------------------------------------------------------------

}  // namespace genie::entropy::paramcabac

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
