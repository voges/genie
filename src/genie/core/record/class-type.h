/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/mitogen/genie for more details.
 */

#ifndef GENIE_CLASSTYPE_H
#define GENIE_CLASSTYPE_H

// ---------------------------------------------------------------------------------------------------------------------

namespace genie {
namespace core {
namespace record {

/**
 *
 */
enum class ClassType : uint8_t {
    NONE = 0,
    CLASS_P = 1,
    CLASS_N = 2,
    CLASS_M = 3,
    CLASS_I = 4,
    CLASS_HM = 5,
    CLASS_U = 6
};

// ---------------------------------------------------------------------------------------------------------------------

}  // namespace record
}  // namespace core
}  // namespace genie

// ---------------------------------------------------------------------------------------------------------------------

#endif  // GENIE_CLASSTYPE_H

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------