#include "VariantSiteParser.h"
/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/mitogen/genie for more details.
 */

#include <algorithm>
#include <string>
#include <utility>

#include "genie/core/record/annotation_access_unit/AnnotationAccessUnitHeader.h"
#include "genie/core/record/annotation_access_unit/block.h"
#include "genie/util/bitreader.h"
#include "genie/util/bitwriter.h"
#include "genie/util/make-unique.h"
#include "genie/util/runtime-exception.h"
#include "genie/variantsite/AccessUnitComposer.h"

#include "genie/core/record/variant_genotype/record.h"
#include "genie/core/record/variant_site/record.h"

#include "genie/entropy/bsc/encoder.h"
#include "genie/entropy/lzma/encoder.h"
#include "genie/entropy/zstd/encoder.h"

// ---------------------------------------------------------------------------------------------------------------------

namespace genie {
namespace variant_site {

void AccessUnitComposer::setAccessUnit(
    std::map<core::AnnotDesc, std::stringstream>& _descriptorStream,
    std::map<std::string, core::record::annotation_access_unit::TypedData>& _attributeTileStream,
    std::map<std::string, core::record::annotation_parameter_set::AttributeData> _attributeInfo,
    const core::record::annotation_parameter_set::Record& _annotationParameterSet,
    core::record::annotation_access_unit::Record& _annotationAccessUnit, uint8_t _AG_class, uint8_t _AT_ID,
    uint64_t _rowIndex) {
    tile_index_1 = _rowIndex;
    std::vector<genie::core::record::annotation_access_unit::Block> blocks;
    AT_ID = _AT_ID;
    AG_class = _AG_class;
    // -------- descriptors ---------- //
    auto descriptorConfigurations =
        _annotationParameterSet.getAnnotationEncodingParameters().getDescriptorConfigurations();
    std::map<core::AnnotDesc, std::stringstream> encodedDescriptors;
    compress(descriptorConfigurations, _descriptorStream, encodedDescriptors);

    for (auto it = encodedDescriptors.begin(); it != encodedDescriptors.end(); ++it) {
        std::vector<uint8_t> data;
        for (auto readbyte : it->second.str()) data.push_back(readbyte);
        genie::core::record::annotation_access_unit::BlockVectorData blockInfo(it->first, 0, data);
        genie::core::record::annotation_access_unit::Block block;
        block.set(blockInfo);
        blocks.push_back(block);
    }

    // -------- attributes ----------- //
    auto attributeParameterSets = _annotationParameterSet.getAnnotationEncodingParameters().getAttributeParameterSets();
    auto compressorParameterSets =
        _annotationParameterSet.getAnnotationEncodingParameters().getCompressorParameterSets();
    std::map<std::string, std::stringstream> encodedAttributes;

    for (auto& attribute : attributeParameterSets) {
        auto compressorId = attribute.getCompressorID();
        {
            std::string filename = attribute.getAttributeName() + "_before.bin";
            std::ofstream output;
            output.open(filename, std::ios::binary);
            auto& temp = _attributeTileStream[attribute.getAttributeName()].getdata();
            output.write(temp.str().c_str(),temp.str().length());
            output.close();
        }
        if (compressorId != 0) {
            compress(_attributeTileStream[attribute.getAttributeName()], compressorParameterSets.at(compressorId - 1));

            {
                std::string filename = attribute.getAttributeName() + "_after.bin";
                std::ofstream output;
                output.open(filename, std::ios::binary);
                auto& temp = _attributeTileStream[attribute.getAttributeName()].getCompresseddata();
                output.write(temp.str().c_str(), temp.str().length());
                output.close();
            }
        }
    }

    for (auto& tile : _attributeTileStream) {
        std::stringstream data;
        genie::core::Writer writer(&data);
        tile.second.write(writer);
        writer.flush();
        auto attributeID = _attributeInfo[tile.first].getAttributeID();
        core::record::annotation_access_unit::BlockData blockInfo(core::AnnotDesc::ATTRIBUTE, attributeID, data);
        genie::core::record::annotation_access_unit::Block block;
        block.set(blockInfo);
        blocks.push_back(block);
    }

    // ------------------------------- //

    uint64_t n_blocks = static_cast<uint64_t>(blocks.size());
    ATCoordSize = _annotationParameterSet.getATCoordSize();
    genie::core::record::annotation_access_unit::AnnotationAccessUnitHeader annotationAccessUnitHeadertot(
        attributeContiguity, twoDimensional, columnMajorTileOrder, variable_size_tiles, ATCoordSize, is_attribute,
        attribute_ID, descriptor_ID, n_tiles_per_col, n_tiles_per_row, n_blocks, tile_index_1, tile_index_2_exists,
        tile_index_2);
    genie::core::record::annotation_access_unit::Record annotationAccessUnit(
        AT_ID, AT_type, AT_subtype, AG_class, annotationAccessUnitHeadertot, blocks, attributeContiguity,
        twoDimensional, columnMajorTileOrder, ATCoordSize, variable_size_tiles, n_blocks, numChrs);
    _annotationAccessUnit = annotationAccessUnit;
}

void AccessUnitComposer::compress(
    const std::vector<genie::core::record::annotation_parameter_set::DescriptorConfiguration>& descriptorConfigurations,
    std::map<genie::core::AnnotDesc, std::stringstream>& inputstream,
    std::map<genie::core::AnnotDesc, std::stringstream>& encodedDescriptors) {
    genie::entropy::bsc::BSCEncoder bscEncoder;
    genie::entropy::lzma::LZMAEncoder lzmaEncoder;
    genie::entropy::zstd::ZSTDEncoder zstdEncoder;
    for (auto encodingpar : descriptorConfigurations) {
        switch (encodingpar.getEncodingModeID()) {
            case genie::core::AlgoID::BSC:
                bscEncoder.encode(inputstream[encodingpar.getDescriptorID()],
                                  encodedDescriptors[encodingpar.getDescriptorID()]);
                break;
            case genie::core::AlgoID::LZMA:
                lzmaEncoder.encode(inputstream[encodingpar.getDescriptorID()],
                                   encodedDescriptors[encodingpar.getDescriptorID()]);
                break;
            case genie::core::AlgoID::ZSTD:
                zstdEncoder.encode(inputstream[encodingpar.getDescriptorID()],
                                   encodedDescriptors[encodingpar.getDescriptorID()]);
                break;

            default:
                break;
        }
    }
}

void AccessUnitComposer::compress(
    std::map<std::string, std::stringstream>& attributeStream,
    const std::vector<genie::core::record::annotation_parameter_set::AttributeParameterSet>& attributeParameterSets,
    const std::vector<genie::core::record::annotation_parameter_set::CompressorParameterSet>& compressorParameterSets,
    std::map<std::string, std::stringstream>& encodedAttributes) {
    genie::entropy::bsc::BSCEncoder bscEncoder;
    genie::entropy::lzma::LZMAEncoder lzmaEncoder;
    genie::entropy::zstd::ZSTDEncoder zstdEncoder;

    for (auto& attribute : attributeParameterSets) {
        auto attributeName = attribute.getAttributeName();
        auto compressorID = attribute.getCompressorID();
        if (compressorID == 0) {
            encodedAttributes[attributeName] << attributeStream[attributeName].rdbuf();
        } else {
            auto encodeID = compressorParameterSets[compressorID - 1].getAlgorithmIDs();

            switch (encodeID[0]) {
                case genie::core::AlgoID::BSC:
                    bscEncoder.encode(attributeStream[attributeName], encodedAttributes[attributeName]);
                    break;
                case genie::core::AlgoID::LZMA:
                    lzmaEncoder.encode(attributeStream[attributeName], encodedAttributes[attributeName]);
                    break;
                case genie::core::AlgoID::ZSTD:
                    zstdEncoder.encode(attributeStream[attributeName], encodedAttributes[attributeName]);
                    break;

                default:
                    encodedAttributes[attributeName] << attributeStream[attributeName].rdbuf();
                    break;
            }
        }
    }
}

void AccessUnitComposer::compress(genie::core::record::annotation_access_unit::TypedData& oneBlock,
                                  genie::core::record::annotation_parameter_set::CompressorParameterSet& compressor) {
    auto encodeId = compressor.getAlgorithmIDs();
    if (compressor.getCompressorID() != 0) {
        std::stringstream compressedData;
        compressors.compress(oneBlock.getdata(), compressedData, compressor.getCompressorID());
        oneBlock.setCompressedData(compressedData);
    }
}

}  // namespace variant_site
}  // namespace genie

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
