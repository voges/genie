/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/MueFab/genie for more details.
 */

#include "genie/entropy/gabac/streams.h"
#include <algorithm>
#include <cstdio>

// ---------------------------------------------------------------------------------------------------------------------

namespace genie::entropy::gabac {

// ---------------------------------------------------------------------------------------------------------------------

FileBuffer::FileBuffer(FILE *f) : fileptr(f) {}

// ---------------------------------------------------------------------------------------------------------------------

int FileBuffer::overflow(const int c) { return fputc(c, fileptr); }

// ---------------------------------------------------------------------------------------------------------------------

std::streamsize FileBuffer::xsputn(const char *s, const std::streamsize n) {
    return fwrite(s, 1, static_cast<size_t>(n), fileptr);
}

// ---------------------------------------------------------------------------------------------------------------------

int FileBuffer::sync() { return fflush(fileptr); }

// ---------------------------------------------------------------------------------------------------------------------

std::streamsize FileBuffer::xsgetn(char *s, const std::streamsize n) {
    return fread(s, 1, static_cast<size_t>(n), fileptr);
}

// ---------------------------------------------------------------------------------------------------------------------

int FileBuffer::underflow() { return fgetc(fileptr); }

// ---------------------------------------------------------------------------------------------------------------------

DataBlockBuffer::DataBlockBuffer(util::DataBlock *d, const size_t pos_i) : block(0, 1), pos(pos_i) { block.swap(d); }

// ---------------------------------------------------------------------------------------------------------------------

size_t DataBlockBuffer::size() const { return block.getRawSize(); }

// ---------------------------------------------------------------------------------------------------------------------

int DataBlockBuffer::overflow(const int c) {
    block.push_back(static_cast<uint64_t>(c));
    return c;
}

// ---------------------------------------------------------------------------------------------------------------------

std::streamsize DataBlockBuffer::xsputn(const char *s, const std::streamsize n) {
    if (block.modByWordSize(n)) {
        UTILS_DIE("Invalid Data length");
    }
    const size_t oldSize = block.size();
    block.resize(block.size() + block.divByWordSize(n));
    memcpy(static_cast<uint8_t *>(block.getData()) + block.mulByWordSize(oldSize), s, static_cast<size_t>(n));
    return n;
}

// ---------------------------------------------------------------------------------------------------------------------

std::streamsize DataBlockBuffer::xsgetn(char *s, const std::streamsize n) {
    if (block.modByWordSize(n)) {
        UTILS_DIE("Invalid Data length");
    }
    const size_t bytesRead = std::min<size_t>(block.getRawSize() - block.mulByWordSize(pos), static_cast<size_t>(n));
    memcpy(s, static_cast<uint8_t *>(block.getData()) + block.mulByWordSize(pos), bytesRead);
    pos += block.divByWordSize(bytesRead);
    return bytesRead;
}

// ---------------------------------------------------------------------------------------------------------------------

int DataBlockBuffer::underflow() {
    if (pos == block.size()) {
        return EOF;
    }
    return static_cast<int>(block.get(pos));
}

// ---------------------------------------------------------------------------------------------------------------------

int DataBlockBuffer::uflow() {
    if (pos == block.size()) {
        return EOF;
    }
    return static_cast<int>(block.get(pos++));
}

// ---------------------------------------------------------------------------------------------------------------------

void DataBlockBuffer::flush_block(util::DataBlock *blk) { block.swap(blk); }

// ---------------------------------------------------------------------------------------------------------------------

std::streambuf::pos_type DataBlockBuffer::seekpos(const pos_type sp, std::ios_base::openmode) { return pos = sp; }

// ---------------------------------------------------------------------------------------------------------------------

std::streambuf::pos_type DataBlockBuffer::seekoff(const off_type off, const std::ios_base::seekdir dir,
                                                  const std::ios_base::openmode which) {
    (void)which;
    if (dir == std::ios_base::cur)
        pos = off < 0 && static_cast<size_t>(std::abs(off)) > pos ? 0 : std::min<size_t>(pos + off, block.size());
    else if (dir == std::ios_base::end)
        pos = off < 0 && static_cast<size_t>(std::abs(off)) > block.size()
                  ? 0
                  : std::min<size_t>(block.size() + off, block.size());
    else if (dir == std::ios_base::beg)
        pos = off < 0 && std::abs(off) > 0 ? 0 : std::min<size_t>(static_cast<size_t>(0) + off, block.size());
    return pos;
}

// ---------------------------------------------------------------------------------------------------------------------

IBufferStream::IBufferStream(util::DataBlock *d, const size_t pos_i) : DataBlockBuffer(d, pos_i), std::istream(this) {}

// ---------------------------------------------------------------------------------------------------------------------

OBufferStream::OBufferStream(util::DataBlock *d) : DataBlockBuffer(d, 0), std::ostream(this) {}

// ---------------------------------------------------------------------------------------------------------------------

void OBufferStream::flush(util::DataBlock *blk) { flush_block(blk); }

// ---------------------------------------------------------------------------------------------------------------------

NullStream::NullStream() : std::ostream(&m_sb) {}

// ---------------------------------------------------------------------------------------------------------------------

}  // namespace genie::entropy::gabac

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
