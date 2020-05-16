#include <cassert>
#include <csignal>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <genie/entropy/gabac/gabac.h>

#include "analyze.h"
#include "code.h"
#include "program-options.h"

int main(int argc, char* argv[]) {
    try {
        gabacify::ProgramOptions programOptions(argc, argv);

        if (programOptions.task == "encode") {
            gabacify::code(programOptions.inputFilePath,
                           // RESTRUCT_DISABLE programOptions.configurationFilePath,
                           programOptions.outputFilePath, programOptions.blocksize, programOptions.descID,
                           programOptions.subseqID, false, programOptions.dependencyFilePath);
        } else if (programOptions.task == "decode") {
            gabacify::code(programOptions.inputFilePath,
                           // RESTRUCT_DISABLE programOptions.configurationFilePath,
                           programOptions.outputFilePath, programOptions.blocksize, programOptions.descID,
                           programOptions.subseqID, true, programOptions.dependencyFilePath);
        } /* RESTRUCT-DISABLE
          else if (programOptions.task == "analyze") {

            gabacify::analyze(programOptions.inputFilePath, programOptions.outputFilePath, programOptions.blocksize,
                              programOptions.maxVal, programOptions.wordSize);

        } */
        else {
            GABAC_DIE("Invalid task: " + std::string(programOptions.task));
        }
    } catch (const genie::entropy::gabac::RuntimeException& e) {
        std::cerr << e.message() << std::endl;
        return EXIT_FAILURE;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "Unkown error occurred" << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
