/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/mitogen/genie for more details.
 */

// ---------------------------------------------------------------------------------------------------------------------

#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "genie/core/constants.h"
#include "genie/util/bitreader.h"
#include "genie/util/bitwriter.h"

#include "LikelihoodParameters.h"

// ---------------------------------------------------------------------------------------------------------------------

namespace genie {
namespace core {
namespace record {
namespace annotation_parameter_set {

LikelihoodParameters::LikelihoodParameters() : num_gl_per_sample(0), transform_flag(false), dtype_id(0) {}

LikelihoodParameters::LikelihoodParameters(uint8_t num_gl_per_sample, bool transform_flag, uint8_t dtype_id)
    : num_gl_per_sample(num_gl_per_sample), transform_flag(transform_flag), dtype_id(dtype_id) {
    if (!transform_flag) dtype_id = 0;
}

LikelihoodParameters::LikelihoodParameters(util::BitReader& reader) { read(reader); }

void LikelihoodParameters::read(util::BitReader& reader) {
    num_gl_per_sample = static_cast<uint8_t>(reader.read_b(8));
    transform_flag = static_cast<bool>(reader.read_b(1));
    if (transform_flag) dtype_id = static_cast<uint8_t>(reader.read_b(8));
}

void LikelihoodParameters::write(Writer& writer) const {
    writer.write(num_gl_per_sample, 8);
    writer.write(transform_flag, 1);
    if (transform_flag) writer.write(dtype_id, 8);
}

size_t LikelihoodParameters::getSize(core::Writer& writesize) const {
    write(writesize);
    return writesize.getBitsWritten();
}

// ---------------------------------------------------------------------------------------------------------------------

}  // namespace annotation_parameter_set
}  // namespace record
}  // namespace core
}  // namespace genie

// ---------------------------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
