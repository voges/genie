/**
 * @file
 * @copyright This file is part of Genie. See LICENSE for more details.
 */

#ifndef GENIE_FILEHANDLINGUTILS_H
#define GENIE_FILEHANDLINGUTILS_H

#include <memory.h>
#include <cerrno>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <vector>

#include <genie/util/bitreader.h>
#include <fstream>

int createMPEGGFileNoMITFromByteStream(const std::string& fileName, const std::string& outputFileName);

#endif  // GENIE_FILEHANDLINGUTILS_H
