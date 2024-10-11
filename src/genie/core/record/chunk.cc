/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/MueFab/genie for more details.
 */

#include "genie/core/record/chunk.h"
#include <utility>
#include <vector>

// ---------------------------------------------------------------------------------------------------------------------

namespace genie::core::record {

// ---------------------------------------------------------------------------------------------------------------------

ReferenceManager::ReferenceExcerpt& Chunk::getRef() { return reference; }

// ---------------------------------------------------------------------------------------------------------------------

void Chunk::addRefToWrite(size_t start, size_t end) { refToWrite.emplace_back(start, end); }

// ---------------------------------------------------------------------------------------------------------------------

const std::vector<std::pair<size_t, size_t>>& Chunk::getRefToWrite() const { return refToWrite; }

// ---------------------------------------------------------------------------------------------------------------------

const ReferenceManager::ReferenceExcerpt& Chunk::getRef() const { return reference; }

// ---------------------------------------------------------------------------------------------------------------------

void Chunk::setRefID(const size_t id) { refID = id; }

// ---------------------------------------------------------------------------------------------------------------------

size_t Chunk::getRefID() const { return refID; }

// ---------------------------------------------------------------------------------------------------------------------

std::vector<Record>& Chunk::getData() { return data; }

// ---------------------------------------------------------------------------------------------------------------------

const std::vector<Record>& Chunk::getData() const { return data; }

// ---------------------------------------------------------------------------------------------------------------------

stats::PerfStats& Chunk::getStats() { return stats; }

// ---------------------------------------------------------------------------------------------------------------------

void Chunk::setStats(stats::PerfStats&& s) { stats = std::move(s); }

// ---------------------------------------------------------------------------------------------------------------------

bool Chunk::isReferenceOnly() const { return referenceOnly; }

// ---------------------------------------------------------------------------------------------------------------------

void Chunk::setReferenceOnly(const bool ref) { referenceOnly = ref; }

// ---------------------------------------------------------------------------------------------------------------------

}  // namespace genie::core::record

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
