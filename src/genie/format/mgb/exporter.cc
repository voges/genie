/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/mitogen/genie for more details.
 */

#include "exporter.h"
#include <genie/util/watch.h>

// ---------------------------------------------------------------------------------------------------------------------

namespace genie {
namespace format {
namespace mgb {

// ---------------------------------------------------------------------------------------------------------------------

Exporter::Exporter(std::ostream* _file) : writer(_file) {}

// ---------------------------------------------------------------------------------------------------------------------

void Exporter::flowIn(core::AccessUnitPayload&& t, const util::Section& id) {
    util::Watch watch;
    core::AccessUnitPayload data = std::move(t);
    util::OrderedSection section(&lock, id);
    getStats().add(data.getStats());
    data.getParameters().write(writer);
    auto type = core::record::ClassType::CLASS_U;
    if (data.getParameters().isComputedReference() && data.getParameters().getComputedRef().getAlgorithm() ==
                                                          core::parameter::ComputedRef::Algorithm::LOCAL_ASSEMBLY) {
        type = core::record::ClassType::CLASS_I;
    }
    mgb::AccessUnit au(id.start, id.start, type, data.getRecordNum() * data.getParameters().getNumberTemplateSegments(),
                       core::parameter::DataUnit::DatasetType::ALIGNED, 32, 32, 0);
    /*au.setAuTypeCfg(
        AuTypeCfg(data.getReference(), data.getMinPos(), data.getMaxPos(), data.getParameters().getPosSize()));*/
    for (size_t descriptor = 0; descriptor < core::getDescriptors().size(); ++descriptor) {
        if (data.getPayload(core::GenDesc(descriptor)).isEmpty()) {
            continue;
        }
        au.addBlock(Block(descriptor, data.movePayload(descriptor)));
    }
    au.write(writer);
    getStats().addDouble("time-mgb-export", watch.check());
}

// ---------------------------------------------------------------------------------------------------------------------

}  // namespace mgb
}  // namespace format
}  // namespace genie

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
