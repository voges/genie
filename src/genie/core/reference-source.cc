/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/MueFab/genie for more details.
 */

#include "genie/core/reference-source.h"
#include <memory>

// ---------------------------------------------------------------------------------------------------------------------

namespace genie::core {

// ---------------------------------------------------------------------------------------------------------------------

ReferenceSource::ReferenceSource(ReferenceManager* mgr) : refMgr(mgr) {}

// ---------------------------------------------------------------------------------------------------------------------

genie::core::meta::Reference ReferenceSource::getMeta() const {
    return {
        "",           0, 0, 0, std::make_unique<meta::InternalRef>(static_cast<uint16_t>(0), static_cast<uint16_t>(0)),
        "Placeholder"};
}

// ---------------------------------------------------------------------------------------------------------------------

}  // namespace genie::core

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
