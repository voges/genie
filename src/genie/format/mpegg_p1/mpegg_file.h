#ifndef GENIE_MPEGG_FILE_H
#define GENIE_MPEGG_FILE_H

#include <vector>
#include "genie/format/mpegg_p1/dataset_group.h"
#include "genie/format/mpegg_p1/file_header.h"

namespace genie {
namespace format {
namespace mpegg_p1 {

class MpeggFile {
   public:
    MpeggFile(std::vector<DatasetGroup>*);

    const FileHeader& getFileHeader() const { return fileHeader; }
    const std::vector<DatasetGroup>& getDatasetGroups() const { return datasetGroups; }

    void writeToFile(const std::string& outputFileName);

   private:
    FileHeader fileHeader;
    std::vector<DatasetGroup> datasetGroups;
};

}  // namespace mpegg_p1
}  // namespace format
}  // namespace genie
#endif  // GENIE_MPEGG_FILE_H
