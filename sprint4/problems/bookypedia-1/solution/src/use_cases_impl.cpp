#include "use_cases_impl.h"

#include "author.h"

namespace app {
using namespace domain;

void UseCasesImpl::AddAuthor(const std::string& name) {
  authors_.Save({AuthorId::New(), name});
}

void UseCasesImpl::AddBook(int year, const std::string& title, const std::string& author_id) {
  // books_Save()
}

void UseCasesImpl::ShowAuthors(std::ostream &ostream) {
  authors_.ShowAuthors(ostream);
}

}  // namespace app
