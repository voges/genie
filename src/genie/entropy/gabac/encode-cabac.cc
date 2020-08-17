/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/mitogen/genie for more details.
 */

#include "encode-cabac.h"

#include <cassert>

#include <genie/util/block-stepper.h>
#include <genie/util/data-block.h>
#include "writer.h"

#include "context-selector.h"
#include "luts-subsymbol-transform.h"

// ---------------------------------------------------------------------------------------------------------------------

namespace genie {
namespace entropy {
namespace gabac {

// ---------------------------------------------------------------------------------------------------------------------

typedef void (Writer::*binFunc)(uint64_t, const std::vector<unsigned int>);

// ---------------------------------------------------------------------------------------------------------------------

static inline void encodeSignflag(Writer &writer, const paramcabac::BinarizationParameters::BinarizationId binID,
                                  const int64_t signedSymbolValue) {
    if (signedSymbolValue != 0) {
        switch (binID) {
            case paramcabac::BinarizationParameters::BinarizationId::SIGNED_EXPONENTIAL_GOMB:
            case paramcabac::BinarizationParameters::BinarizationId::SIGNED_TRUNCATED_EXPONENTIAL_GOLOMB:
            case paramcabac::BinarizationParameters::BinarizationId::SIGNED_SPLIT_UNITWISE_TRUNCATED_UNARY:
            case paramcabac::BinarizationParameters::BinarizationId::SIGNED_DOUBLE_TRUNCATED_UNARY:
                writer.writeSignFlag(signedSymbolValue);
                break;
            default:
                break;
        }
    }
}

// ---------------------------------------------------------------------------------------------------------------------

static inline binFunc getBinarizor(const uint8_t outputSymbolSize, const bool bypassFlag,
                                   const paramcabac::BinarizationParameters::BinarizationId binID,
                                   const paramcabac::BinarizationParameters &binarzationParams,
                                   const paramcabac::StateVars &stateVars, std::vector<unsigned int> &binParams) {
    binFunc func = nullptr;
    if (bypassFlag) {
        switch (binID) {
            case paramcabac::BinarizationParameters::BinarizationId::BINARY_CODING:
                func = &Writer::writeAsBIbypass;
                binParams[0] = stateVars.getCLengthBI();
                break;
            case paramcabac::BinarizationParameters::BinarizationId::TRUNCATED_UNARY:
                func = &Writer::writeAsTUbypass;
                binParams[0] = binarzationParams.getCMax();
                break;
            case paramcabac::BinarizationParameters::BinarizationId::EXPONENTIAL_GOLOMB:
            case paramcabac::BinarizationParameters::BinarizationId::SIGNED_EXPONENTIAL_GOMB:
                func = &Writer::writeAsEGbypass;
                break;
            case paramcabac::BinarizationParameters::BinarizationId::TRUNCATED_EXPONENTIAL_GOLOMB:
            case paramcabac::BinarizationParameters::BinarizationId::SIGNED_TRUNCATED_EXPONENTIAL_GOLOMB:
                func = &Writer::writeAsTEGbypass;
                binParams[0] = binarzationParams.getCMaxTeg();
                break;
            case paramcabac::BinarizationParameters::BinarizationId::SPLIT_UNITWISE_TRUNCATED_UNARY:
            case paramcabac::BinarizationParameters::BinarizationId::SIGNED_SPLIT_UNITWISE_TRUNCATED_UNARY:
                func = &Writer::writeAsSUTUbypass;
                binParams[0] = outputSymbolSize;
                binParams[1] = binarzationParams.getSplitUnitSize();
                break;
            case paramcabac::BinarizationParameters::BinarizationId::DOUBLE_TRUNCATED_UNARY:
            case paramcabac::BinarizationParameters::BinarizationId::SIGNED_DOUBLE_TRUNCATED_UNARY:
                func = &Writer::writeAsDTUbypass;
                binParams[0] = outputSymbolSize;
                binParams[1] = binarzationParams.getSplitUnitSize();
                binParams[2] = binarzationParams.getCMaxDtu();
                break;
            default:
                UTILS_DIE("Unknown Binarization");
        }
    } else {
        switch (binID) {
            case paramcabac::BinarizationParameters::BinarizationId::BINARY_CODING:
                func = &Writer::writeAsBIcabac;
                binParams[0] = stateVars.getCLengthBI();
                break;
            case paramcabac::BinarizationParameters::BinarizationId::TRUNCATED_UNARY:
                func = &Writer::writeAsTUcabac;
                binParams[0] = binarzationParams.getCMax();
                break;
            case paramcabac::BinarizationParameters::BinarizationId::EXPONENTIAL_GOLOMB:
            case paramcabac::BinarizationParameters::BinarizationId::SIGNED_EXPONENTIAL_GOMB:
                func = &Writer::writeAsEGcabac;
                break;
            case paramcabac::BinarizationParameters::BinarizationId::TRUNCATED_EXPONENTIAL_GOLOMB:
            case paramcabac::BinarizationParameters::BinarizationId::SIGNED_TRUNCATED_EXPONENTIAL_GOLOMB:
                func = &Writer::writeAsTEGcabac;
                binParams[0] = binarzationParams.getCMaxTeg();
                break;
            case paramcabac::BinarizationParameters::BinarizationId::SPLIT_UNITWISE_TRUNCATED_UNARY:
            case paramcabac::BinarizationParameters::BinarizationId::SIGNED_SPLIT_UNITWISE_TRUNCATED_UNARY:
                func = &Writer::writeAsSUTUcabac;
                binParams[0] = outputSymbolSize;
                binParams[1] = binarzationParams.getSplitUnitSize();
                break;
            case paramcabac::BinarizationParameters::BinarizationId::DOUBLE_TRUNCATED_UNARY:
            case paramcabac::BinarizationParameters::BinarizationId::SIGNED_DOUBLE_TRUNCATED_UNARY:
                func = &Writer::writeAsDTUcabac;
                binParams[0] = outputSymbolSize;
                binParams[1] = binarzationParams.getSplitUnitSize();
                binParams[2] = binarzationParams.getCMaxDtu();
                break;
            default:
                UTILS_DIE("Unknown Binarization");
        }
    }

    return func;
}

// ---------------------------------------------------------------------------------------------------------------------

size_t encodeTransformSubseqOrder0(const paramcabac::TransformedSubSeq &trnsfSubseqConf, util::DataBlock *symbols,
                                   size_t maxSize) {
    assert(symbols != nullptr);

    size_t numSymbols = symbols->size();
    if (numSymbols <= 0) return 0;

    const paramcabac::SupportValues &supportVals = trnsfSubseqConf.getSupportValues();
    const paramcabac::Binarization &binarzation = trnsfSubseqConf.getBinarization();
    const paramcabac::BinarizationParameters &binarzationParams = binarzation.getCabacBinarizationParameters();
    const paramcabac::StateVars &stateVars = trnsfSubseqConf.getStateVars();
    const paramcabac::BinarizationParameters::BinarizationId binID = binarzation.getBinarizationID();

    const uint8_t outputSymbolSize = supportVals.getOutputSymbolSize();
    const uint8_t codingSubsymSize = supportVals.getCodingSubsymSize();
    const uint64_t subsymMask = paramcabac::StateVars::get2PowN(codingSubsymSize) - 1;
    const bool bypassFlag = binarzation.getBypassFlag();

    util::DataBlock block(0, 1);
    OBufferStream bitstream(&block);
    Writer writer(&bitstream, bypassFlag, stateVars.getNumCtxTotal());
    writer.start();

    std::vector<unsigned int> binParams(4,
                                        0);  // first three elements are for binarization params, last one is for ctxIdx

    util::BlockStepper r = symbols->getReader();
    std::vector<Subsymbol> subsymbols(stateVars.getNumSubsymbols());

    ContextSelector ctxSelector(stateVars);
    const bool diffEnabled =
        (trnsfSubseqConf.getTransformIDSubsym() == paramcabac::SupportValues::TransformIdSubsym::DIFF_CODING);

    binFunc func = getBinarizor(outputSymbolSize, bypassFlag, binID, binarzationParams, stateVars, binParams);

    while (r.isValid()) {
        if (maxSize <= bitstream.size()) {
            break;
        }

        // Split symbol into subsymbols and then encode subsymbols
        const uint64_t origSymbol = r.get();
        const int64_t signedSymbolValue = paramcabac::StateVars::getSignedValue(origSymbol, symbols->getWordSize());
        const uint64_t symbolValue = abs(signedSymbolValue);
        uint64_t subsymValToCode = 0;

        uint32_t oss = outputSymbolSize;
        for (uint8_t s = 0; s < stateVars.getNumSubsymbols(); s++) {
            subsymValToCode = subsymbols[s].subsymValue = (symbolValue >> (oss -= codingSubsymSize)) & subsymMask;
            subsymbols[s].subsymIdx = s;

            if (diffEnabled) {
                assert(subsymbols[s].prvValues[0] <= subsymValToCode);
                subsymValToCode -= subsymbols[s].prvValues[0];
                subsymbols[s].prvValues[0] = subsymbols[s].subsymValue;
            }

            binParams[3] = ctxSelector.getContextIdxOrder0(s);

            (writer.*func)(subsymValToCode, binParams);
        }

        encodeSignflag(writer, binID, signedSymbolValue);

        r.inc();
    }

    writer.close();

    bitstream.flush(symbols);

    return symbols->size();  // size of bitstream
}

// ---------------------------------------------------------------------------------------------------------------------

size_t encodeTransformSubseqOrder1(const paramcabac::TransformedSubSeq &trnsfSubseqConf, util::DataBlock *symbols,
                                   util::DataBlock *const depSymbols, size_t maxSize) {
    assert(symbols != nullptr);

    size_t numSymbols = symbols->size();
    if (numSymbols <= 0) return 0;

    const paramcabac::SupportValues &supportVals = trnsfSubseqConf.getSupportValues();
    const paramcabac::Binarization &binarzation = trnsfSubseqConf.getBinarization();
    const paramcabac::BinarizationParameters &binarzationParams = binarzation.getCabacBinarizationParameters();
    const paramcabac::StateVars &stateVars = trnsfSubseqConf.getStateVars();
    const paramcabac::BinarizationParameters::BinarizationId binID = binarzation.getBinarizationID();
    const core::Alphabet alphaProps = getAlphabetProperties(trnsfSubseqConf.getAlphabetID());

    const uint8_t outputSymbolSize = supportVals.getOutputSymbolSize();
    const uint8_t codingSubsymSize = supportVals.getCodingSubsymSize();
    const uint8_t codingOrder = supportVals.getCodingOrder();
    const uint64_t subsymMask = paramcabac::StateVars::get2PowN(codingSubsymSize) - 1;
    const bool bypassFlag = binarzation.getBypassFlag();

    uint8_t const numLuts =
        stateVars.getNumLuts(codingOrder, supportVals.getShareSubsymLutFlag(), trnsfSubseqConf.getTransformIDSubsym());
    uint8_t const numPrvs = stateVars.getNumPrvs(supportVals.getShareSubsymPrvFlag());

    util::DataBlock block(0, 1);
    OBufferStream bitstream(&block);
    Writer writer(&bitstream, bypassFlag, stateVars.getNumCtxTotal());
    writer.start();

    std::vector<unsigned int> binParams(4,
                                        0);  // first three elements are for binarization params, last one is for ctxIdx

    util::BlockStepper r = symbols->getReader();
    std::vector<Subsymbol> subsymbols(stateVars.getNumSubsymbols());

    LUTsSubSymbolTransform lutsSubsymTrnsfm(supportVals, stateVars, numLuts, numPrvs, true);
    if (numLuts > 0) {
        lutsSubsymTrnsfm.encodeLUTs(writer, alphaProps, symbols, depSymbols);
    }

    util::BlockStepper rDep;
    if (depSymbols) {
        rDep = depSymbols->getReader();
    }

    ContextSelector ctxSelector(stateVars);

    binFunc func = getBinarizor(outputSymbolSize, bypassFlag, binID, binarzationParams, stateVars, binParams);

    while (r.isValid()) {
        if (maxSize <= bitstream.size()) {
            break;
        }

        // Split symbol into subsymbols and then encode subsymbols
        const uint64_t origSymbol = r.get();
        const int64_t signedSymbolValue = paramcabac::StateVars::getSignedValue(origSymbol, symbols->getWordSize());
        const uint64_t symbolValue = abs(signedSymbolValue);
        uint64_t subsymValToCode = 0;

        uint64_t depSymbolValue = 0, depSubsymValue = 0;
        if (rDep.isValid()) {
            depSymbolValue = alphaProps.inverseLut[rDep.get()];
            rDep.inc();
        }

        uint32_t oss = outputSymbolSize;
        for (uint8_t s = 0; s < stateVars.getNumSubsymbols(); s++) {
            const uint8_t lutIdx = (numLuts > 1) ? s : 0;  // either private or shared LUT
            const uint8_t prvIdx = (numPrvs > 1) ? s : 0;  // either private or shared PRV

            if (depSymbols) {
                depSubsymValue = (depSymbolValue >> (oss - codingSubsymSize)) & subsymMask;
                subsymbols[prvIdx].prvValues[0] = depSubsymValue;
            }

            subsymValToCode = subsymbols[s].subsymValue = (symbolValue >> (oss -= codingSubsymSize)) & subsymMask;
            subsymbols[s].subsymIdx = s;

            binParams[3] = ctxSelector.getContextIdxOrderGT0(s, prvIdx, subsymbols, codingOrder);

            if (numLuts > 0) {
                subsymbols[s].lutEntryIdx = 0;
                lutsSubsymTrnsfm.transformOrder1(subsymbols, s, lutIdx, prvIdx);
                subsymValToCode = subsymbols[s].lutEntryIdx;
                if (binID == paramcabac::BinarizationParameters::BinarizationId::TRUNCATED_UNARY) {
                    binParams[0] =
                        std::min((uint64_t)binarzationParams.getCMax(), subsymbols[s].lutNumMaxElems);  // update cMax
                }
            }

            (writer.*func)(subsymValToCode, binParams);

            subsymbols[prvIdx].prvValues[0] = subsymbols[s].subsymValue;
        }

        encodeSignflag(writer, binID, signedSymbolValue);

        r.inc();
    }

    writer.close();

    bitstream.flush(symbols);

    return symbols->size();  // size of bitstream
}

// ---------------------------------------------------------------------------------------------------------------------

size_t encodeTransformSubseqOrder2(const paramcabac::TransformedSubSeq &trnsfSubseqConf, util::DataBlock *symbols,
                                   size_t maxSize) {
    assert(symbols != nullptr);

    size_t numSymbols = symbols->size();
    if (numSymbols <= 0) return 0;

    const paramcabac::SupportValues &supportVals = trnsfSubseqConf.getSupportValues();
    const paramcabac::Binarization &binarzation = trnsfSubseqConf.getBinarization();
    const paramcabac::BinarizationParameters &binarzationParams = binarzation.getCabacBinarizationParameters();
    const paramcabac::StateVars &stateVars = trnsfSubseqConf.getStateVars();
    const paramcabac::BinarizationParameters::BinarizationId binID = binarzation.getBinarizationID();
    const core::Alphabet alphaProps = getAlphabetProperties(trnsfSubseqConf.getAlphabetID());

    const uint8_t outputSymbolSize = supportVals.getOutputSymbolSize();
    const uint8_t codingSubsymSize = supportVals.getCodingSubsymSize();
    const uint8_t codingOrder = supportVals.getCodingOrder();
    const uint64_t subsymMask = paramcabac::StateVars::get2PowN(codingSubsymSize) - 1;
    const bool bypassFlag = binarzation.getBypassFlag();

    uint8_t const numLuts =
        stateVars.getNumLuts(codingOrder, supportVals.getShareSubsymLutFlag(), trnsfSubseqConf.getTransformIDSubsym());
    uint8_t const numPrvs = stateVars.getNumPrvs(supportVals.getShareSubsymPrvFlag());

    util::DataBlock block(0, 1);
    OBufferStream bitstream(&block);
    Writer writer(&bitstream, bypassFlag, stateVars.getNumCtxTotal());
    writer.start();

    std::vector<unsigned int> binParams(4,
                                        0);  // first three elements are for binarization params, last one is for ctxIdx

    util::BlockStepper r = symbols->getReader();
    std::vector<Subsymbol> subsymbols(stateVars.getNumSubsymbols());

    LUTsSubSymbolTransform lutsSubsymTrnsfm(supportVals, stateVars, numLuts, numPrvs, true);
    if (numLuts > 0) {
        lutsSubsymTrnsfm.encodeLUTs(writer, alphaProps, symbols, nullptr);
    }

    ContextSelector ctxSelector(stateVars);

    binFunc func = getBinarizor(outputSymbolSize, bypassFlag, binID, binarzationParams, stateVars, binParams);

    while (r.isValid()) {
        if (maxSize <= bitstream.size()) {
            break;
        }

        // Split symbol into subsymbols and then encode subsymbols
        const uint64_t origSymbol = r.get();
        const int64_t signedSymbolValue = paramcabac::StateVars::getSignedValue(origSymbol, symbols->getWordSize());
        const uint64_t symbolValue = abs(signedSymbolValue);
        uint64_t subsymValToCode = 0;

        uint32_t oss = outputSymbolSize;
        for (uint8_t s = 0; s < stateVars.getNumSubsymbols(); s++) {
            const uint8_t lutIdx = (numLuts > 1) ? s : 0;  // either private or shared LUT
            const uint8_t prvIdx = (numPrvs > 1) ? s : 0;  // either private or shared PRV

            subsymValToCode = subsymbols[s].subsymValue = (symbolValue >> (oss -= codingSubsymSize)) & subsymMask;
            subsymbols[s].subsymIdx = s;

            binParams[3] = ctxSelector.getContextIdxOrderGT0(s, prvIdx, subsymbols, codingOrder);

            if (numLuts > 0) {
                subsymbols[s].lutEntryIdx = 0;
                lutsSubsymTrnsfm.transformOrder2(subsymbols, s, lutIdx, prvIdx);
                subsymValToCode = subsymbols[s].lutEntryIdx;
                if (binID == paramcabac::BinarizationParameters::BinarizationId::TRUNCATED_UNARY) {
                    binParams[0] =
                        std::min((uint64_t)binarzationParams.getCMax(), subsymbols[s].lutNumMaxElems);  // update cMax
                }
            }

            (writer.*func)(subsymValToCode, binParams);

            subsymbols[prvIdx].prvValues[1] = subsymbols[prvIdx].prvValues[0];
            subsymbols[prvIdx].prvValues[0] = subsymbols[s].subsymValue;
        }

        encodeSignflag(writer, binID, signedSymbolValue);

        r.inc();
    }

    writer.close();

    bitstream.flush(symbols);

    return symbols->size();  // size of bitstream
}

// ---------------------------------------------------------------------------------------------------------------------

size_t encodeTransformSubseq(const paramcabac::TransformedSubSeq &trnsfSubseqConf, util::DataBlock *symbols,
                             util::DataBlock *const depSymbols, size_t maxSize) {
    switch (trnsfSubseqConf.getSupportValues().getCodingOrder()) {
        case 0:
            return encodeTransformSubseqOrder0(trnsfSubseqConf, symbols, maxSize);
            break;
        case 1:
            return encodeTransformSubseqOrder1(trnsfSubseqConf, symbols, depSymbols, maxSize);
            break;
        case 2:
            return encodeTransformSubseqOrder2(trnsfSubseqConf, symbols, maxSize);
            break;
        default:
            UTILS_DIE("Unknown coding order");
    }

    return 0;
}

// ---------------------------------------------------------------------------------------------------------------------

}  // namespace gabac
}  // namespace entropy
}  // namespace genie

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------