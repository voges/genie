#ifndef GENIE_FILE_HEADER_H
#define GENIE_FILE_HEADER_H

#include <string>
#include <vector>
#include "genie/util/bitwriter.h"

namespace genie {
namespace format {
namespace mpegg_p1 {

class FileHeader {
   public:
    explicit FileHeader() = default;
    explicit FileHeader(std::vector<std::string>*);  // FIXME:strings needs to be checked for size!

    void addCompatibleBrand(const std::string& brand) {
        compatible_brand.push_back(brand);
    }  // FIXME:string needs to be checked for size!

    const char* getMajorBrand() const { return major_brand; }
    const char* getMinorBrand() const { return minor_brand; }
    const std::vector<std::string>& getCompatibleBrand() const { return compatible_brand; }

    void writeToFile(genie::util::BitWriter& bitWriter) const;

   private:
    /**
     * ISO 23092-1 Section 6.6.2 table 30
     *
     * ------------------------------------------------------------------------------------------------------------- */
    const char* const major_brand = "MPEG-G";
    const char* const minor_brand = "2000";  // FIXME: 19 or 20?
    std::vector<std::string> compatible_brand;
};

}  // namespace mpegg_p1
}  // namespace format
}  // namespace genie

#endif  // GENIE_FILE_HEADER_H
