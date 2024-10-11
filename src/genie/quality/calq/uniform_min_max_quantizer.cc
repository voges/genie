/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/MueFab/genie for more details.
 */

#include "genie/quality/calq/uniform_min_max_quantizer.h"

// -----------------------------------------------------------------------------

namespace genie::quality::calq {

// -----------------------------------------------------------------------------

UniformMinMaxQuantizer::UniformMinMaxQuantizer(const int& valueMin, const int& valueMax, const int& nrSteps)
    : UniformQuantizer(valueMin, valueMax, nrSteps) {
    // Change the smallest and largest reconstruction values

    const int smallestIndex = 0;
    const int largestIndex = nrSteps - 1;

    for (auto& lutElem : lut_) {
        const int currentIndex = lutElem.second.first;
        if (currentIndex == smallestIndex) {
            lutElem.second.second = valueMin;
        }
        if (currentIndex == largestIndex) {
            lutElem.second.second = valueMax;
        }
    }

    for (auto& inverseLutElem : inverseLut_) {
        const int currentIndex = inverseLutElem.first;
        if (currentIndex == smallestIndex) {
            inverseLutElem.second = valueMin;
        }
        if (currentIndex == largestIndex) {
            inverseLutElem.second = valueMax;
        }
    }
}

// -----------------------------------------------------------------------------

UniformMinMaxQuantizer::~UniformMinMaxQuantizer() = default;

// -----------------------------------------------------------------------------

}  // namespace genie::quality::calq

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
