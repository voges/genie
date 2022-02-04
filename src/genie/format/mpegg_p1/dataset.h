/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/mitogen/genie for more details.
 */

#ifndef SRC_GENIE_FORMAT_MPEGG_P1_DATASET_H_
#define SRC_GENIE_FORMAT_MPEGG_P1_DATASET_H_

// ---------------------------------------------------------------------------------------------------------------------

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "genie/core/meta/blockheader/disabled.h"
#include "genie/core/meta/blockheader/enabled.h"
#include "genie/core/meta/dataset.h"
#include "genie/format/mgb/mgb_file.h"
#include "genie/format/mpegg_p1/access_unit.h"
#include "genie/format/mpegg_p1/dataset_header.h"
#include "genie/format/mpegg_p1/dataset_metadata.h"
#include "genie/format/mpegg_p1/dataset_parameter_set.h"
#include "genie/format/mpegg_p1/dataset_protection.h"
#include "genie/format/mpegg_p1/descriptor_stream.h"
#include "genie/format/mpegg_p1/gen_info.h"
#include "genie/format/mpegg_p1/master_index_table.h"

#define GENIE_DEBUG_PRINT_NODETAIL

// ---------------------------------------------------------------------------------------------------------------------

namespace genie {
namespace format {
namespace mpegg_p1 {

/**
 * @brief
 */
class Dataset : public GenInfo {
 private:
    DatasetHeader header;                                  //!< @brief
    boost::optional<DatasetMetadata> metadata;             //!< @brief
    boost::optional<DatasetProtection> protection;         //!< @brief
    std::vector<DatasetParameterSet> parameterSets;        //!< @brief
    boost::optional<MasterIndexTable> master_index_table;  //!< @brief
    std::vector<AccessUnit> access_units;                  //!< @brief
    std::vector<DescriptorStream> descriptor_streams;      //!< @brief
    core::MPEGMinorVersion version;                        //!< @brief

    std::map<size_t, core::parameter::EncodingSet> encoding_sets;  //!< @brief

 public:
    /**
     * @brief
     * @param reader
     * @param _version
     */
    Dataset(util::BitReader& reader, core::MPEGMinorVersion _version);

    Dataset(format::mgb::MgbFile& file, const core::meta::Dataset& meta, core::MPEGMinorVersion _version, const std::vector<uint8_t>& param_ids)
        : version(_version) {
        bool mitFlag = false;
        bool cc_mode = false;
        bool ordered_blocks = false;
        bool headerON = meta.getHeader().getType() == core::meta::BlockHeader::HeaderType::ENABLED;
        if (headerON) {
            cc_mode = dynamic_cast<const core::meta::blockheader::Enabled&>(meta.getHeader()).getCCFlag();
            mitFlag = dynamic_cast<const core::meta::blockheader::Enabled&>(meta.getHeader()).getMITFlag();
        } else {
            ordered_blocks = dynamic_cast<const core::meta::blockheader::Disabled&>(meta.getHeader()).getOrderedFlag();
        }

        if (!headerON) {
            if (ordered_blocks) {
                file.sort_by_position();
            }
            for (size_t c = 0; c < size_t(core::record::ClassType::COUNT); ++c) {
                for (size_t d = 0; d < size_t(core::GenDesc::COUNT); ++d) {
                    auto blocks = file.extractDescriptor(core::record::ClassType(c), core::GenDesc(d), param_ids);
                    auto desc = DescriptorStream(core::GenDesc(d), core::record::ClassType(c), blocks);
                    if (!desc.isEmpty()) {
                        descriptor_streams.emplace_back(std::move(desc));
                    }
                }
            }
            file.clearAUBlocks(param_ids);
        } else {
            if (cc_mode) {
                file.sort_by_class();
            } else {
                file.sort_by_position();
            }
        }

        auto params_p2 = file.extractParameters(param_ids);
        for (auto& p : params_p2) {
            parameterSets.emplace_back(0, 0, p->getID(), p->getParentID(), std::move(p->getEncodingSet()), version);
        }



        auto access_units_p2 = file.extractAUs(param_ids);

        for (auto& a : access_units_p2) {
            access_units.emplace_back(std::move(*a), mitFlag, version);
        }

        header = DatasetHeader(0, 0, version, parameterSets.front().getEncodingSet().hasMultipleAlignments(), true, false,
                               parameterSets.front().getEncodingSet().getPosSize() == 40, parameterSets.front().getEncodingSet().getDatasetType(), false,
                               parameterSets.front().getEncodingSet().getAlphabetID());


    }

    /**
     * @brief
     * @return
     */
    const DatasetHeader& getHeader() const;

    /**
     * @brief
     * @param bitWriter
     */
    void box_write(genie::util::BitWriter& bitWriter) const override;

    /**
     * @brief
     * @return
     */
    const std::string& getKey() const override;

