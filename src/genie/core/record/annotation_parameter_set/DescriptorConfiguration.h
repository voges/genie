/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/mitogen/genie for more details.
 */

#ifndef SRC_GENIE_CORE_RECORD_ANNOTATION_PARAMETER_SET_DESCRIPTORCONFIGURATION_H_
#define SRC_GENIE_CORE_RECORD_ANNOTATION_PARAMETER_SET_DESCRIPTORCONFIGURATION_H_

// ---------------------------------------------------------------------------------------------------------------------

#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "genie/core/constants.h"
#include "genie/util/bitreader.h"
#include "genie/util/bitwriter.h"

#include "AlgorithmParameters.h"
#include "ContactMatrixParameters.h"
#include "genie/contact/contact_matrix_parameters.h"
#include "genie/core/writer.h"
#include "genie/genotype/genotype_parameters.h"
#include "genie/likelihood/likelihood_parameters.h"

// ---------------------------------------------------------------------------------------------------------------------

namespace genie {
namespace core {
namespace record {
namespace annotation_parameter_set {

class DescriptorConfiguration {
 private:
    AnnotDesc descriptor_ID;
    AlgoID encoding_mode_ID;
    genie::genotype::GenotypeParameters genotype_parameters;
    genie::likelihood::LikelihoodParameters likelihood_parameters;
    genie::contact::ContactMatrixParameters contact_matrix_parameters;
    AlgorithmParameters algorithm_parameters;

 public:
    DescriptorConfiguration();
    explicit DescriptorConfiguration(util::BitReader& reader);

    DescriptorConfiguration(AnnotDesc descriptor_ID, AlgoID encoding_mode_ID,
        genie::genotype::GenotypeParameters genotype_parameters,
        AlgorithmParameters algorithm_parameters);

    DescriptorConfiguration(AnnotDesc descriptor_ID, AlgoID encoding_mode_ID,
        genie::likelihood::LikelihoodParameters likelihood_parameters,
        AlgorithmParameters algorithm_parameters);

    DescriptorConfiguration(AnnotDesc descriptor_ID, AlgoID encoding_mode_ID,
        genie::contact::ContactMatrixParameters _contact_matrix_parameters,
        AlgorithmParameters algorithm_parameters);
 
    DescriptorConfiguration(AnnotDesc descriptor_ID, AlgoID encoding_mode_ID,
        AlgorithmParameters algorithm_parameters);

    void read(util::BitReader& reader);
    void write(core::Writer& writer) const;
    size_t getSize(core::Writer& write_size) const;
    AnnotDesc getDescriptorID() const { return descriptor_ID; }
    AlgoID getEncodingModeID() const { return encoding_mode_ID; }

    genie::genotype::GenotypeParameters getGenotypeParameters() const { return genotype_parameters; }
    genie::likelihood::LikelihoodParameters getLikelihoodParameters() const { return likelihood_parameters; }
    genie::contact::ContactMatrixParameters getContactMatrixParameters() const { return contact_matrix_parameters; }
    AlgorithmParameters getAlgorithmParameters() const { return algorithm_parameters; }
};

// ---------------------------------------------------------------------------------------------------------------------

}  // namespace annotation_parameter_set
}  // namespace record
}  // namespace core
}  // namespace genie

// ---------------------------------------------------------------------------------------------------------------------

#endif  // SRC_GENIE_CORE_RECORD_ANNOTATION_PARAMETER_SET_DESCRIPTORCONFIGURATION_H_

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
