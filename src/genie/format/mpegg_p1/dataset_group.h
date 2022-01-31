/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/mitogen/genie for more details.
 */

#ifndef SRC_GENIE_FORMAT_MPEGG_P1_DATASET_GROUP_H_
#define SRC_GENIE_FORMAT_MPEGG_P1_DATASET_GROUP_H_

// ---------------------------------------------------------------------------------------------------------------------

#include <boost/optional/optional.hpp>
#include <vector>
#include "genie/core/constants.h"
#include "genie/format/mpegg_p1/dataset.h"
#include "genie/format/mpegg_p1/dataset_group_header.h"
#include "genie/format/mpegg_p1/dataset_group_metadata.h"
#include "genie/format/mpegg_p1/dataset_group_protection.h"
#include "genie/format/mpegg_p1/gen_info.h"
#include "genie/format/mpegg_p1/label_list.h"
#include "genie/format/mpegg_p1/reference.h"
#include "genie/format/mpegg_p1/reference_metadata.h"

// ---------------------------------------------------------------------------------------------------------------------

namespace genie {
namespace format {
namespace mpegg_p1 {

/**
 * @brief
 */
class DatasetGroup : public GenInfo {
 private:
    DatasetGroupHeader header;                           //!< @brief
    std::vector<Reference> references;                   //!< @brief
    std::vector<ReferenceMetadata> reference_metadatas;  //!< @brief
    boost::optional<LabelList> labels;                   //!< @brief
    boost::optional<DatasetGroupMetadata> metadata;      //!< @brief
    boost::optional<DatasetGroupProtection> protection;  //!< @brief
    std::vector<Dataset> dataset;                        //!< @brief

    core::MPEGMinorVersion version;  //!< @brief

 public:
    /**
     * @brief
     * @param info
     * @return
     */
    bool operator==(const GenInfo& info) const override;

    /**
     * @brief
     * @param reader
     * @param _version
     */
    explicit DatasetGroup(util::BitReader& reader, core::MPEGMinorVersion _version);

    /**
     * @brief
     * @param _ID
     * @param _version
     * @param _mpeg_version
     */
    DatasetGroup(uint8_t _ID, uint8_t _version, core::MPEGMinorVersion _mpeg_version);

    /**
     * @brief
     * @param ds
     */
    void addDataset(Dataset ds);

    /**
     * @brief
     * @return
     */
    const DatasetGroupHeader& getHeader() const;

    /**
     * @brief
     * @return
     */
    const std::vector<Reference>& getReferences() const;

    /**
     * @brief
     * @return
     */
    const std::vector<ReferenceMetadata>& getReferenceMetadata() const;

    /**
     * @brief
     * @return
     */
    bool hasLabelList() const;

    /**
     * @brief
     * @return
     */
    const LabelList& getLabelList() const;

    /**
     * @brief
     * @return
     */
    bool hasMetadata() const;

    /**
     * @brief
     * @return
     */
    const DatasetGroupMetadata& getMetadata() const;

    /**
     * @brief
     * @return
     */
    bool hasProtection() const;

    /**
     * @brief
     * @return
     */
    const DatasetGroupProtection& getProtection() const;

    /**
     * @brief
     * @param md
     */
    void addMetadata(DatasetGroupMetadata md);

    /**
     * @brief
     * @param pr
     */
    void addProtection(DatasetGroupProtection pr);

    /**
     * @brief
     * @return
     */
    const std::vector<Dataset>& getDatasets() const;

    /**
     * @brief
     * @return
     */
    uint64_t getSize() const override;

    /**
     * @brief
     * @return
     */
    const std::string& getKey() const override;

    /**
     * @brief
     * @param wr
     */
    void write(util::BitWriter& wr) const override;
};

// ---------------------------------------------------------------------------------------------------------------------

}  // namespace mpegg_p1
}  // namespace format
}  // namespace genie

// ---------------------------------------------------------------------------------------------------------------------

#endif  // SRC_GENIE_FORMAT_MPEGG_P1_DATASET_GROUP_H_

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
