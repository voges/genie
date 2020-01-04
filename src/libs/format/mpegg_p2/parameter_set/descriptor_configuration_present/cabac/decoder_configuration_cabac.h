#ifndef GENIE_DECODER_CONFIGURATION_CABAC_H
#define GENIE_DECODER_CONFIGURATION_CABAC_H

// -----------------------------------------------------------------------------------------------------------------

#include "../../../clutter.h"
#include "../decoder_configuration.h"
#include "util/bitwriter.h"

// -----------------------------------------------------------------------------------------------------------------

namespace format {
namespace mpegg_p2 {
namespace desc_conf_pres {
namespace cabac {
/**
 * Base for ISO 23092-2 Section 8.3.1 and ISO 23092-2 Section 8.3.5
 */
class DecoderConfigurationCabac : public DecoderConfiguration {
   protected:
    GenDesc desc;
    uint8_t rle_guard_tokentype : 8;                                                     //!< line 4
    std::vector<std::unique_ptr<DescriptorSubsequenceCfg>> descriptor_subsequence_cfgs;  //!< Line 4 to 13

   public:
    explicit DecoderConfigurationCabac(GenDesc _desc);

    explicit DecoderConfigurationCabac(GenDesc _desc, util::BitReader &reader) : DecoderConfiguration(DecoderConfiguration::EncodingModeId::CABAC){
        uint8_t num_descriptor_subsequence_cfgs = reader.read(8) + 1;
        desc = _desc;
        for(size_t i = 0; i < num_descriptor_subsequence_cfgs; ++i) {
            descriptor_subsequence_cfgs.emplace_back(util::make_unique<DescriptorSubsequenceCfg>(reader));
        }
    }

    void setSubsequenceCfg(uint8_t index, std::unique_ptr<TransformSubseqParameters> cfg);

    DescriptorSubsequenceCfg *getSubsequenceCfg(uint8_t index) const {
        return descriptor_subsequence_cfgs[uint8_t(index)].get();
    }

    DecoderConfigurationCabac(const DecoderConfigurationCabac &c)
        : DecoderConfiguration(EncodingModeId::CABAC), desc(c.desc), rle_guard_tokentype(c.rle_guard_tokentype) {
        if(this == &c) {
           return;
        }
        for(const auto& a : c.descriptor_subsequence_cfgs)  {
            descriptor_subsequence_cfgs.emplace_back(a->clone());
        }
    }

    std::unique_ptr<DecoderConfiguration> clone() const override;

    void write(util::BitWriter &writer) const override;

    uint8_t getRleGuardTokentype() const { return rle_guard_tokentype; }
};
}  // namespace cabac
}  // namespace desc_conf_pres
}  // namespace mpegg_p2
}  // namespace format

// -----------------------------------------------------------------------------------------------------------------

#endif  // GENIE_DECODER_CONFIGURATION_CABAC_H
