/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/MueFab/genie for more details.
 */

#include "genie/format/mgg/dataset_group_header.h"
#include <string>
#include <vector>
#include "genie/util/runtime-exception.h"

// ---------------------------------------------------------------------------------------------------------------------

namespace genie::format::mgg {

// ---------------------------------------------------------------------------------------------------------------------

bool DatasetGroupHeader::operator==(const GenInfo& info) const {
    if (!GenInfo::operator==(info)) {
        return false;
    }
    const auto& other = dynamic_cast<const DatasetGroupHeader&>(info);
    return ID == other.ID && version == other.version && dataset_IDs == other.dataset_IDs;
}

// ---------------------------------------------------------------------------------------------------------------------

DatasetGroupHeader::DatasetGroupHeader() : DatasetGroupHeader(0, 0) {}

// ---------------------------------------------------------------------------------------------------------------------

DatasetGroupHeader::DatasetGroupHeader(uint8_t _id, uint8_t _version) : ID(_id), version(_version) {}

// ---------------------------------------------------------------------------------------------------------------------

const std::string& DatasetGroupHeader::getKey() const {
    static const std::string key = "dghd";
    return key;
}

// ---------------------------------------------------------------------------------------------------------------------

DatasetGroupHeader::DatasetGroupHeader(util::BitReader& reader) {
    auto start_pos = reader.getStreamPosition() - 4;
    auto length = reader.readAlignedInt<uint64_t>();
    auto num_datasets = (length - 14) / 2;
    dataset_IDs.resize(num_datasets);
    ID = reader.readAlignedInt<uint8_t>();
    version = reader.readAlignedInt<uint8_t>();
    for (auto& d : dataset_IDs) {
        d = reader.readAlignedInt<uint16_t>();
    }

    UTILS_DIE_IF(start_pos + length != static_cast<uint64_t>(reader.getStreamPosition()), "Invalid length");
}

// ---------------------------------------------------------------------------------------------------------------------

uint8_t DatasetGroupHeader::getID() const { return ID; }

// ---------------------------------------------------------------------------------------------------------------------

void DatasetGroupHeader::setID(uint8_t _id) { ID = _id; }

// ---------------------------------------------------------------------------------------------------------------------

uint8_t DatasetGroupHeader::getVersion() const { return version; }

// ---------------------------------------------------------------------------------------------------------------------

const std::vector<uint16_t>& DatasetGroupHeader::getDatasetIDs() const { return dataset_IDs; }

// ---------------------------------------------------------------------------------------------------------------------

void DatasetGroupHeader::addDatasetID(uint8_t _id) { dataset_IDs.emplace_back(_id); }

// ---------------------------------------------------------------------------------------------------------------------

void DatasetGroupHeader::box_write(util::BitWriter& writer) const {
    writer.writeAlignedInt<uint8_t>(ID);
    writer.writeAlignedInt<uint8_t>(version);
    for (auto& d : dataset_IDs) {
        writer.writeAlignedInt<uint16_t>(d);
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void DatasetGroupHeader::patchID(uint8_t groupID) { ID = groupID; }

// ---------------------------------------------------------------------------------------------------------------------

void DatasetGroupHeader::print_debug(std::ostream& output, uint8_t depth, uint8_t max_depth) const {
    print_offset(output, depth, max_depth, "* Dataset Group Header");
    print_offset(output, depth + 1, max_depth, "ID: " + std::to_string(ID));
    print_offset(output, depth + 1, max_depth, "Version: " + std::to_string(version));
    for (const auto& id : dataset_IDs) {
        print_offset(output, depth + 1, max_depth, "Dataset ID: " + std::to_string(id));
    }
}

// ---------------------------------------------------------------------------------------------------------------------

}  // namespace genie::format::mgg

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
