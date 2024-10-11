/**
 * @file
 * @copyright This file is part of GENIE.
 * See LICENSE and/or visit https://github.com/MueFab/genie for more details.
 * @brief Header file for the UniformQuantizer class.
 *
 * This file defines the `UniformQuantizer` class, which is used in the CALQ quality value compression
 * framework to perform uniform quantization of quality values. Uniform quantization is a technique
 * where the range of quality values is divided into equal-sized steps, making it easier to reduce
 * the representation size of quality values in bioinformatics data.
 *
 * @details The `UniformQuantizer` inherits from the base `Quantizer` class and implements a uniform
 * quantization scheme. This is a simpler quantization method compared to others like Lloyd-Max, where
 * the quantization boundaries are evenly spaced. It is particularly useful when the distribution of
 * values is roughly uniform and doesn't have distinct clusters or peaks.
 */

#ifndef SRC_GENIE_QUALITY_CALQ_UNIFORM_QUANTIZER_H_
#define SRC_GENIE_QUALITY_CALQ_UNIFORM_QUANTIZER_H_

#include "genie/quality/calq/quantizer.h"

namespace genie::quality::calq {

/**
 * @brief The `UniformQuantizer` class provides a simple uniform quantization scheme.
 *
 * The `UniformQuantizer` divides the range of quality values into evenly spaced steps. Each step is
 * assigned a representative value, which is used as the quantized value for all input values falling
 * within that step. This quantization method is particularly effective for datasets where quality values
 * are evenly distributed, or when a simple compression method is required.
 */
class UniformQuantizer : public Quantizer {
 public:
    /**
     * @brief Constructs a `UniformQuantizer` with a specified value range and number of steps.
     *
     * @param valueMin The minimum value in the range to be quantized.
     * @param valueMax The maximum value in the range to be quantized.
     * @param nrSteps The number of quantization steps (intervals) within the specified range.
     *
     * This constructor initializes the quantizer with evenly spaced intervals between `valueMin` and
     * `valueMax`. Each interval is assigned a unique quantized value, which is used during the encoding
     * and decoding processes.
     */
    UniformQuantizer(const int& valueMin, const int& valueMax, const int& nrSteps);

    /**
     * @brief Destructor for the `UniformQuantizer` class.
     *
     * This function cleans up any allocated resources and performs any necessary finalization.
     */
    ~UniformQuantizer() override;
};

}  // namespace genie::quality::calq

#endif  // SRC_GENIE_QUALITY_CALQ_UNIFORM_QUANTIZER_H_

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
