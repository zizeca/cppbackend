#ifndef __COMMAND_PARSE_H__
#define __COMMAND_PARSE_H__

#include <boost/program_options.hpp>
#include <optional>

using namespace std::literals;

struct Args {
  unsigned int period = 0;
  std::string config_file;
  std::string www_root;
  bool random = false;
};

[[nodiscard]] std::optional<Args> ParseCommandLine(int argc, const char* const argv[]) {
  namespace po = boost::program_options;

  po::options_description desc{"All options"s};
  Args args;
  desc.add_options()                                                                                            //
      ("help,h", "produce help message")                                                                        //
      ("tick-period,t", po::value<unsigned int>(&args.period)->value_name("milliseconds"), "set tick period")            //
      ("config-file,c", po::value<std::string>(&args.config_file)->value_name("file"), "set config file path")  //
      ("www-root,w", po::value<std::string>(&args.www_root)->value_name("dir"), "set static files root")        //
      ("randomize-spawn-points", po::value<bool>(&args.random), "spawn dogs at random positions");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if (vm.contains("help"s)) {
    std::cout << desc;
    return std::nullopt;
  }

  if (!vm.contains("config-file"s)) {
    throw std::runtime_error("Config file is not specified"s);
  }

  if (!vm.contains("www-root"s)) {
    throw std::runtime_error("WWW Root directory is not specified"s);
  }

  return args;
}

#endif  // __COMMAND_PARSE_H__