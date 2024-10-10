/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/MueFab/genie for more details.
 */

#ifndef SRC_APPS_GENIE_CAPSULATOR_PROGRAM_OPTIONS_H_
#define SRC_APPS_GENIE_CAPSULATOR_PROGRAM_OPTIONS_H_

// ---------------------------------------------------------------------------------------------------------------------

#include <string>

// ---------------------------------------------------------------------------------------------------------------------

namespace genieapp::capsulator {

/**
 * @brief
 */
class ProgramOptions {
 public:
    /**
     * @brief
     * @param argc
     * @param argv
     */
    ProgramOptions(int argc, char *argv[]);

 public:
    std::string inputFile;  //!< @brief

    std::string outputFile;  //!< @brief

    bool forceOverwrite;  //!< @brief

    bool help;  //!< @brief

 private:
    /**
     * @brief
     */
    void validate() const;
};

// ---------------------------------------------------------------------------------------------------------------------

}  // namespace genieapp::capsulator

// ---------------------------------------------------------------------------------------------------------------------

#endif  // SRC_APPS_GENIE_CAPSULATOR_PROGRAM_OPTIONS_H_

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
