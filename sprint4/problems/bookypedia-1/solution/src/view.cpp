#include "view.h"

#include <boost/algorithm/string/trim.hpp>
#include <iostream>
#include <string>

#include "menu.h"
#include "use_cases.h"

using namespace std::literals;
namespace ph = std::placeholders;

namespace ui {

View::View(menu::Menu& menu, app::UseCases& use_cases, std::istream& input, std::ostream& output)
    : menu_{menu}, use_cases_{use_cases}, input_{input}, output_{output} {
  menu_.AddAction(  //
      "AddAuthor"s, "name"s, "Adds author"s, std::bind(&View::AddAuthor, this, ph::_1)
      // ����
      // [this](auto& cmd_input) { return AddAuthor(cmd_input); }
  );

  menu_.AddAction(
    "AddBook"s, "name"s, "Add book"s, std::bind(&View::AddBook, this, ph::_1)
  );

  menu_.AddAction(
    "ShowAuthors"s, "name"s, "Add book"s, std::bind(&View::ShowAuthors, this, ph::_1)
  );
}

bool View::AddAuthor(std::istream& cmd_input) {
  try {
    std::string name;
    std::getline(cmd_input, name);
    boost::algorithm::trim(name);
    use_cases_.AddAuthor(std::move(name));
  } catch (const std::exception&) {
    output_ << "Failed to add author"sv << std::endl;
  }
  return true;
}


bool View::ShowAuthors(std::istream& cmd_input) {
  std::string ignore;
  std::getline(cmd_input, ignore);
  use_cases_.ShowAuthors(output_);
  return true;
}


bool View::AddBook(std::istream& cmd_input) {
  try {
    int year{0};
    std::string title;
    cmd_input >> year;
    std::getline(cmd_input, title);
    boost::algorithm::trim(title);

    // show question
    output_ << "Select author:" << std::endl;
    ShowAuthors(cmd_input);
    
    int id{0};
    output_ << "Enter author # or empty line to cancel" << std::endl;
    input_ >> id;
    return use_cases_.AddBook(year, title, id);

  } catch (const std::exception& e) {
    output_ << e.what() << '\n';
  }

  return false;
}

}  // namespace ui
