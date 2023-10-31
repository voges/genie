/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/mitogen/genie for more details.
 */

#ifndef SRC_GENIE_VARIANT_SITE_PARAMETERSETCOMPOSER_H_
#define SRC_GENIE_VARIANT_SITE_PARAMETERSETCOMPOSER_H_

// ---------------------------------------------------------------------------------------------------------------------
#include <cstdint>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "genie/core/arrayType.h"
#include "genie/core/constants.h"
#include "genie/core/writer.h"
#include "genie/util/bitreader.h"

#include "genie/core/record/annotation_parameter_set/record.h"
#include "genie/entropy/bsc/encoder.h"
#include "genie/entropy/lzma/encoder.h"
#include "genie/entropy/zstd/encoder.h"

// ---------------------------------------------------------------------------------------------------------------------

namespace genie {
namespace variant_site {

class ParameterSetComposer {
 public:
    genie::core::record::annotation_parameter_set::Record setParameterSet(
        std::map<std::string, genie::core::record::annotation_parameter_set::AttributeData>& info,
        uint64_t defaultTileSize);

    ParameterSetComposer(std::map<genie::core::AnnotDesc, std::stringstream>& _encodedDescriptors)
        : encodedDescriptors(_encodedDescriptors){};

    genie::core::record::annotation_parameter_set::Record setParameterSet(
        std::map<std::string, genie::core::record::annotation_parameter_set::AttributeData>& info,
        genie::genotype::GenotypeParameters genotype_parameters, genie::core::AlgoID encodeMode,
        uint64_t defaultTileSize);

 private:
    std::map<genie::core::AnnotDesc, std::stringstream>& encodedDescriptors;
};

// ---------------------------------------------------------------------------------------------------------------------

}  // namespace variant_site
}  // namespace genie

// ---------------------------------------------------------------------------------------------------------------------

#endif  // SRC_GENIE_VARIANT_SITE_PARAMETERSETCOMPOSER_H_

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
