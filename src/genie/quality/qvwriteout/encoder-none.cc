/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/MueFab/genie for more details.
 */

#include "genie/quality/qvwriteout/encoder-none.h"
#include <memory>
#include <utility>
#include "genie/quality/paramqv1/qv_coding_config_1.h"

// ---------------------------------------------------------------------------------------------------------------------

namespace genie::quality::qvwriteout {

// ---------------------------------------------------------------------------------------------------------------------

core::QVEncoder::QVCoded NoneEncoder::process(const core::record::Chunk&) {
    auto param = std::make_unique<paramqv1::QualityValues1>(paramqv1::QualityValues1::QvpsPresetId::ASCII, false);
    core::AccessUnit::Descriptor desc(core::GenDesc::QV);

    return std::make_tuple(std::move(param), std::move(desc), core::stats::PerfStats());
}

// ---------------------------------------------------------------------------------------------------------------------

}  // namespace genie::quality::qvwriteout

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
