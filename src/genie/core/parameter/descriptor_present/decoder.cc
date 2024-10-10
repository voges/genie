/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/MueFab/genie for more details.
 */

#include "genie/core/parameter/descriptor_present/decoder.h"

// ---------------------------------------------------------------------------------------------------------------------

namespace genie::core::parameter::desc_pres {

// ---------------------------------------------------------------------------------------------------------------------

void Decoder::write(util::BitWriter& writer) const { writer.writeBits(encoding_mode_ID, 8); }

// ---------------------------------------------------------------------------------------------------------------------

uint8_t Decoder::getMode() const { return encoding_mode_ID; }

// ---------------------------------------------------------------------------------------------------------------------

Decoder::Decoder(uint8_t _encoding_mode_id) : encoding_mode_ID(_encoding_mode_id) {}

// ---------------------------------------------------------------------------------------------------------------------

bool Decoder::equals(const Decoder* dec) const { return encoding_mode_ID == dec->encoding_mode_ID; }

// ---------------------------------------------------------------------------------------------------------------------

}  // namespace genie::core::parameter::desc_pres

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
