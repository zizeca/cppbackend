#pragma once

#include <string>
#include <ostream>

namespace app {

class UseCases {
 public:
  virtual void AddAuthor(const std::string& name) = 0;
  virtual bool AddBook(int year, const std::string& title, int author_id) = 0;
  
  virtual void ShowAuthors(std::ostream& ostream) = 0;
  virtual void ShowAuthorBooks(std::ostream &ostream, int id) = 0;
  virtual void ShowBooks(std::ostream &ostream) = 0;

 protected:
  ~UseCases() = default;
};

}  // namespace app
