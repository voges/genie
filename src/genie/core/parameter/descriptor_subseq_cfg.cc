/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/MueFab/genie for more details.
 */

#include "genie/core/parameter/descriptor_subseq_cfg.h"
#include <memory>
#include <utility>
#include "genie/core/parameter/descriptor_present/descriptor_present.h"

#include "genie/util/runtime-exception.h"

// ---------------------------------------------------------------------------------------------------------------------

namespace genie::core::parameter {

// ---------------------------------------------------------------------------------------------------------------------

DescriptorSubseqCfg::DescriptorSubseqCfg() : class_specific_dec_cfg_flag(false), descriptor_configurations(0) {
    descriptor_configurations.push_back(std::make_unique<desc_pres::DescriptorPresent>());
}

// ---------------------------------------------------------------------------------------------------------------------

DescriptorSubseqCfg::DescriptorSubseqCfg(const DescriptorSubseqCfg& cfg) : class_specific_dec_cfg_flag(false) {
    *this = cfg;
}

// ---------------------------------------------------------------------------------------------------------------------

DescriptorSubseqCfg::DescriptorSubseqCfg(DescriptorSubseqCfg&& cfg) noexcept : class_specific_dec_cfg_flag(false) {
    *this = std::move(cfg);
}

// ---------------------------------------------------------------------------------------------------------------------

DescriptorSubseqCfg& DescriptorSubseqCfg::operator=(const DescriptorSubseqCfg& cfg) {
    if (this == &cfg) {
        return *this;
    }
    class_specific_dec_cfg_flag = cfg.class_specific_dec_cfg_flag;
    descriptor_configurations.clear();
    for (const auto& b : cfg.descriptor_configurations) {
        descriptor_configurations.emplace_back(b->clone());
    }
    return *this;
}

// ---------------------------------------------------------------------------------------------------------------------

DescriptorSubseqCfg& DescriptorSubseqCfg::operator=(DescriptorSubseqCfg&& cfg) noexcept {
    class_specific_dec_cfg_flag = cfg.class_specific_dec_cfg_flag;
    descriptor_configurations = std::move(cfg.descriptor_configurations);
    return *this;
}

// ---------------------------------------------------------------------------------------------------------------------

DescriptorSubseqCfg::DescriptorSubseqCfg(const size_t num_classes, const GenDesc desc, util::BitReader& reader) {
    class_specific_dec_cfg_flag = reader.read<bool>(1);
    if (class_specific_dec_cfg_flag == 0) {
        descriptor_configurations.emplace_back(Descriptor::factory(desc, reader));
    } else {
        for (size_t i = 0; i < num_classes; ++i) {
            descriptor_configurations.emplace_back(Descriptor::factory(desc, reader));
        }
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void DescriptorSubseqCfg::setClassSpecific(const uint8_t index, std::unique_ptr<Descriptor> conf) {
    if (index > descriptor_configurations.size()) {
        UTILS_THROW_RUNTIME_EXCEPTION("Config index out of bounds.");
    }
    descriptor_configurations[index] = std::move(conf);
}

// ---------------------------------------------------------------------------------------------------------------------

void DescriptorSubseqCfg::set(std::unique_ptr<Descriptor> conf) {
    if (class_specific_dec_cfg_flag) {
        UTILS_THROW_RUNTIME_EXCEPTION("Invalid setConfig() for non class-specific descriptor config.");
    }
    descriptor_configurations[0] = std::move(conf);
}

// ---------------------------------------------------------------------------------------------------------------------

const Descriptor& DescriptorSubseqCfg::get() const { return *descriptor_configurations.front(); }

// ---------------------------------------------------------------------------------------------------------------------

const Descriptor& DescriptorSubseqCfg::getClassSpecific(const uint8_t index) const {
    return *descriptor_configurations[index];
}

// ---------------------------------------------------------------------------------------------------------------------

void DescriptorSubseqCfg::enableClassSpecific(const uint8_t numClasses) {
    if (class_specific_dec_cfg_flag) {
        return;
    }
    class_specific_dec_cfg_flag = true;
    descriptor_configurations.resize(numClasses);
    for (size_t i = 1; i < descriptor_configurations.size(); ++i) {
        descriptor_configurations[i] = descriptor_configurations[0]->clone();
    }
}

// ---------------------------------------------------------------------------------------------------------------------

bool DescriptorSubseqCfg::isClassSpecific() const { return class_specific_dec_cfg_flag; }

// ---------------------------------------------------------------------------------------------------------------------

void DescriptorSubseqCfg::write(util::BitWriter& writer) const {
    writer.writeBits(class_specific_dec_cfg_flag, 1);
    for (auto& i : descriptor_configurations) {
        i->write(writer);
    }
}

// ---------------------------------------------------------------------------------------------------------------------

bool DescriptorSubseqCfg::operator==(const DescriptorSubseqCfg& cfg) const {
    return class_specific_dec_cfg_flag == cfg.class_specific_dec_cfg_flag && desc_comp(cfg);
}

// ---------------------------------------------------------------------------------------------------------------------

bool DescriptorSubseqCfg::desc_comp(const DescriptorSubseqCfg& cfg) const {
    if (cfg.descriptor_configurations.size() != descriptor_configurations.size()) {
        return false;
    }
    for (size_t i = 0; i < cfg.descriptor_configurations.size(); ++i) {
        if (!descriptor_configurations[i]->equals(cfg.descriptor_configurations[i].get())) {
            return false;
        }
    }
    return true;
}

// ---------------------------------------------------------------------------------------------------------------------

}  // namespace genie::core::parameter

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
