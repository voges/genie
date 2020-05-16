/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/mitogen/genie for more details.
 */

#ifndef GENIE_STATE_VARS_H
#define GENIE_STATE_VARS_H

// ---------------------------------------------------------------------------------------------------------------------

#include "binarization.h"
#include "support_values.h"

#include <genie/core/constants.h>

// ---------------------------------------------------------------------------------------------------------------------

namespace genie {
namespace entropy {
namespace paramcabac {

/**
 * ISO 23092-2 Section 8.3.2 table
 */
class StateVars {
   private:
    uint64_t numAlphaSubsym;
    uint32_t numSubsyms;
    uint32_t numCtxSubsym;
    uint32_t cLengthBI;
    uint64_t codingOrderCtxOffset[3];
    uint64_t codingSizeCtxOffset;
    uint32_t numCtxLuts;
    uint64_t numCtxTotal;

   public:
    static const uint32_t MAX_LUT_SIZE = (1u << 8u);

    StateVars();

    void populate(const SupportValues::TransformIdSubsym transform_ID_subsym, const SupportValues support_values,
                  const Binarization cabac_binarization, const core::GenSubIndex subsequence_ID,
                  const core::AlphabetID alphabet_ID);

    virtual ~StateVars() = default;

    uint32_t getNumSubsymbols() const { return numSubsyms; }

    uint64_t getNumAlphaSubsymbol() const { return numAlphaSubsym; }

    uint32_t getNumCtxPerSubsymbol() const { return numCtxSubsym; }

    uint32_t getCLengthBI() const { return cLengthBI; }

    uint64_t getCodingOrderCtxOffset(uint8_t index) const { return codingOrderCtxOffset[index]; }

    uint64_t getCodingSizeCtxOffset() const { return codingSizeCtxOffset; }

    uint32_t getNumCtxLUTs() const { return numCtxLuts; }

    uint64_t getNumCtxTotal() const { return numCtxTotal; }

    uint8_t getNumLuts(const uint8_t codingOrder, const bool shareSubsymLutFlag,
                       const SupportValues::TransformIdSubsym trnsfSubsymID) const;

    uint8_t getNumPrvs(const bool shareSubsymPrvFlag) const;

    /* some extra functions (for now put in statevars)
     * FIXME move else where
     */
    static uint64_t getNumAlphaSpecial(const core::GenSubIndex subsequence_ID, const core::AlphabetID alphabet_ID);

    static uint8_t getMinimalSizeInBytes(uint8_t sizeInBits) { return (sizeInBits / 8) + ((sizeInBits % 8) ? 1 : 0); }

    static uint8_t getLgWordSize(uint8_t sizeInBits) { return StateVars::getMinimalSizeInBytes(sizeInBits) >> 3; }

    static int64_t getSignedValue(uint64_t input, uint8_t sizeInBytes) {
        int64_t signedValue = 0;

        switch (sizeInBytes) {
            case 1:
                signedValue = static_cast<int64_t>(static_cast<int8_t>(input));
                break;
            case 2:
                signedValue = static_cast<int64_t>(static_cast<int16_t>(input));
                break;
            case 4:
                signedValue = static_cast<int64_t>(static_cast<int32_t>(input));
                break;
            case 8:
                signedValue = static_cast<int64_t>(input);
                break;
            default:
                UTILS_THROW_RUNTIME_EXCEPTION("Invalid size");
        }

        return signedValue;
    }

    static uint64_t get2PowN(uint8_t N) {
        assert(N <= 32);
        uint64_t one = 1u;
        return one << N;
    }
};

// ---------------------------------------------------------------------------------------------------------------------

}  // namespace paramcabac
}  // namespace entropy
}  // namespace genie

// ---------------------------------------------------------------------------------------------------------------------

#endif  // GENIE_STATE_VARS_H

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
