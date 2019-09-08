#pragma once
#include "util/configuration.hpp"

#include <memory>
#include <spdlog/spdlog.h>

namespace logging {
using namespace spdlog;

using LoggerPtr = std::shared_ptr<spdlog::logger>;

void setup(util::Configuration const& conf);

inline LoggerPtr logger(std::string const& name)
{
  return default_logger()->clone(name);
}

}  // namespace logging
