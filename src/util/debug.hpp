#pragma once
#include <iomanip>
#include <sstream>

namespace debug {

std::string dump_memory(uint8_t const* mem, std::size_t length, std::size_t offset = 0);

}  // namespace debug
