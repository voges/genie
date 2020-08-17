/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/mitogen/genie for more details.
 */

#ifndef GENIE_ALIGNMENT_EXTERNAL_H
#define GENIE_ALIGNMENT_EXTERNAL_H

// ---------------------------------------------------------------------------------------------------------------------

#include <genie/util/bitreader.h>
#include <genie/util/bitwriter.h>
#include <cstdint>
#include <memory>

// ---------------------------------------------------------------------------------------------------------------------

namespace genie {
namespace core {
namespace record {

/**
 *
 */
class AlignmentExternal {
   public:
    /**
     *
     */
    enum class Type : uint8_t { NONE, OTHER_REC };

    /**
     *
     * @param _moreAlignmentInfoType
     */
    explicit AlignmentExternal(Type _moreAlignmentInfoType);

    /**
     *
     */
    virtual ~AlignmentExternal() = default;

    /**
     *
     * @param writer
     */
    virtual void write(util::BitWriter &writer) const;

    /**
     *
     * @param reader
     * @return
     */
    static std::unique_ptr<AlignmentExternal> factory(util::BitReader &reader);

    /**
     *
     * @return
     */
    virtual std::unique_ptr<AlignmentExternal> clone() const = 0;

   protected:
    Type moreAlignmentInfoType;  //!<
};

// ---------------------------------------------------------------------------------------------------------------------

}  // namespace record
}  // namespace core
}  // namespace genie

// ---------------------------------------------------------------------------------------------------------------------

#endif  // GENIE_ALIGNMENT_EXTERNAL_H

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------