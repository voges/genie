/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/MueFab/genie for more details.
 */

#include "genie/util/ordered-section.h"

// ---------------------------------------------------------------------------------------------------------------------

namespace genie::util {

// ---------------------------------------------------------------------------------------------------------------------

OrderedSection::OrderedSection(OrderedLock* _lock, const Section& id) : lock(_lock), length(id.length) {
    lock->wait(id.start);
}

// ---------------------------------------------------------------------------------------------------------------------

OrderedSection::~OrderedSection() { lock->finished(length); }

// ---------------------------------------------------------------------------------------------------------------------

}  // namespace genie::util

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
