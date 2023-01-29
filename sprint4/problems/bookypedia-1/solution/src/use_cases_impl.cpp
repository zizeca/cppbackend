#include "use_cases_impl.h"

#include <optional>

#include "author.h"
#include "book.h"

namespace app {
using namespace domain;

void UseCasesImpl::AddAuthor(const std::string& name) {
  authors_.Save({AuthorId::New(), name});
}

bool UseCasesImpl::AddBook(int year, const std::string& title, int author_id) {
  std::optional<AuthorId> id = authors_.GetAuthorIdByIndex(author_id);

  if(id) {
    books_.Save({BookId::New(), *id, year, title});
    return true;
  }

  return false;
}

void UseCasesImpl::ShowAuthors(std::ostream &ostream) {
  authors_.ShowAuthors(ostream);
}

void UseCasesImpl::ShowAuthorBooks(std::ostream &ostream, int author_id) {
  std::optional<AuthorId> id = authors_.GetAuthorIdByIndex(author_id);
  if(id) {
    books_.ShowAuthorBooks(ostream, *id);
  }
}

void UseCasesImpl::ShowBooks(std::ostream &ostream) {
  books_.ShowBooks(ostream);
}

}  // namespace app
