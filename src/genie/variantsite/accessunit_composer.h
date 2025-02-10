/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/mitogen/genie for more details.
 */

#ifndef SRC_GENIE_VARIANTSITE_ACCESSUNITCOMPOSER_H_
#define SRC_GENIE_VARIANTSITE_ACCESSUNITCOMPOSER_H_

// ---------------------------------------------------------------------------------------------------------------------
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "genie/core/arrayType.h"
#include "genie/core/constants.h"
#include "genie/core/writer.h"
#include "genie/util/bit_reader.h"

#include "genie/annotation/compressors.h"
#include "genie/core/record/annotation_access_unit/TypedData.h"
#include "genie/core/record/annotation_access_unit/record.h"
#include "genie/core/record/annotation_parameter_set/AttributeData.h"
#include "genie/core/record/annotation_parameter_set/AttributeParameterSet.h"
#include "genie/core/record/annotation_parameter_set/CompressorParameterSet.h"
#include "genie/core/record/annotation_parameter_set/DescriptorConfiguration.h"
#include "genie/core/record/annotation_parameter_set/record.h"
// ---------------------------------------------------------------------------------------------------------------------

namespace genie {
namespace variant_site {

class AccessUnitComposer {
 public:
    void setCompressors(genie::annotation::Compressor _compressors) { compressors = _compressors; }
    //    void setParameterSet(const core::record::annotation_parameter_set::Record& _annotationParameterSet) {
    //       annotationParameterSet = _annotationParameterSet;
    // AG_class = annotationParameterSet;
    //   }
    void setATtype(core::record::annotation_access_unit::AnnotationType ATtype, uint8_t ATsubtype) {
        AT_type = ATtype;
        AT_subtype = ATsubtype;
    };


    void setAccessUnit(std::map<core::AnnotDesc, std::stringstream>& descriptorStream,
                       std::map<std::string, core::record::annotation_access_unit::TypedData>& attributeTileStream,
                       std::map<std::string, core::record::annotation_parameter_set::AttributeData> AttributeInfo,
                       const core::record::annotation_parameter_set::Record& annotationParameterSet,
                       core::record::annotation_access_unit::Record& annotationAccessUnit, uint8_t AG_class,
                       uint8_t AT_ID, uint64_t _rowIndex);

    void setAccessUnit(std::map<core::AnnotDesc, std::stringstream>& descriptorStream,
                       std::map<std::string, core::record::annotation_access_unit::TypedData>& attributeTileStream,
                       std::map<std::string, core::record::annotation_parameter_set::AttributeData> AttributeInfo,
                       const core::record::annotation_parameter_set::Record& annotationParameterSet,
                       core::record::annotation_access_unit::Record& annotationAccessUnit, uint8_t AG_class,
                       uint8_t AT_ID, uint64_t _rowIndex, uint64_t _colIndex);

 private:
    genie::annotation::Compressor compressors;
    //  core::record::annotation_parameter_set::Record& annotationParameterSet;
    // default values
    bool attributeContiguity = false;
    bool twoDimensional = false;
    bool columnMajorTileOrder = false;
    uint8_t ATCoordSize = 3;
    bool variable_size_tiles = false;
    uint8_t AT_subtype = 1;
    uint64_t n_tiles_per_col = 1;
    uint64_t n_tiles_per_row = 1;
    uint64_t tile_index_1 = 0;
    bool tile_index_2_exists = false;
    uint64_t tile_index_2 = 0;
    bool is_attribute = false;
    uint16_t attribute_ID = 0;
    core::AnnotDesc descriptor_ID = core::AnnotDesc::STARTPOS;
    uint8_t AT_ID = 1;
    uint8_t AG_class = 0;
    core::record::annotation_access_unit::AnnotationType AT_type {core::record::annotation_access_unit::AnnotationType::VARIANTS};
    uint8_t numChrs = 0;

    void compress(const std::vector<genie::core::record::annotation_parameter_set::DescriptorConfiguration>&
                      descriptorConfigurations,
                  std::map<genie::core::AnnotDesc, std::stringstream>& inputstream,
                  std::map<genie::core::AnnotDesc, std::stringstream>& encodedDescriptors);
    void compress(
        std::map<std::string, std::stringstream>& attributeStream,
        const std::vector<genie::core::record::annotation_parameter_set::AttributeParameterSet>& attributeParameterSets,
        const std::vector<genie::core::record::annotation_parameter_set::CompressorParameterSet>&
            compressorParameterSets,
        std::map<std::string, std::stringstream>& encodedAttributes);

    void compress(genie::core::record::annotation_access_unit::TypedData& oneBlock,
                  genie::core::record::annotation_parameter_set::CompressorParameterSet& compressor);
};

// ---------------------------------------------------------------------------------------------------------------------

}  // namespace variant_site
}  // namespace genie

// ---------------------------------------------------------------------------------------------------------------------

#endif  // SRC_GENIE_VARIANTSITE_ACCESSUNITCOMPOSER_H_

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
