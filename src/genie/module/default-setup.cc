/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/mitogen/genie for more details.
 */

#include "default-setup.h"
#include <genie/read/localassembly/decoder.h>
#include <genie/read/localassembly/encoder.h>
#include <genie/read/refcoder/decoder.h>
#include <genie/read/refcoder/encoder.h>
//#include <genie/read/spring/encoder.h>
#include <genie/core/classifier-bypass.h>
#include <genie/entropy/gabac/gabac-compressor.h>
#include <genie/entropy/gabac/gabac-decompressor.h>
#include <genie/name/tokenizer/decoder.h>
#include <genie/name/tokenizer/encoder.h>
#include <genie/quality/qvwriteout/decoder.h>
#include <genie/quality/qvwriteout/encoder.h>
#include <genie/read/spring/spring-encoder.h>

// ---------------------------------------------------------------------------------------------------------------------

namespace genie {
namespace module {

// ---------------------------------------------------------------------------------------------------------------------

std::unique_ptr<core::FlowGraphEncode> buildDefaultEncoder(size_t threads, const std::string& working_dir, size_t) {
    std::unique_ptr<core::FlowGraphEncode> ret = genie::util::make_unique<core::FlowGraphEncode>(threads);

    ret->setClassifier(genie::util::make_unique<genie::core::ClassifierBypass>());

    ret->addReadCoder(genie::util::make_unique<genie::read::spring::SpringEncoder>(working_dir));
    ret->addReadCoder(genie::util::make_unique<genie::read::localassembly::Encoder>(2048, false));
    ret->addReadCoder(genie::util::make_unique<genie::read::refcoder::Encoder>());
    ret->setReadCoderSelector([](const genie::core::record::Chunk&) -> size_t { return 1; });

    ret->addQVCoder(genie::util::make_unique<genie::quality::qvwriteout::Encoder>());
    ret->setQVSelector([](const genie::core::record::Chunk&) -> size_t { return 0; });

    ret->addNameCoder(genie::util::make_unique<genie::name::tokenizer::Encoder>());
    ret->setNameSelector([](const genie::core::record::Chunk&) -> size_t { return 0; });

    ret->addEntropyCoder(genie::util::make_unique<genie::entropy::gabac::GabacCompressor>());
    ret->setEntropyCoderSelector([](const genie::core::AccessUnitRaw&) -> size_t { return 0; });

    ret->setExporterSelector([](const genie::core::AccessUnitPayload&) -> size_t { return 0; });

    return ret;
}

// ---------------------------------------------------------------------------------------------------------------------

std::unique_ptr<core::FlowGraphDecode> buildDefaultDecoder(size_t threads, const std::string&, size_t) {
    std::unique_ptr<core::FlowGraphDecode> ret = genie::util::make_unique<core::FlowGraphDecode>(threads);

    // ret->addReadCoder(genie::util::make_unique<genie::read::spring::Decoder>(working_dir, nullptr));
    ret->addReadCoder(genie::util::make_unique<genie::read::localassembly::Decoder>());
    ret->addReadCoder(genie::util::make_unique<genie::read::localassembly::Decoder>());
    ret->addReadCoder(genie::util::make_unique<genie::read::refcoder::Decoder>());
    ret->setReadCoderSelector([](const genie::core::AccessUnitRaw&) -> size_t { return 1; });

    ret->addQVCoder(genie::util::make_unique<genie::quality::qvwriteout::Decoder>());
    ret->setQVSelector([](const genie::core::parameter::QualityValues&, const std::vector<std::string>&,
                          genie::core::AccessUnitRaw::Descriptor&) -> size_t { return 0; });

    ret->addNameCoder(genie::util::make_unique<genie::name::tokenizer::Decoder>());
    ret->setNameSelector([](const genie::core::AccessUnitRaw::Descriptor&) -> size_t { return 0; });

    ret->addEntropyCoder(genie::util::make_unique<genie::entropy::gabac::GabacDecompressor>());
    ret->setEntropyCoderSelector([](const genie::core::AccessUnitPayload&) -> size_t { return 0; });

    ret->setExporterSelector([](const genie::core::record::Chunk&) -> size_t { return 0; });

    return ret;
}

// ---------------------------------------------------------------------------------------------------------------------

}  // namespace module
}  // namespace genie

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------