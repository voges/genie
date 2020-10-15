#include "genie/util/runtime-exception.h"
#include "label_list.h"

namespace genie {
namespace format {
namespace mpegg_p1 {

// ---------------------------------------------------------------------------------------------------------------------

LabelList::LabelList()
    : dataset_group_ID() {}

// ---------------------------------------------------------------------------------------------------------------------

LabelList::LabelList(uint8_t _ds_group_ID)
    : dataset_group_ID(_ds_group_ID) {}

// ---------------------------------------------------------------------------------------------------------------------

LabelList::LabelList(uint8_t _ds_group_ID, std::vector<Label> &&_labels)
    : dataset_group_ID(_ds_group_ID),
      labels(std::move(_labels)){}

// ---------------------------------------------------------------------------------------------------------------------

void LabelList::setLabels(std::vector<Label>&& _labels) {
    labels = _labels;
}

// ---------------------------------------------------------------------------------------------------------------------

void LabelList::setDatasetGroupId(uint8_t _dataset_group_ID) {dataset_group_ID = _dataset_group_ID;}

// ---------------------------------------------------------------------------------------------------------------------

uint8_t LabelList::getDatasetGroupID() const { return dataset_group_ID; }

// ---------------------------------------------------------------------------------------------------------------------

uint16_t LabelList::getNumLabels() const { return (uint16_t) labels.size(); }

// ---------------------------------------------------------------------------------------------------------------------

const std::vector<Label>& LabelList::getLabels() const { return labels; }

// ---------------------------------------------------------------------------------------------------------------------

uint64_t LabelList::getLength() const {
    // key (4), Length (8)
    uint64_t len = 12;

    // TODO (Yeremia): len of Value[]?

    // dataset_group_ID u(8)
    len += 1;

    // num_labels u(16)
    len += 2;

    // for Label
    for (auto& label: labels){
        len += label.getLength();
    }

    return len;
}

// ---------------------------------------------------------------------------------------------------------------------

void LabelList::writeToFile(util::BitWriter& bit_writer) const {
    // KLV (Key Length Value) format

    // Key of KVL format
    bit_writer.write("labl");

    // Length of KVL format
    bit_writer.write(getLength(), 64);

    // dataset_group_ID u(8)
    bit_writer.write(dataset_group_ID, 8);

    // num_labels u(16)
    bit_writer.write(getNumLabels(), 16);

    // for Label
    for (auto& label: labels){
        label.write(bit_writer);
    }
}

// ---------------------------------------------------------------------------------------------------------------------

}  // namespace mpegg_p1
}  // namespace format
}  // namespace genie