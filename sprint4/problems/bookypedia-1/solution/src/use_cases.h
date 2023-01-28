#pragma once

#include <string>
#include <ostream>

namespace app {

class UseCases {
 public:
  virtual void AddAuthor(const std::string& name) = 0;
  virtual void AddBook(int year, const std::string& title, const std::string& id) = 0;
  
  virtual void ShowAuthors(std::ostream& ostream) = 0;

 protected:
  ~UseCases() = default;
};

}  // namespace app
