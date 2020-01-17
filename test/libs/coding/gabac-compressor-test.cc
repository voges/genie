#include <genie/entropy/gabac/gabac-compressor.h>
#include <genie/util/drain.h>
#include <gtest/gtest.h>
#include <iostream>
#include "helpers.h"

TEST(GabacCompressor, NonEmpty) {  // NOLINT(cert-err58-cpp)

    class DummyModule : public genie::util::Drain<genie::core::AccessUnitPayload> {
       public:
        void flowIn(genie::core::AccessUnitPayload&& t, size_t id) override {
            auto data = std::move(t);  // Needed to "consume" the data and move it into local scope
            /* This is where we catch the result of GabacCompressor. Collect any information about results here */

            std::cout << "GabacCompressor finished encoding access unit " << id << std::endl;

            // Example how to iterate BlockPayloadSet. Gabac compressor generates at least one transformed payload for
            // every Subsequence that contained data.
            for (auto& payload : data.getPayloads()) {
                if (payload.isEmpty()) {
                    continue;
                }
                std::cout << "Descriptor " << getDescriptor(payload.getID()).name << ":" << std::endl;
                for (auto& subseqpayload : payload.getSubsequencePayloads()) {
                    if (subseqpayload.isEmpty()) {
                        continue;
                    }
                    std::cout << "\tSubSequence " << getSubsequence(subseqpayload.getID()).name << ":" << std::endl;
                    for (auto& transformedPayload : subseqpayload.getTransformedPayloads()) {
                        auto dataBlock = transformedPayload.move();  // Extract the data
                        std::cout << "\t\tTransformedSequence " << dataBlock.getRawSize() << " bytes" << std::endl;
                    }
                }
                std::cout << std::endl;
            }
        }

        void dryIn() override { /* This should be called in the end after the last result ended up in flowIn */
            std::cout << "GabacCompressor finished!" << std::endl;
        }

        int veryImportantExampleInformation() { return 42; }   /* Dummy information needed for tests */
        bool veryImportantExampleTest() { return 1 + 1 == 2; } /* Dummy check */
    };

    genie::entropy::gabac::GabacCompressor compressor;
    DummyModule dummy;
    compressor.setDrain(&dummy);

    size_t numRecs = 50;  // Number of records. Not important, as we are not working with records at the level of gabac
    genie::core::AccessUnitRaw raw_aus(genie::core::parameter::ParameterSet(), numRecs);

    for (size_t i = 0; i < 50; ++i) {
        raw_aus.get(genie::core::GenSub::RLEN).push(i);  // Append some data for one selected subsequence
        // See core/constants.cpp/getDescriptors() or core/constants.h/GenSub for all valid descriptors and how many
        // subsequences there are for each descriptor (here we are only populating sequence 0). Maybe even use
        // different, random data for each run? You should vary test data a little bit to cover more cases and also use
        // different descriptor streams

        //  Example how to iterate descriptors, here we append data to every subsequence
        for (const auto& descriptor : genie::core::getDescriptors()) {
            for (const auto& subsequence : descriptor.subseqs) {
                raw_aus.get(subsequence.id).push(i);
            }
        }
    }

    // Execute
    compressor.flowIn(std::move(raw_aus), 0);
    compressor.dryIn();

    EXPECT_EQ(dummy.veryImportantExampleInformation(), 42);
    ASSERT_TRUE(dummy.veryImportantExampleTest());
}