/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/mitogen/genie for more details.
 */

#ifndef GENIE_SPRING_ENCODER_H
#define GENIE_SPRING_ENCODER_H

#include <genie/core/read-encoder.h>
#include <genie/core/stats/perf-stats.h>
#include "preprocess.h"

namespace genie {
namespace read {
namespace spring {

class SpringEncoder : public genie::core::ReadEncoder {
   private:
    Preprocessor preprocessor;
    core::stats::FastqStats* stats;

   public:
    SpringEncoder(const std::string&, core::stats::FastqStats* _stats) : stats(_stats) {
        //  preprocessor.setup(ncoder, coder, working_dir);
    }
    void flowIn(genie::core::record::Chunk&& t, const util::Section& id) override;

    void flushIn() override;
};
}  // namespace spring
}  // namespace read
}  // namespace genie
#endif  // GENIE_SPRING_ENCODER_H
