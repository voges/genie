/**
* @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/mitogen/genie for more details.
 */

#include "genie/entropy/bsc/subsequence.h"

// ---------------------------------------------------------------------------------------------------------------------

namespace genie::entropy::bsc {

// ---------------------------------------------------------------------------------------------------------------------

Subsequence::Subsequence(uint8_t _output_symbol_size) : output_symbol_size(_output_symbol_size) {}

// ---------------------------------------------------------------------------------------------------------------------

uint8_t Subsequence::getOutputSymbolSize() const { return output_symbol_size; }

// ---------------------------------------------------------------------------------------------------------------------

void Subsequence::write(util::BitWriter &writer) const { writer.write(output_symbol_size, 6); }

// ---------------------------------------------------------------------------------------------------------------------

bool Subsequence::operator==(const Subsequence &rhs) const { return output_symbol_size == rhs.output_symbol_size; }

// ---------------------------------------------------------------------------------------------------------------------

}  // namespace genie::entropy::bsc

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
