/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/MueFab/genie for more details.
 */

#include "genie/format/mgg/dataset_group.h"
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

// ---------------------------------------------------------------------------------------------------------------------

namespace genie::format::mgg {

// ---------------------------------------------------------------------------------------------------------------------

bool DatasetGroup::operator==(const GenInfo& info) const {
    if (!GenInfo::operator==(info)) {
        return false;
    }
    const auto& other = dynamic_cast<const DatasetGroup&>(info);
    return header == other.header && references == other.references &&
           reference_metadatas == other.reference_metadatas && labels == other.labels && metadata == other.metadata &&
           protection == other.protection && dataset == other.dataset;
}

// ---------------------------------------------------------------------------------------------------------------------

DatasetGroup::DatasetGroup(util::BitReader& reader, core::MPEGMinorVersion _version) : version(_version) {
    auto start_pos = reader.getStreamPosition() - 4;
    auto length = reader.readAlignedInt<uint64_t>();
    auto end_pos = start_pos + static_cast<int64_t>(length);
    while (reader.getStreamPosition() != end_pos) {
        UTILS_DIE_IF(reader.getStreamPosition() > end_pos,
                     "Read " + std::to_string(reader.getStreamPosition() - end_pos) + " bytes too far");
        read_box(reader, false);
    }

    std::cout << std::to_string(end_pos - reader.getStreamPosition()) << std::endl;
    UTILS_DIE_IF(header == std::nullopt, "Datasetgroup without header");
}

// ---------------------------------------------------------------------------------------------------------------------

DatasetGroup::DatasetGroup(uint8_t _id, uint8_t _version, core::MPEGMinorVersion _mpeg_version)
    : header(DatasetGroupHeader(_id, _version)), version(_mpeg_version) {}

// ---------------------------------------------------------------------------------------------------------------------

void DatasetGroup::addDataset(Dataset ds) {
    dataset.emplace_back(std::move(ds));
    header->addDatasetID(static_cast<uint8_t>(dataset.back().getHeader().getDatasetID()));
}

// ---------------------------------------------------------------------------------------------------------------------

const DatasetGroupHeader& DatasetGroup::getHeader() const { return *header; }

// ---------------------------------------------------------------------------------------------------------------------

const std::vector<Reference>& DatasetGroup::getReferences() const { return references; }

// ---------------------------------------------------------------------------------------------------------------------

const std::vector<ReferenceMetadata>& DatasetGroup::getReferenceMetadata() const { return reference_metadatas; }

// ---------------------------------------------------------------------------------------------------------------------

bool DatasetGroup::hasLabelList() const { return labels != std::nullopt; }

// ---------------------------------------------------------------------------------------------------------------------

const LabelList& DatasetGroup::getLabelList() const { return *labels; }

// ---------------------------------------------------------------------------------------------------------------------

bool DatasetGroup::hasMetadata() const { return metadata != std::nullopt; }

// ---------------------------------------------------------------------------------------------------------------------

const DatasetGroupMetadata& DatasetGroup::getMetadata() const { return *metadata; }

// ---------------------------------------------------------------------------------------------------------------------

bool DatasetGroup::hasProtection() const { return protection != std::nullopt; }

// ---------------------------------------------------------------------------------------------------------------------

const DatasetGroupProtection& DatasetGroup::getProtection() const { return *protection; }

// ---------------------------------------------------------------------------------------------------------------------

void DatasetGroup::addMetadata(DatasetGroupMetadata md) { metadata = std::move(md); }

// ---------------------------------------------------------------------------------------------------------------------

void DatasetGroup::addProtection(DatasetGroupProtection pr) { protection = std::move(pr); }

// ---------------------------------------------------------------------------------------------------------------------

const std::vector<Dataset>& DatasetGroup::getDatasets() const { return dataset; }

// ---------------------------------------------------------------------------------------------------------------------

const std::string& DatasetGroup::getKey() const {
    static const std::string key = "dgcn";
    return key;
}

// ---------------------------------------------------------------------------------------------------------------------

void DatasetGroup::box_write(util::BitWriter& wr) const {
    header->write(wr);
    for (const auto& r : references) {
        r.write(wr);
    }
    for (const auto& r : reference_metadatas) {
        r.write(wr);
    }
    if (labels != std::nullopt) {
        labels->write(wr);
    }
    if (metadata != std::nullopt) {
        metadata->write(wr);
    }
    if (protection != std::nullopt) {
        protection->write(wr);
    }
    for (const auto& d : dataset) {
        d.write(wr);
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void DatasetGroup::setMetadata(DatasetGroupMetadata meta) { metadata = std::move(meta); }

// ---------------------------------------------------------------------------------------------------------------------

void DatasetGroup::setProtection(DatasetGroupProtection prot) { protection = std::move(prot); }

// ---------------------------------------------------------------------------------------------------------------------

void DatasetGroup::addReference(Reference r) { references.emplace_back(std::move(r)); }

// ---------------------------------------------------------------------------------------------------------------------

void DatasetGroup::addReferenceMeta(ReferenceMetadata r) { reference_metadatas.emplace_back(std::move(r)); }

// ---------------------------------------------------------------------------------------------------------------------

void DatasetGroup::setLabels(LabelList l) { labels = std::move(l); }

// ---------------------------------------------------------------------------------------------------------------------

void DatasetGroup::patchID(uint8_t groupID) {
    if (header != std::nullopt) {
        header->patchID(groupID);
    }
    for (auto& r : references) {
        r.patchID(groupID);
    }
    for (auto& r : reference_metadatas) {
        r.patchID(groupID);
    }
    if (labels != std::nullopt) {
        labels->patchID(groupID);
    }
    if (metadata != std::nullopt) {
        metadata->patchID(groupID);
    }
    if (protection != std::nullopt) {
        protection->patchID(groupID);
    }
    for (auto& d : dataset) {
        d.patchID(groupID, d.getHeader().getDatasetID());
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void DatasetGroup::patchRefID(uint8_t _old, uint8_t _new) {
    for (auto& r : references) {
        r.patchRefID(_old, _new);
    }
    for (auto& r : reference_metadatas) {
        r.patchRefID(_old, _new);
    }
    for (auto& d : dataset) {
        d.patchRefID(_old, _new);
    }
}

// ---------------------------------------------------------------------------------------------------------------------

std::vector<Reference>& DatasetGroup::getReferences() { return references; }

// ---------------------------------------------------------------------------------------------------------------------

std::vector<ReferenceMetadata>& DatasetGroup::getReferenceMetadata() { return reference_metadatas; }

// ---------------------------------------------------------------------------------------------------------------------

LabelList& DatasetGroup::getLabelList() { return *labels; }

// ---------------------------------------------------------------------------------------------------------------------

DatasetGroupMetadata& DatasetGroup::getMetadata() { return *metadata; }

// ---------------------------------------------------------------------------------------------------------------------

DatasetGroupProtection& DatasetGroup::getProtection() { return *protection; }

// ---------------------------------------------------------------------------------------------------------------------

std::vector<Dataset>& DatasetGroup::getDatasets() { return dataset; }

// ---------------------------------------------------------------------------------------------------------------------

void DatasetGroup::print_debug(std::ostream& output, uint8_t depth, uint8_t max_depth) const {
    print_offset(output, depth, max_depth, "* Dataset Group");
    header->print_debug(output, depth + 1, max_depth);
    for (const auto& r : references) {
        r.print_debug(output, depth + 1, max_depth);
    }
    for (const auto& r : reference_metadatas) {
        r.print_debug(output, depth + 1, max_depth);
    }
    if (labels) {
        labels->print_debug(output, depth + 1, max_depth);
    }
    if (metadata) {
        metadata->print_debug(output, depth + 1, max_depth);
    }
    if (protection) {
        metadata->print_debug(output, depth + 1, max_depth);
    }
    for (const auto& r : dataset) {
        r.print_debug(output, depth + 1, max_depth);
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void DatasetGroup::read_box(util::BitReader& reader, bool in_offset) {
    std::string tmp_str(4, '\0');
    reader.readAlignedBytes(tmp_str.data(), tmp_str.length());
    if (tmp_str == "dghd") {
        UTILS_DIE_IF(header != std::nullopt, "More than one header");
        UTILS_DIE_IF(!reference_metadatas.empty(), "Header must be before ref metadata");
        UTILS_DIE_IF(labels != std::nullopt, "Header must be before labels");
        UTILS_DIE_IF(metadata != std::nullopt, "Header must be before metadata");
        UTILS_DIE_IF(protection != std::nullopt, "Header must be before protection");
        UTILS_DIE_IF(!dataset.empty(), "Header must be before dataset");
        header = DatasetGroupHeader(reader);
    } else if (tmp_str == "rfgn") {
        UTILS_DIE_IF(!reference_metadatas.empty(), "Reference must be before ref metadata");
        UTILS_DIE_IF(labels != std::nullopt, "Reference must be before labels");
        UTILS_DIE_IF(metadata != std::nullopt, "Reference must be before metadata");
        UTILS_DIE_IF(protection != std::nullopt, "Reference must be before protection");
        UTILS_DIE_IF(!dataset.empty(), "Reference must be before dataset");
        references.emplace_back(reader, version);
    } else if (tmp_str == "rfmd") {
        UTILS_DIE_IF(labels != std::nullopt, "Ref metadata must be before labels");
        UTILS_DIE_IF(metadata != std::nullopt, "Ref metadata must be before metadata");
        UTILS_DIE_IF(protection != std::nullopt, "Ref metadata must be before protection");
        UTILS_DIE_IF(!dataset.empty(), "Ref metadata must be before dataset");
        reference_metadatas.emplace_back(reader);
    } else if (tmp_str == "labl") {
        UTILS_DIE_IF(labels != std::nullopt, "Labels already present");
        UTILS_DIE_IF(metadata != std::nullopt, "Labels must be before metadata");
        UTILS_DIE_IF(protection != std::nullopt, "Labels must be before protection");
        UTILS_DIE_IF(!dataset.empty(), "Labels must be before dataset");
        labels.emplace(reader);
    } else if (tmp_str == "dgmd") {
        UTILS_DIE_IF(metadata != std::nullopt, "Metadata already present");
        UTILS_DIE_IF(protection != std::nullopt, "Metadata must be before protection");
        UTILS_DIE_IF(!dataset.empty(), "Metadata must be before dataset");
        metadata = DatasetGroupMetadata(reader, version);
    } else if (tmp_str == "dgpr") {
        UTILS_DIE_IF(protection != std::nullopt, "Protection already present");
        UTILS_DIE_IF(!dataset.empty(), "Protection must be before dataset");
        protection = DatasetGroupProtection(reader, version);
    } else if (tmp_str == "dtcn") {
        dataset.emplace_back(reader, version);
    } else if (tmp_str == "offs") {
        UTILS_DIE_IF(in_offset, "Recursive offset not permitted");
        reader.readAlignedBytes(tmp_str.data(), tmp_str.length());
        auto offset = reader.readAlignedInt<uint64_t>();
        if (offset == ~static_cast<uint64_t>(0)) {
            return;
        }
        auto pos_save = reader.getStreamPosition();
        reader.setStreamPosition(offset);
        read_box(reader, true);
        reader.setStreamPosition(pos_save);
    } else {
        UTILS_DIE("Unknown box");
    }
}

// ---------------------------------------------------------------------------------------------------------------------

}  // namespace genie::format::mgg

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
