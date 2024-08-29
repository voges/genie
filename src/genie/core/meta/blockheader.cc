/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/mitogen/genie for more details.
 */

#include "genie/core/meta/blockheader.h"

// ---------------------------------------------------------------------------------------------------------------------

namespace genie::core::meta {

// ---------------------------------------------------------------------------------------------------------------------

BlockHeader::BlockHeader(HeaderType _type) : type(_type) {}

// ---------------------------------------------------------------------------------------------------------------------

BlockHeader::HeaderType BlockHeader::getType() const { return type; }

// ---------------------------------------------------------------------------------------------------------------------

}  // namespace genie::core::meta

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
