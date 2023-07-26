/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/mitogen/genie for more details.
 */

#include <atomic>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include "genie/util/make-unique.h"
#include "genie/util/watch.h"

#include "codecs/api/mpegg_utils.h"
#include "codecs/include/mpegg-codecs.h"
#include "genie/entropy/bsc/encoder.h"

// ---------------------------------------------------------------------------------------------------------------------

namespace genie {
namespace entropy {
namespace bsc {

// ---------------------------------------------------------------------------------------------------------------------

BSCEncoder::BSCEncoder()
    : lzpHashSize(MPEGG_BSC_DEFAULT_LZPHASHSIZE),
      lzpMinLen(MPEGG_BSC_DEFAULT_LZPMINLEN),
      blockSorter(MPEGG_BSC_BLOCKSORTER_BWT),
      coder(MPEGG_BSC_CODER_QLFC_STATIC) {}

void BSCEncoder::encode(std::stringstream &input, std::stringstream &output) {
    const size_t srcLen = input.str().size();
    char *dest = new char[srcLen];
    unsigned char *destination;
    size_t destLen = srcLen;

    int ret = mpegg_bsc_compress(&destination, &destLen, (const unsigned char *)input.str().c_str(), srcLen,
                                 lzpHashSize, lzpMinLen, blockSorter, coder);
    if (ret != 0) {
        std::cerr << "error with compression\n";
    }
    output.write((const char *)destination, destLen);
    delete[] dest;
}

void BSCEncoder::decode(std::stringstream &input, std::stringstream &output) {
    const size_t srcLen = input.str().size();
    char *dest = new char[srcLen];
    unsigned char *destination;
    size_t destLen = srcLen;
    int ret = mpegg_bsc_decompress(&destination, &destLen, (const unsigned char *)input.str().c_str(), srcLen);
    if (ret != 0) {
        std::cerr << "error with decompression\n";
    }
    output.write((const char *)destination, destLen);
    delete[] dest;
}

genie::core::record::annotation_parameter_set::AlgorithmParameters BSCParameters::convertToAlgorithmParameters()
    const {
    uint8_t n_pars = 4;
    std::vector<uint8_t> par_ID{1, 2, 3, 4};
    std::vector<uint8_t> par_type{4, 4, 4, 4};
    std::vector<uint8_t> par_num_array_dims(4, 0);
    std::vector<uint8_t> values{MPEGG_BSC_DEFAULT_LZPHASHSIZE, MPEGG_BSC_DEFAULT_LZPMINLEN, MPEGG_BSC_BLOCKSORTER_BWT,
                                MPEGG_BSC_CODER_QLFC_STATIC};

    std::vector<std::vector<uint8_t>> par_array_dims(0, std::vector<uint8_t>(1, 0));
    //   genie::core::arrayType toar;
    std::vector<std::vector<std::vector<std::vector<uint8_t>>>> temp;
    std::vector<std::vector<std::vector<uint8_t>>> temp2;
    std::vector<std::vector<uint8_t>> temp3;
    std::vector<uint8_t> temp4(1, 0);
    temp3.resize(n_pars, temp4);
    temp2.resize(1, temp3);
    temp.resize(1, temp2);
    std::vector<std::vector<std::vector<std::vector<std::vector<uint8_t>>>>> par_val(1, temp);

    for (auto l = 0; l < par_val.size(); ++l) {
        for (auto k = 0; k < par_val[l].size(); ++k)
            for (auto j = 0; j < par_val[l][k].size(); ++j)
                for (auto i = 0; i < par_val[l][k][j].size(); ++i) {
                    par_val[l][k][j][i][0] = values[i];
                }
    }

    genie::core::record::annotation_parameter_set::AlgorithmParameters algorithmParameters(
        n_pars, par_ID, par_type, par_num_array_dims, par_array_dims, par_val);
    return genie::core::record::annotation_parameter_set::AlgorithmParameters(
        n_pars, par_ID, par_type, par_num_array_dims, par_array_dims, par_val);
}

}  // namespace bsc
}  // namespace entropy
}  // namespace genie

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
