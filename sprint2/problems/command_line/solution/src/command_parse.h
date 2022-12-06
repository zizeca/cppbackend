#ifndef __COMMAND_PARSE_H__
#define __COMMAND_PARSE_H__


#include <boost/program_options.hpp>
#include <fstream>
#include <iostream>
#include <optional>
#include <vector>

using namespace std::literals;

struct Args {
  std::vector<std::string> source;
  std::string destination;
};

[[nodiscard]] std::optional<Args> ParseCommandLine(int argc, const char* const argv[]) {
  namespace po = boost::program_options;

  po::options_description desc{"All options"s};
  Args args;
  desc.add_options()           //
      ("help,h", "Show help")  //
      ("tick-period,t", "Tick time for update state")  //
      ("config-file", "Path to configuration file");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if (vm.contains("help"s)) {
    std::cout << desc;
    return std::nullopt;
  }



  return std::nullopt;
}



#endif // __COMMAND_PARSE_H__