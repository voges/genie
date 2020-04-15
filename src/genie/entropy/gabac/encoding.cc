/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/mitogen/genie for more details.
 */

#include "encoding.h"

#include <algorithm>
#include <cmath>
#include <vector>

#include <genie/util/block-stepper.h>
#include <genie/util/data-block.h>
#include <genie/entropy/paramcabac/transformed-subseq.h>
#include "configuration.h"
#include "encode-cabac.h"
#include "stream-handler.h"
#include "writer.h"
#include "exceptions.h"

#include "equality-subseq-transform.h"
#include "match-subseq-transform.h"
#include "rle-subseq-transform.h"
#include "merge-subseq-transform.h"

namespace genie {
namespace entropy {
namespace gabac {

static inline
void doSubsequenceTransform(const paramcabac::Subsequence &subseqCfg,
                         std::vector<util::DataBlock> *const transformedSubseqs) {
    // GABACIFY_LOG_TRACE << "Encoding sequence of length: " <<
    // (*transformedSequences)[0].size();

    // GABACIFY_LOG_DEBUG << "Performing sequence transformation " <<
    // gabac::transformationInformation[id].name;

    const paramcabac::TransformedParameters& trnsfSubseqParams = subseqCfg.getTransformParameters();
    const uint16_t param = trnsfSubseqParams.getParam(); // get first param

    switch(trnsfSubseqParams.getTransformIdSubseq()) {
        case paramcabac::TransformedParameters::TransformIdSubseq::NO_TRANSFORM:
            transformedSubseqs->resize(1);
        break;
        case paramcabac::TransformedParameters::TransformIdSubseq::EQUALITY_CODING:
            transformedSubseqs->resize(2);
            (*transformedSubseqs)[1] = util::DataBlock(0, 1); // FIXME shouldn't this wsize=1 be based on outputSymbolSize?
            transformEqualityCoding(&(*transformedSubseqs)[0], &(*transformedSubseqs)[1]);
        break;
        case paramcabac::TransformedParameters::TransformIdSubseq::MATCH_CODING:
            transformedSubseqs->resize(3);
            assert(param <= std::numeric_limits<uint32_t>::max());
            (*transformedSubseqs)[1] = util::DataBlock(0, 4); // FIXME shouldn't this wsize=4 be based on outputSymbolSize?
            (*transformedSubseqs)[2] = util::DataBlock(0, 4); // FIXME shouldn't this wsize=4 be based on outputSymbolSize?
            transformMatchCoding(static_cast<uint32_t>(param), &(*transformedSubseqs)[0],
                                 &(*transformedSubseqs)[1], &(*transformedSubseqs)[2]);
        break;
        case paramcabac::TransformedParameters::TransformIdSubseq::RLE_CODING:
            transformedSubseqs->resize(2);
            (*transformedSubseqs)[1] = util::DataBlock(0, 1); // FIXME shouldn't this wsize=1 be based on outputSymbolSize?
            transformRleCoding(param, &(*transformedSubseqs)[0], &(*transformedSubseqs)[1]);
        break;
        case paramcabac::TransformedParameters::TransformIdSubseq::MERGE_CODING:
            transformMergeCoding(subseqCfg, transformedSubseqs);
        break;
        default:
            GABAC_DIE("Invalid subseq transforamtion");
        break;
    }

    // GABACIFY_LOG_TRACE << "Got " << transformedSequences->size() << "
    // sequences";
    for (unsigned i = 0; i < transformedSubseqs->size(); ++i) {
        // GABACIFY_LOG_TRACE << i << ": " << (*transformedSequences)[i].size()
        // << " bytes";
    }
}

unsigned long encodeDescSubsequence(const IOConfiguration &conf, const EncodingConfiguration &enConf) {
    conf.validate();
    util::DataBlock subsequence(0, 4);  //FIXME
    util::DataBlock dependency(0, 4);   //FIXME
    size_t numDescSubseqSymbols = 0;
    size_t subseqPayloadSize = 0;

    numDescSubseqSymbols = gabac::StreamHandler::readFull(*conf.inputStream, &subsequence);
    if(conf.dependencyStream != nullptr) {
        if(numDescSubseqSymbols != gabac::StreamHandler::readFull(*conf.dependencyStream, &dependency)) {
            GABAC_DIE("Size mismatch between dependency and descriptor subsequence");
        }
    }

    if(numDescSubseqSymbols > 0) {
        // write number of symbols in descriptor subsequence
        if(enConf.getSubseqConfig().getTokentypeFlag()) {
            subseqPayloadSize += gabac::StreamHandler::writeU7(*conf.outputStream, numDescSubseqSymbols);
        } else {
            subseqPayloadSize += gabac::StreamHandler::writeUInt(*conf.outputStream, numDescSubseqSymbols, 4);
        }

        // Insert subsequence into vector
        std::vector<util::DataBlock> transformedSubseqs;
        transformedSubseqs.resize(1);
        transformedSubseqs[0].swap(&subsequence);

        // Put descriptor subsequence, get transformed subsequences out
        doSubsequenceTransform(enConf.getSubseqConfig(), &transformedSubseqs);
        const size_t numTrnsfSubseqs = transformedSubseqs.size();

        // Loop through the transformed sequences
        for (size_t i = 0; i < numTrnsfSubseqs; i++) {
            uint64_t numtrnsfSymbols = transformedSubseqs[i].size();
            uint64_t trnsfSubseqPayloadSize = 0;
            if(numtrnsfSymbols > 0) {
                // Encoding
                gabac::encode_cabac(enConf.subseq.getTransformSubseqCfg(i),
                                    &(transformedSubseqs[i]),
                                    (dependency.size()) ? &dependency : nullptr);

                trnsfSubseqPayloadSize = transformedSubseqs[i].getRawSize();
            }

            if(i < (numTrnsfSubseqs-1)) {
                subseqPayloadSize += gabac::StreamHandler::writeUInt(*conf.outputStream, trnsfSubseqPayloadSize, 4);
            }

            if(numTrnsfSubseqs > 1) {
                subseqPayloadSize += gabac::StreamHandler::writeUInt(*conf.outputStream, numtrnsfSymbols, 4);
            }

            if(trnsfSubseqPayloadSize > 0) {
                subseqPayloadSize += gabac::StreamHandler::writeBytes(*conf.outputStream, &transformedSubseqs[i]);
            }
        }
    }

    return subseqPayloadSize;
}
}  // namespace gabac
}  // namespace entropy
}  // namespace genie
