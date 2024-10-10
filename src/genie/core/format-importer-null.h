/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/MueFab/genie for more details.
 */

#ifndef SRC_GENIE_CORE_FORMAT_IMPORTER_NULL_H_
#define SRC_GENIE_CORE_FORMAT_IMPORTER_NULL_H_

// ---------------------------------------------------------------------------------------------------------------------

#include "genie/core/format-importer.h"

// ---------------------------------------------------------------------------------------------------------------------

namespace genie::core {
/**
 * @brief
 */
class NullImporter : public FormatImporter {
 protected:
    /**
     * @brief
     * @param _classifier
     * @return
     */
    bool pumpRetrieve(Classifier* _classifier) override;
};

// ---------------------------------------------------------------------------------------------------------------------

}  // namespace genie::core

// ---------------------------------------------------------------------------------------------------------------------

#endif  // SRC_GENIE_CORE_FORMAT_IMPORTER_NULL_H_

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
