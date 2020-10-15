#include "mpegg_ref.h"
#include "checksum.h"
#include "md5.h"
#include "sha256.h"

namespace genie {
namespace format {
namespace mpegg_p1 {

// ---------------------------------------------------------------------------------------------------------------------

MpegReference::MpegReference(util::BitReader& reader, Checksum::Algo checksum_alg)
    : dataset_group_ID(reader.read<uint8_t>()),
      dataset_ID(reader.read<uint8_t>()),
      ref_checksum(){

    switch (checksum_alg) {
        case Checksum::Algo::MD5: {
            ref_checksum = Md5(reader);
            break;
        }
        case Checksum::Algo::SHA256: {
            ref_checksum = Sha256(reader);
            break;
        }
        default: {
            UTILS_DIE("Unsupported checksum algorithm");
            break;
        }
    }
}


// ---------------------------------------------------------------------------------------------------------------------

MpegReference::MpegReference(uint8_t _dataset_group_ID, uint16_t _dataset_ID, Checksum &&_ref_checksum)
    : ExternalReference(ExternalReference::Type::MPEGG_REF),
      dataset_group_ID(_dataset_group_ID),
      dataset_ID(_dataset_ID),
      ref_checksum(_ref_checksum){}

// ---------------------------------------------------------------------------------------------------------------------

uint8_t MpegReference::getDatasetGroupID() const { return dataset_group_ID; }

// ---------------------------------------------------------------------------------------------------------------------

uint16_t MpegReference::getDatasetID() const { return dataset_ID; }

// ---------------------------------------------------------------------------------------------------------------------

void MpegReference::addChecksum(Checksum&& _checksum) {
    ref_checksum = _checksum;
}

// ---------------------------------------------------------------------------------------------------------------------

uint64_t MpegReference::getLength() const {

}

// ---------------------------------------------------------------------------------------------------------------------

void MpegReference::write(util::BitWriter& writer) {
    writer.write(dataset_group_ID, 8);
    writer.write(dataset_ID, 16);

    switch (ref_checksum.getType()) {
        case Checksum::Algo::MD5: {
            dynamic_cast<Md5*>(&ref_checksum)->write(writer);
            break;
        }
        case Checksum::Algo::SHA256: {
            dynamic_cast<Sha256*>(&ref_checksum)->write(writer);
            break;
        }
        default: {
            UTILS_DIE("Unsupported checksum algorithm");
            break;
        }
    }
}

// ---------------------------------------------------------------------------------------------------------------------

}  // namespace mpegg_p1
}  // namespace format
}  // namespace genie