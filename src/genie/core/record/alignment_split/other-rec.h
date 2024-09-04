/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/mitogen/genie for more details.
 */

#ifndef SRC_GENIE_CORE_RECORD_ALIGNMENT_SPLIT_OTHER_REC_H_
#define SRC_GENIE_CORE_RECORD_ALIGNMENT_SPLIT_OTHER_REC_H_

// ---------------------------------------------------------------------------------------------------------------------

#include <cstdint>
#include <memory>
#include "genie/core/record/alignment-split.h"
#include "genie/util/bit-reader.h"
#include "genie/util/bit-writer.h"

// ---------------------------------------------------------------------------------------------------------------------

namespace genie::core::record::alignment_split {

/**
 * @brief
 */
class OtherRec : public AlignmentSplit {
 private:
    uint64_t split_pos;     //!< @brief
    uint16_t split_seq_ID;  //!< @brief

 public:
    /**
     * @brief
     * @param _split_pos
     * @param _split_seq_ID
     */
    OtherRec(uint64_t _split_pos, uint16_t _split_seq_ID);

    /**
     * @brief
     * @return
     */
    [[nodiscard]] uint64_t getNextPos() const;

    /**
     * @brief
     * @return
     */
    [[nodiscard]] uint16_t getNextSeq() const;

    /**
     * @brief
     * @param reader
     */
    explicit OtherRec(util::BitReader& reader);

    /**
     * @brief
     */
    OtherRec();

    /**
     * @brief
     * @param writer
     */
    void write(util::BitWriter& writer) const override;

    /**
     * @brief
     * @return
     */
    [[nodiscard]] std::unique_ptr<AlignmentSplit> clone() const override;
};

// ---------------------------------------------------------------------------------------------------------------------

}  // namespace genie::core::record::alignment_split

// ---------------------------------------------------------------------------------------------------------------------

#endif  // SRC_GENIE_CORE_RECORD_ALIGNMENT_SPLIT_OTHER_REC_H_

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