    void read_box(util::BitReader& reader, bool in_offset) {
        std::string tmp_str(4, '\0');
        reader.readBypass(tmp_str);
        if (tmp_str == "dtmd") {
            UTILS_DIE_IF(metadata != boost::none, "Metadata already present");
            UTILS_DIE_IF(protection != boost::none, "Metadata must be before protection");
            UTILS_DIE_IF(!parameterSets.empty(), "Metadata must be before parametersets");
            UTILS_DIE_IF(master_index_table != boost::none, "Metadata must be before MIT");
            UTILS_DIE_IF(!access_units.empty(), "Metadata must be before Access Units");
            UTILS_DIE_IF(!descriptor_streams.empty(), "Metadata must be before Descriptor streams");
            metadata = DatasetMetadata(reader, version);
        } else if (tmp_str == "dtpr") {
            UTILS_DIE_IF(protection != boost::none, "Protection already present");
            UTILS_DIE_IF(!parameterSets.empty(), "Metadata must be before parametersets");
            UTILS_DIE_IF(master_index_table != boost::none, "Metadata must be before MIT");
            UTILS_DIE_IF(!access_units.empty(), "Metadata must be before Access Units");
            UTILS_DIE_IF(!descriptor_streams.empty(), "Metadata must be before Descriptor streams");
            protection = DatasetProtection(reader, version);
        } else if (tmp_str == "pars") {
            UTILS_DIE_IF(in_offset, "Offset not permitted");
            UTILS_DIE_IF(master_index_table != boost::none, "Metadata must be before MIT");
            UTILS_DIE_IF(!access_units.empty(), "Metadata must be before Access Units");
            UTILS_DIE_IF(!descriptor_streams.empty(), "Metadata must be before Descriptor streams");
            parameterSets.emplace_back(reader, version, header.getParameterUpdateFlag());
            encoding_sets.emplace(std::make_pair(size_t(parameterSets.back().getParameterSetID()),
                                                 parameterSets.back().getEncodingSet()));
        } else if (tmp_str == "mitb") {
            UTILS_DIE_IF(in_offset, "Offset not permitted");
            UTILS_DIE_IF(master_index_table != boost::none, "MIT already present");
            UTILS_DIE_IF(!access_units.empty(), "Metadata must be before Access Units");
            UTILS_DIE_IF(!descriptor_streams.empty(), "Metadata must be before Descriptor streams");
            master_index_table = MasterIndexTable(reader, header);
        } else if (tmp_str == "aucn") {
            UTILS_DIE_IF(in_offset, "Offset not permitted");
            UTILS_DIE_IF(!descriptor_streams.empty(), "Metadata must be before Descriptor streams");
            access_units.emplace_back(reader, encoding_sets, header.isMITEnabled(), header.isBlockHeaderEnabled(),
                                      version);
        } else if (tmp_str == "dscn") {
            UTILS_DIE_IF(in_offset, "Offset not permitted");
            UTILS_DIE_IF(master_index_table == boost::none, "descriptor streams without MIT not allowed");
            UTILS_DIE_IF(header.isBlockHeaderEnabled(), "descriptor streams only allowed without block headers");
            descriptor_streams.emplace_back(reader, *master_index_table, header.getMITConfigs());
        } else if (tmp_str == "offs") {
            UTILS_DIE_IF(in_offset, "Recursive offset not permitted");
            reader.readBypass(tmp_str);
            uint64_t offset = reader.readBypassBE<uint64_t>();
            if (offset == ~static_cast<uint64_t>(0)) {
                return;
            }
            auto pos_save = reader.getPos();
            reader.setPos(offset);
            read_box(reader, true);
            reader.setPos(pos_save);
        } else {
            UTILS_DIE("Unknown box");
        }
    }

    void print_debug(std::ostream& output, uint8_t depth, uint8_t max_depth) const override {
        print_offset(output, depth, max_depth, "* Dataset");
        header.print_debug(output, depth + 1, max_depth);
        if (metadata) {
            metadata->print_debug(output, depth + 1, max_depth);
        }
        if (protection) {
            metadata->print_debug(output, depth + 1, max_depth);
        }
        for (const auto& r : parameterSets) {
            r.print_debug(output, depth + 1, max_depth);
        }
        if (master_index_table) {
            master_index_table->print_debug(output, depth + 1, max_depth);
        }
#ifdef GENIE_DEBUG_PRINT_NODETAIL
        if (!access_units.empty()) {
            print_offset(output, depth + 1, max_depth, "* " + std::to_string(access_units.size()) + " access units");
        }
        if (!descriptor_streams.empty()) {
            print_offset(output, depth + 1, max_depth,
                         "* " + std::to_string(descriptor_streams.size()) + " descriptor streams");
        }
#else
        for (const auto& r : access_units) {
            r.print_debug(output, depth + 1, max_depth);
        }
        for (const auto& r : descriptor_streams) {
            r.print_debug(output, depth + 1, max_depth);
        }
#endif
    }
};

// ---------------------------------------------------------------------------------------------------------------------

}  // namespace mpegg_p1
}  // namespace format
}  // namespace genie

// ---------------------------------------------------------------------------------------------------------------------

#endif  // SRC_GENIE_FORMAT_MPEGG_P1_DATASET_H_

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
