/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/mitogen/genie for more details.
 */

#include "genie/util/string-helpers.h"

namespace util_tests {

std::string exec(const std::string &cmd) {
#ifdef WIN32
    FILE *pipe = _popen(cmd.c_str(), "r");
#else
    FILE *pipe = popen(cmd.c_str(), "r");
#endif
    if (!pipe) {
        return "<exec(" + cmd + ") failed>";
    }

    const int bufferSize = 256;
    char buffer[bufferSize];
    std::string result;

    while (!feof(pipe)) {
        if (fgets(buffer, bufferSize, pipe) != nullptr) {
            result += buffer;
        }
    }
#ifdef WIN32
    _pclose(pipe);
#else
    pclose(pipe);
#endif
    genie::util::rtrim(result);

    return result;
}

}  // namespace util_tests