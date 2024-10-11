/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/MueFab/genie for more details.
 */

#include "genie/format/mgb/au_type_cfg.h"
#include <utility>
#include "genie/util/bit-writer.h"

// ---------------------------------------------------------------------------------------------------------------------

namespace genie::format::mgb {

// ---------------------------------------------------------------------------------------------------------------------

bool AuTypeCfg::operator==(const AuTypeCfg &other) const {
    return sequence_ID == other.sequence_ID && AU_start_position == other.AU_start_position &&
           AU_end_position == other.AU_end_position && extended_AU == other.extended_AU && posSize == other.posSize;
}

// ---------------------------------------------------------------------------------------------------------------------

void AuTypeCfg::write(util::BitWriter &writer) const {
    writer.writeBits(sequence_ID, 16);
    writer.writeBits(AU_start_position, posSize);
    writer.writeBits(AU_end_position, posSize);
    if (extended_AU) {
        extended_AU->write(writer);
    }
}

// ---------------------------------------------------------------------------------------------------------------------

AuTypeCfg::AuTypeCfg(const uint16_t _sequence_ID, const uint64_t _aU_start_position, const uint64_t _aU_end_position,
                     const uint8_t _posSize)
    : sequence_ID(_sequence_ID),
      AU_start_position(_aU_start_position),
      AU_end_position(_aU_end_position),
      posSize(_posSize) {}

// ---------------------------------------------------------------------------------------------------------------------

AuTypeCfg::AuTypeCfg(const uint8_t _posSize) : AuTypeCfg(0, 0, 0, _posSize) {}

// ---------------------------------------------------------------------------------------------------------------------

AuTypeCfg::AuTypeCfg(const uint8_t _posSize, const bool multiple_alignments, util::BitReader &reader)
    : posSize(_posSize) {
    sequence_ID = reader.read<uint16_t>();
    AU_start_position = reader.read<uint64_t>(posSize);
    AU_end_position = reader.read<uint64_t>(posSize);
    if (multiple_alignments) {
        extended_AU = ExtendedAu(_posSize, reader);
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void AuTypeCfg::setExtendedAu(ExtendedAu &&_extended_AU) { extended_AU = std::move(_extended_AU); }

// ---------------------------------------------------------------------------------------------------------------------

uint16_t AuTypeCfg::getRefID() const { return sequence_ID; }

// ---------------------------------------------------------------------------------------------------------------------

uint64_t AuTypeCfg::getStartPos() const { return AU_start_position; }

// ---------------------------------------------------------------------------------------------------------------------

uint64_t AuTypeCfg::getEndPos() const { return AU_end_position; }

// ---------------------------------------------------------------------------------------------------------------------

}  // namespace genie::format::mgb

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
