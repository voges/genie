/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/MueFab/genie for more details.
 */

#ifndef SRC_APPS_GENIE_TRANSCODE_SAM_SAM_SAM_TO_MGREC_PROGRAM_OPTIONS_H_
#define SRC_APPS_GENIE_TRANSCODE_SAM_SAM_SAM_TO_MGREC_PROGRAM_OPTIONS_H_

// ---------------------------------------------------------------------------------------------------------------------

#include <cstdint>
#include <string>

// ---------------------------------------------------------------------------------------------------------------------

namespace genieapp::transcode_sam::sam::sam_to_mgrec {

/**
 * @brief
 */
class Config {
 public:
    /**
     * @brief
     */
    enum format : std::uint8_t {
        sam = 0,
    };

    /**
     * @brief
     * @param argc
     * @param argv
     */
    Config(int argc, char *argv[]);

    /**
     * @brief
     */
    ~Config();

 public:
    int verbosity_level;          //!< @brief
    std::string tmp_dir_path;     //!< @brief
    std::string fasta_file_path;  //!< @brief
    std::string inputFile;        //!< @brief
    std::string outputFile;       //!< @brief
    bool forceOverwrite;          //!< @brief
    bool help;                    //!< @brief
    bool no_ref;                  //!< @brief
    bool clean;                   //!< @brief
    uint32_t num_threads;         //!< @brief

 private:
    /**
     * @brief
     * @param argc
     * @param argv
     */
    void processCommandLine(int argc, char *argv[]);

    /**
     * @brief
     */
    void validate();
};

// ---------------------------------------------------------------------------------------------------------------------

}  // namespace genieapp::transcode_sam::sam::sam_to_mgrec

// ---------------------------------------------------------------------------------------------------------------------

#endif  // SRC_APPS_GENIE_TRANSCODE_SAM_SAM_SAM_TO_MGREC_PROGRAM_OPTIONS_H_

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
