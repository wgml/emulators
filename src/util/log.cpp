#include "util/log.hpp"

namespace logging {

void setup(util::Configuration const& conf)
{
  auto lvl = level::info;
  if (conf.verbose)
    lvl = level::debug;
  if (conf.trace)
    lvl = level::trace;

  set_level(lvl);

  info("Logging configured");
}
}  // namespace logging
