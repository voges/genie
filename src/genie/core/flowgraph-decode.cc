/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/MueFab/genie for more details.
 */

#include "genie/core/flowgraph-decode.h"
#include <memory>
#include <string>
#include <utility>
#include <vector>

// ---------------------------------------------------------------------------------------------------------------------

namespace genie::core {

// ---------------------------------------------------------------------------------------------------------------------

void FlowGraphDecode::addReferenceSource(std::unique_ptr<ReferenceSource> dat) {
    refSources.push_back(std::move(dat));
}

// ---------------------------------------------------------------------------------------------------------------------

ReferenceManager& FlowGraphDecode::getRefMgr() { return *refMgr; }

// ---------------------------------------------------------------------------------------------------------------------

void FlowGraphDecode::setRefDecoder(RefDecoder* _refDecoder) { refDecoder = _refDecoder; }

// ---------------------------------------------------------------------------------------------------------------------

RefDecoder* FlowGraphDecode::getRefDecoder() { return refDecoder; }

// ---------------------------------------------------------------------------------------------------------------------

FlowGraphDecode::FlowGraphDecode(size_t threads) : mgr(threads) {
    readSelector.setDrain(&exporterSelector);
    refMgr = std::make_unique<ReferenceManager>(16);
}

// ---------------------------------------------------------------------------------------------------------------------

void FlowGraphDecode::addImporter(std::unique_ptr<FormatImporterCompressed> dat) {
    importers.emplace_back();
    setImporter(std::move(dat), importers.size() - 1);
}

// ---------------------------------------------------------------------------------------------------------------------

void FlowGraphDecode::setImporter(std::unique_ptr<FormatImporterCompressed> dat, size_t index) {
    importers[index] = std::move(dat);
    importers[index]->setDrain(&readSelector);
}

// ---------------------------------------------------------------------------------------------------------------------

void FlowGraphDecode::addReadCoder(std::unique_ptr<ReadDecoder> dat) {
    readCoders.emplace_back(std::move(dat));
    readSelector.addBranch(readCoders.back().get(), readCoders.back().get());
    readCoders.back()->setQVCoder(&qvSelector);
    readCoders.back()->setNameCoder(&nameSelector);
    readCoders.back()->setEntropyCoder(&entropySelector);
}

// ---------------------------------------------------------------------------------------------------------------------

void FlowGraphDecode::setReadCoder(std::unique_ptr<ReadDecoder> dat, size_t index) {
    readCoders[index] = std::move(dat);
    readSelector.setBranch(readCoders[index].get(), readCoders[index].get(), index);
    readCoders[index]->setQVCoder(&qvSelector);
    readCoders[index]->setNameCoder(&nameSelector);
    readCoders.back()->setEntropyCoder(&entropySelector);
}

// ---------------------------------------------------------------------------------------------------------------------

void FlowGraphDecode::addEntropyCoder(std::unique_ptr<EntropyDecoder> dat) {
    entropyCoders.emplace_back(std::move(dat));
    entropySelector.addMod(entropyCoders.back().get());
}

// ---------------------------------------------------------------------------------------------------------------------

void FlowGraphDecode::setEntropyCoder(std::unique_ptr<EntropyDecoder> dat, size_t index) {
    entropyCoders[index] = std::move(dat);
    entropySelector.setMod(entropyCoders[index].get(), index);
}

// ---------------------------------------------------------------------------------------------------------------------

void FlowGraphDecode::addExporter(std::unique_ptr<FormatExporter> dat) {
    exporters.emplace_back(std::move(dat));
    exporterSelector.add(exporters.back().get());
}

// ---------------------------------------------------------------------------------------------------------------------

void FlowGraphDecode::setExporter(std::unique_ptr<FormatExporter> dat, size_t index) {
    exporters[index] = std::move(dat);
    exporterSelector.set(exporters[index].get(), index);
}

// ---------------------------------------------------------------------------------------------------------------------

void FlowGraphDecode::setReadCoderSelector(const std::function<size_t(const AccessUnit&)>& fun) {
    readSelector.setOperation(fun);
}

// ---------------------------------------------------------------------------------------------------------------------

void FlowGraphDecode::setQVSelector(
    std::function<size_t(const parameter::QualityValues& param, const std::vector<std::string>& ecigar,
                         const std::vector<uint64_t>& positions, AccessUnit::Descriptor& desc)>
        fun) {
    qvSelector.setSelection(std::move(fun));

    for (auto& r : readCoders) {
        r->setQVCoder(&qvSelector);
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void FlowGraphDecode::setNameSelector(std::function<size_t(AccessUnit::Descriptor& desc)> fun) {
    nameSelector.setSelection(std::move(fun));

    for (auto& r : readCoders) {
        r->setNameCoder(&nameSelector);
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void FlowGraphDecode::setEntropyCoderSelector(
    const std::function<size_t(const parameter::DescriptorSubseqCfg&, AccessUnit::Descriptor&, bool)>&
        fun) {
    entropySelector.setSelection(fun);
}

// ---------------------------------------------------------------------------------------------------------------------

void FlowGraphDecode::setExporterSelector(const std::function<size_t(const record::Chunk&)>& fun) {
    exporterSelector.setOperation(fun);
}

// ---------------------------------------------------------------------------------------------------------------------

void FlowGraphDecode::addNameCoder(std::unique_ptr<NameDecoder> dat) {
    nameCoders.emplace_back(std::move(dat));
    nameSelector.addMod(nameCoders.back().get());
}

// ---------------------------------------------------------------------------------------------------------------------

void FlowGraphDecode::setNameCoder(std::unique_ptr<NameDecoder> dat, size_t index) {
    nameCoders[index] = std::move(dat);
    nameSelector.setMod(nameCoders[index].get(), index);
}

// ---------------------------------------------------------------------------------------------------------------------

void FlowGraphDecode::addQVCoder(std::unique_ptr<QVDecoder> dat) {
    qvCoders.emplace_back(std::move(dat));
    qvSelector.addMod(qvCoders.back().get());
}

// ---------------------------------------------------------------------------------------------------------------------

void FlowGraphDecode::setQVCoder(std::unique_ptr<QVDecoder> dat, size_t index) {
    qvCoders[index] = std::move(dat);
    qvSelector.setMod(qvCoders[index].get(), index);
}

// ---------------------------------------------------------------------------------------------------------------------

void FlowGraphDecode::run() {
    std::vector<util::OriginalSource*> imps;
    imps.reserve(importers.size());
    for (auto& i : importers) {
        imps.emplace_back(i.get());
    }
    mgr.setSource(std::move(imps));
    mgr.run();
}

// ---------------------------------------------------------------------------------------------------------------------

stats::PerfStats FlowGraphDecode::getStats() {
    stats::PerfStats ret;
    for (auto& e : exporters) {
        ret.add(e->getStats());
    }
    return ret;
}

// ---------------------------------------------------------------------------------------------------------------------

}  // namespace genie::core

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
