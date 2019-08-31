#ifndef GENIE_PART2_FORMAT_BITWRITER_H
#define GENIE_PART2_FORMAT_BITWRITER_H

#include <ostream>
#include <cstdint>

class BitWriter {
private:
    std::ostream* stream;
    uint64_t m_heldBits;
    uint8_t m_numHeldBits;
    uint64_t m_bitsWritten;

    void writeOut(uint8_t byte);
public:
    explicit BitWriter(std::ostream* str);
    ~BitWriter();
    void write (uint64_t value, uint8_t bits);
    void flush ();
    uint64_t getBitsWritten();

};


#endif //GENIE_PART2_FORMAT_BITWRITER_H
