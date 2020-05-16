/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/mitogen/genie for more details.
 */

#ifndef GENIE_REF_ENCODER_H
#define GENIE_REF_ENCODER_H

// ---------------------------------------------------------------------------------------------------------------------

#include <genie/core/read-encoder.h>

// ---------------------------------------------------------------------------------------------------------------------

namespace genie {
namespace read {
namespace refcoder {

// ---------------------------------------------------------------------------------------------------------------------

class Encoder : public core::ReadEncoder {
   public:
    Encoder() : core::ReadEncoder() {}

    void flowIn(core::record::Chunk&&, const util::Section&) override {}

    void dryIn() override { dryOut(); }
};

// ---------------------------------------------------------------------------------------------------------------------

}  // namespace refcoder
}  // namespace read
}  // namespace genie

// ---------------------------------------------------------------------------------------------------------------------

#endif  // GENIE_ENCODER_H

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
