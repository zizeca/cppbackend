#include "postgres.h"

#include <pqxx/pqxx>
#include <pqxx/zview.hxx>

namespace postgres {

using namespace std::literals;
using pqxx::operator"" _zv;

void AuthorRepositoryImpl::Save(const domain::Author& author) {
  work_.exec_params(R"(
INSERT INTO authors (id, name) VALUES ($1, $2)
ON CONFLICT (id) DO UPDATE SET name=$2;
)"_zv,
                    author.GetId().ToString(), author.GetName());
  work_.commit();
}


void AuthorRepositoryImpl::ShowAuthors(std::ostream &output) {
  pqxx::read_transaction r(work_.conn());
  int counter = 1;
  for(const auto& [name] : r.query<std::string>("SELECT name FROM authors ORDER BY name;"_zv)) {
    output << counter++ << ". " << name << std::endl;
  }
}

std::optional<domain::AuthorId> AuthorRepositoryImpl::GetAuthorIdByIndex(int index) {
  pqxx::read_transaction r(work_.conn());
  index--;

  auto result = r.exec("SELECT * FROM authors ORDER BY name;"_zv);
  if(index < 0 || index >= result.size()) {
    return std::nullopt;
  }

  auto row = result.at(index);
  return  domain::AuthorId::FromString(row.at(0).c_str());
}


void BookRepositoryImpl::Save(const domain::Book& book) {
  work_.exec_params(R"(
INSERT INTO books (id, author_id, title, publication_year) VALUES ($1, $2, $3, $4)
ON CONFLICT (id) DO UPDATE SET author_id=$2, title=$3, publication_year=$4;
)"_zv,
                    book.GetId().ToString(), book.GetAuthorId().ToString(), book.GetTitle(), book.GetYear());
  work_.commit();
}

Database::Database(pqxx::connection connection)
    : connection_{std::move(connection)} {
  work_.exec(R"(
CREATE TABLE IF NOT EXISTS authors (
    id UUID CONSTRAINT author_id_constraint PRIMARY KEY,
    name varchar(100) UNIQUE NOT NULL
);
)"_zv);
  // ... создать другие таблицы

  work_.exec(R"(
CREATE TABLE IF NOT EXISTS books (
    id UUID CONSTRAINT book_id_constraint PRIMARY KEY,
    author_id UUID NOT NULL,
    title varchar(100) NOT NULL,
    publication_year integer NOT NULL
);
)"_zv);

  // коммитим изменения
  work_.commit();
}

}  // namespace postgres