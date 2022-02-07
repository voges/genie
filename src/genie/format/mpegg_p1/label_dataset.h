/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/mitogen/genie for more details.
 */

#ifndef SRC_GENIE_FORMAT_MPEGG_P1_LABEL_DATASET_H_
#define SRC_GENIE_FORMAT_MPEGG_P1_LABEL_DATASET_H_

// ---------------------------------------------------------------------------------------------------------------------

#include <vector>
#include "genie/format/mpegg_p1/label_region.h"
#include "genie/util/bitreader.h"
#include "genie/util/bitwriter.h"

// ---------------------------------------------------------------------------------------------------------------------

namespace genie {
namespace format {
namespace mpegg_p1 {

/**
 * @brief
 */
class LabelDataset {
 private:
    uint16_t dataset_ID;  //!< @brief

    std::vector<LabelRegion> dataset_regions;  //!< @brief

 public:

    std::vector<genie::core::meta::Region> decapsulate(uint16_t dataset) {
        std::vector<genie::core::meta::Region> ret;
        if(dataset != dataset_ID) {
            return ret;
        }
        for(auto& r : dataset_regions) {
            ret.emplace_back(r.decapsulate());
        }
        return ret;
    }

    /**
     * @brief
     * @param other
     * @return
     */
    bool operator==(const LabelDataset& other) const;

    /**
     * @brief
     * @param _ds_ID
     */
    explicit LabelDataset(uint16_t _ds_ID);
    /**
     * @brief
     * @param reader
     */
    explicit LabelDataset(util::BitReader& reader);

    /**
     * @brief
     * @param _ds_region
     */
    void addDatasetRegion(LabelRegion _ds_region);

    /**
     * @brief
     * @return
     */
    uint16_t getDatasetID() const;

    /**
     * @brief
     * @return
     */
    uint64_t getBitLength() const;

    /**
     * @brief
     * @param bit_writer
     */
    void write(genie::util::BitWriter& bit_writer) const;
};

// ---------------------------------------------------------------------------------------------------------------------

}  // namespace mpegg_p1
}  // namespace format
}  // namespace genie

// ---------------------------------------------------------------------------------------------------------------------

#endif  // SRC_GENIE_FORMAT_MPEGG_P1_LABEL_DATASET_H_

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
