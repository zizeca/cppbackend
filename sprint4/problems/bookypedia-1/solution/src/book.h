#pragma once
#include <string>

#include "tagged_uuid.h"
#include "author.h"

namespace domain {

namespace detail {
struct BookTag {};
}  // namespace detail

using BookId = util::TaggedUUID<detail::BookTag>;

class Book {
 public:
  Book(BookId id, AuthorId author_id, std::string title)
      : m_id(id),
        m_author_id(author_id),
        m_title(title) {}

  const BookId& GetId() const noexcept {
    return m_id;
  }

  const AuthorId& GetAuthorId() const noexcept {
    return m_author_id;
  }

  const std::string& GetTitle() const noexcept {
    return m_title;
  }

 private:
  BookId m_id;
  AuthorId m_author_id;
  std::string m_title;
};

class BookRepository {
 public:
  virtual void Save(const Book& book) = 0;

 protected:
  ~BookRepository() = default;
};

}  // namespace domain