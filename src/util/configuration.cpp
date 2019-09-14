#include "util/configuration.hpp"

#include <boost/program_options.hpp>

#include <cstdlib>
#include <iostream>

namespace util {
namespace po = boost::program_options;

namespace {
void show_help_and_exit(po::options_description const& desc, int exit_code = 1)
{
  std::cout << desc << std::endl;
  std::exit(exit_code);
}

template<typename T>
T get_required(po::options_description const& desc, po::variables_map const& vm, std::string const& opt)
{
  if (!vm.count(opt))
  {
    std::cout << "Required parameter missing: " << opt << std::endl;
    show_help_and_exit(desc, 2);
  }

  return vm[opt].as<T>();
}

template<typename T>
bool get_optional(po::variables_map const& vm, std::string const& opt, T& dest)
{
  if (!vm.count(opt))
    return false;

  dest = vm[opt].as<T>();
  return true;
}
}  // namespace

Configuration parse_args(int argc, char* argv[])
{
  po::options_description desc("Emulators");

  // clang-format off
  desc.add_options()
    ("help,h", "produce help message and exit.")
    ("verbose,v", "Be more verbose in logs.")
    ("trace,t", "Trace emulator execution.")
    ("rom", po::value<std::string>(), "ROM to load.")
    ("sanitize", "Sanitize emulation, break on failure.")
    ("spin", "Use spin locks for time manipulation.");
  // clang-format on

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if (vm.count("help"))
    show_help_and_exit(desc);

  Configuration conf;

  if (vm.count("verbose"))
    conf.verbose = true;

  if (vm.count("trace"))
    conf.trace = true;

  conf.emulation.rom = get_required<std::string>(desc, vm, "rom");

  if (vm.count("sanitize"))
    conf.emulation.sanitize = true;

  if (vm.count("spin"))
    conf.emulation.spin = true;

  return conf;
}
}  // namespace util
