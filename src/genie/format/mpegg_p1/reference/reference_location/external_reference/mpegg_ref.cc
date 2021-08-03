/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/mitogen/genie for more details.
 */

#include "genie/format/mpegg_p1/reference/reference_location/external_reference/mpegg_ref.h"
#include <utility>
#include "genie/format/mpegg_p1/reference/reference_location/external_reference/md5.h"
#include "genie/format/mpegg_p1/reference/reference_location/external_reference/sha256.h"
#include "genie/util/make-unique.h"

// ---------------------------------------------------------------------------------------------------------------------

namespace genie {
namespace format {
namespace mpegg_p1 {


// ---------------------------------------------------------------------------------------------------------------------

MpegReference::MpegReference(uint8_t _dataset_group_ID, uint16_t _dataset_ID, std::unique_ptr<Checksum> _ref_checksum)
    : ExternalReference(ExternalReference::Type::MPEGG_REF),
      dataset_group_ID(_dataset_group_ID),
      dataset_ID(_dataset_ID),
      ref_checksum(std::move(_ref_checksum)) {}

// ---------------------------------------------------------------------------------------------------------------------

MpegReference::MpegReference(util::BitReader& reader, FileHeader& fhd, Checksum::Algo checksum_alg)
    : dataset_group_ID(reader.read<uint8_t>()), dataset_ID(reader.read<uint8_t>()), ref_checksum() {

    if (fhd.getMinorVersion() == "1900") {
        switch (checksum_alg) {
            case Checksum::Algo::MD5: {
                ref_checksum = genie::util::make_unique<Md5>(reader);
                break;
            }
            case Checksum::Algo::SHA256: {
                ref_checksum = genie::util::make_unique<Sha256>(reader);
                break;
            }
            default: {
                UTILS_DIE("Unsupported checksum algorithm");
                break;
            }
        }
    }

}

// ---------------------------------------------------------------------------------------------------------------------

uint8_t MpegReference::getDatasetGroupID() const { return dataset_group_ID; }

// ---------------------------------------------------------------------------------------------------------------------

uint16_t MpegReference::getDatasetID() const { return dataset_ID; }

// ---------------------------------------------------------------------------------------------------------------------

Checksum::Algo MpegReference::getChecksumAlg() const { return ref_checksum->getType(); }

// ---------------------------------------------------------------------------------------------------------------------

uint64_t MpegReference::getLength() const {
    // TODO(Raouf): Check this one
    // external_dataset_group_ID u(8), external_dataset_ID u(16)
    uint64_t len = 1 + 2;
    len += ref_checksum->getLength();

    return len;
}

// ---------------------------------------------------------------------------------------------------------------------

void MpegReference::write(util::BitWriter& writer) const {
    writer.write(dataset_group_ID, 8);
    writer.write(dataset_ID, 16);
    ref_checksum->write(writer);
}

// ---------------------------------------------------------------------------------------------------------------------

}  // namespace mpegg_p1
}  // namespace format
}  // namespace genie

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
