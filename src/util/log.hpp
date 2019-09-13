#pragma once
#include "util/configuration.hpp"

#include <memory>
#include <spdlog/spdlog.h>

namespace logging {
using namespace spdlog;

using Ptr = std::shared_ptr<spdlog::logger>;

void setup(util::Configuration const& conf);

inline Ptr logger(std::string const& name)
{
  return default_logger()->clone(name);
}

}  // namespace logging
