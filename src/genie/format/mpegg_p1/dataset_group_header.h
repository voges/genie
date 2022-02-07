/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/mitogen/genie for more details.
 */

#ifndef SRC_GENIE_FORMAT_MPEGG_P1_DATASET_GROUP_HEADER_H_
#define SRC_GENIE_FORMAT_MPEGG_P1_DATASET_GROUP_HEADER_H_

// ---------------------------------------------------------------------------------------------------------------------

#include <string>
#include <vector>
#include "genie/format/mpegg_p1/gen_info.h"
#include "genie/util/bitreader.h"

// ---------------------------------------------------------------------------------------------------------------------

namespace genie {
namespace format {
namespace mpegg_p1 {

/**
 * @brief
 */
class DatasetGroupHeader : public GenInfo {
 private:
    uint8_t ID;                         //!< @brief
    uint8_t version;                    //!< @brief
    std::vector<uint16_t> dataset_IDs;  //!< @brief

 public:

    void patchID(uint8_t groupID) {
        ID = groupID;
    }

    /**
     * @brief
     * @param info
     * @return
     */
    bool operator==(const GenInfo& info) const override;

    /**
     * @brief
     */
    DatasetGroupHeader();

    /**
     * @brief
     */
    DatasetGroupHeader(uint8_t _id, uint8_t _version);

    /**
     * @brief
     * @return
     */
    const std::string& getKey() const override;

    /**
     * @brief
     * @param reader
     */
    explicit DatasetGroupHeader(genie::util::BitReader& reader);

    /**
     * @brief
     * @return
     */
    uint8_t getID() const;

    /**
     * @brief
     * @param _ID
     */
    void setID(uint8_t _ID);

    /**
     * @brief
     * @return
     */
    uint8_t getVersion() const;

    /**
     * @brief
     * @return
     */
    const std::vector<uint16_t>& getDatasetIDs() const;

    /**
     * @brief
     * @param _id
     */
    void addDatasetID(uint16_t _id);

    /**
     * @brief
     * @param writer
     */
    void box_write(genie::util::BitWriter& writer) const override;

    void print_debug(std::ostream& output, uint8_t depth, uint8_t max_depth) const override {
        print_offset(output, depth, max_depth, "* Dataset Group Header");
        print_offset(output, depth + 1, max_depth, "ID: " + std::to_string(int(ID)));
        print_offset(output, depth + 1, max_depth, "Version: " + std::to_string(int(version)));
        for (const auto &id : dataset_IDs) {
            print_offset(output, depth + 1, max_depth, "Dataset ID: " + std::to_string(int(id)));
        }
    }
};

// ---------------------------------------------------------------------------------------------------------------------

}  // namespace mpegg_p1
}  // namespace format
}  // namespace genie

// ---------------------------------------------------------------------------------------------------------------------

#endif  // SRC_GENIE_FORMAT_MPEGG_P1_DATASET_GROUP_HEADER_H_

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
