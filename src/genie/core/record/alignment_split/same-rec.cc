/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/MueFab/genie for more details.
 */

#include "genie/core/record/alignment_split/same-rec.h"
#include <memory>
#include <utility>
#include "genie/util/bit-reader.h"
#include "genie/util/bit-writer.h"

// ---------------------------------------------------------------------------------------------------------------------

namespace genie::core::record::alignment_split {

// ---------------------------------------------------------------------------------------------------------------------

SameRec::SameRec() : AlignmentSplit(Type::SAME_REC), delta(0), alignment() {}

// ---------------------------------------------------------------------------------------------------------------------

SameRec::SameRec(const int64_t _delta, Alignment _alignment)
    : AlignmentSplit(Type::SAME_REC), delta(_delta), alignment(std::move(_alignment)) {}

// ---------------------------------------------------------------------------------------------------------------------

SameRec::SameRec(const uint8_t as_depth, util::BitReader &reader)
    : AlignmentSplit(Type::SAME_REC),
      delta(reader.readAlignedInt<int64_t, 6>()),
      alignment(as_depth, reader) {}

// ---------------------------------------------------------------------------------------------------------------------

void SameRec::write(util::BitWriter &writer) const {
    AlignmentSplit::write(writer);
    writer.writeAlignedInt<int64_t, 6>(delta);
    alignment.write(writer);
}

// ---------------------------------------------------------------------------------------------------------------------

const Alignment &SameRec::getAlignment() const { return alignment; }

// ---------------------------------------------------------------------------------------------------------------------

int64_t SameRec::getDelta() const { return delta; }

// ---------------------------------------------------------------------------------------------------------------------

std::unique_ptr<AlignmentSplit> SameRec::clone() const {
    auto ret = std::make_unique<SameRec>();
    ret->delta = this->delta;
    ret->alignment = this->alignment;
    return ret;
}

// ---------------------------------------------------------------------------------------------------------------------

}  // namespace genie::core::record::alignment_split

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
