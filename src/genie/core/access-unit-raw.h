/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/mitogen/genie for more details.
 */

#ifndef GENIE_ACCESS_UNIT_RAW_H
#define GENIE_ACCESS_UNIT_RAW_H

// ---------------------------------------------------------------------------------------------------------------------

#include <genie/util/data-block.h>
#include "constants.h"
#include "parameter/parameter_set.h"

// ---------------------------------------------------------------------------------------------------------------------

namespace genie {
namespace core {

/**
 * @brief
 */
class AccessUnitRaw {
   public:
    /**
     * @brief
     */
    class Subsequence {
       private:
        util::DataBlock data;  //!< @brief
        size_t position;       //!< @brief

        GenSubIndex id;  //!< @brief

       public:
        /**
         * @brief
         * @param wordsize
         */
        Subsequence(size_t wordsize, GenSubIndex _id);

        /**
         * @brief
         * @param d
         */
        Subsequence(util::DataBlock d, GenSubIndex _id);

        /**
         * @brief
         * @param val
         */
        void push(uint64_t val);

        /**
         * @brief
         */
        void inc();

        /**
         * @brief
         * @return
         */
        uint64_t get(size_t lookahead = 0) const;

        /**
         * @brief
         * @return
         */
        bool end() const;

        /**
         * @brief
         * @param block
         */
        util::DataBlock&& move();

        /**
         * @brief
         * @return
         */
        GenSubIndex getID() const;

        uint64_t pull();

        size_t getNumSymbols() const { return data.size(); }
    };

    /**
     * @brief
     */
    class Descriptor {
       private:
        std::vector<Subsequence> subdesc;  //!< @brief
        GenDesc id;                        //!< @brief

       public:
        /**
         * @brief
         * @param sub
         * @return
         */
        Subsequence& get(uint16_t sub);

        const Subsequence& get(uint16_t sub) const { return subdesc[sub]; }

        Subsequence& getTokenType(uint16_t pos, uint8_t type) {
            uint16_t s_id = ((pos << 4u) | (type & 0xfu));
            while (subdesc.size() <= s_id) {
                subdesc.emplace_back(4, GenSubIndex(getID(), subdesc.size()));
            }
            return get(s_id);
        }

        /**
         * @brief
         * @return
         */
        GenDesc getID() const;

        /**
         * @brief
         * @param sub
         */
        void add(Subsequence&& sub);

        /**
         * @brief
         * @param _id
         * @param sub
         */
        void set(uint16_t _id, Subsequence&& sub);

        /**
         * @brief
         * @param _id
         */
        explicit Descriptor(GenDesc _id);

        Subsequence* begin() { return &subdesc.front(); }

        Subsequence* end() { return &subdesc.back() + 1; }

        const Subsequence* begin() const { return &subdesc.front(); }

        const Subsequence* end() const { return &subdesc.back() + 1; }

        size_t getSize() const { return subdesc.size(); }
    };

    /**
     * @brief
     * @param desc
     * @param sub
     * @return
     */
    const Subsequence& get(GenSubIndex sub) const;

    /**
     * @brief
     * @param desc
     * @param sub
     * @return
     */
    Subsequence& get(GenSubIndex sub);

    Descriptor& get(GenDesc desc) { return descriptors[uint8_t(desc)]; }
    const Descriptor& get(GenDesc desc) const { return descriptors[uint8_t(desc)]; }

    /**
     * @brief
     * @param desc
     * @param sub
     * @param data
     */
    void set(GenSubIndex sub, Subsequence&& data);

    /**
     *
     * @param sub
     * @param value
     */
    void push(GenSubIndex sub, uint64_t value);

    bool isEnd(GenSubIndex sub);

    uint64_t peek(GenSubIndex sub, size_t lookahead = 0);

    /**
     *
     * @param sub
     * @return
     */
    uint64_t pull(GenSubIndex sub);

    /**
     * @brief
     * @param set
     */
    AccessUnitRaw(parameter::ParameterSet&& set, size_t _numRecords);

    /**
     * @brief
     * @param _parameters
     */
    void setParameters(parameter::ParameterSet&& _parameters);

    /**
     * @brief
     * @return
     */
    const parameter::ParameterSet& getParameters() const;

    /**
     * @brief
     * @return
     */
    parameter::ParameterSet& getParameters();

    /**
     * @brief
     * @return
     */
    parameter::ParameterSet&& moveParameters();

    /**
     * @brief
     * @return
     */
    size_t getNumRecords() const;

    void setNumRecords(size_t recs) { numRecords = recs; }

    core::record::ClassType getClassType() const { return type; }

    void setClassType(core::record::ClassType _type) { type = _type; }

    void clear();

    void addRecord();

    void setReference(uint16_t ref);

    uint16_t getReference();

    void setMaxPos(uint64_t pos);

    void setMinPos(uint64_t pos);

    uint64_t getMaxPos() const;

    uint64_t getMinPos() const;

    Descriptor* begin() { return &descriptors.front(); }

    Descriptor* end() { return &descriptors.back() + 1; }

    const Descriptor* begin() const { return &descriptors.front(); }

    const Descriptor* end() const { return &descriptors.back() + 1; }

   private:
    std::vector<Descriptor> descriptors;  //!< @brief
    parameter::ParameterSet parameters;   //!< @brief

    size_t numRecords;  //!< @brief

    core::record::ClassType type{};
    uint64_t minPos;
    uint64_t maxPos;

    uint16_t referenceSequence;
};

// ---------------------------------------------------------------------------------------------------------------------

}  // namespace core
}  // namespace genie

// ---------------------------------------------------------------------------------------------------------------------

#endif  // GENIE_ACCESS_UNIT_RAW_H

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------