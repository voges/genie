/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/mitogen/genie for more details.
 */

#ifndef GABAC_DECODE_CABAC_H_
#define GABAC_DECODE_CABAC_H_

#include <cstdint>
#include <vector>

#include <genie/entropy/paramcabac/transformed-subseq.h>

namespace genie {
namespace util {
class DataBlock;
}
}  // namespace genie

namespace genie {
namespace entropy {
namespace gabac {

size_t decodeTransformSubseq(const paramcabac::TransformedSubSeq& trnsfSubseqConf, const unsigned int numEncodedSymbols,
                             util::DataBlock* bitstream, util::DataBlock* const depSymbols = nullptr);

}  // namespace gabac
}  // namespace entropy
}  // namespace genie
#endif  // GABAC_DECODE_CABAC_H_
