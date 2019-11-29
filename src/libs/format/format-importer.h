#ifndef GENIE_FORMAT_IMPORTER_H
#define GENIE_FORMAT_IMPORTER_H

#include "format/part2/mpegg_rec/mpegg-record.h"
#include <vector>

class FormatImporter {
public:
    ~FormatImporter() = default;
    virtual void importBlock(std::vector<MpeggRecord>* vec) = 0;
};

#endif //GENIE_FORMAT_IMPORTER_H
