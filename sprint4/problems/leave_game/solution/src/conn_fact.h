#ifndef __CONN_FACT_H__
#define __CONN_FACT_H__

#include <pqxx/connection.hxx>
#include <pqxx/zview.hxx>

#include "conn_pool.h"

namespace dbconn {

using namespace std::literals;
using pqxx::operator"" _zv;

struct ColName
{
  static constexpr char token[] = "token";
  static constexpr char name[] = "name";
  static constexpr char score[] = "score";
  static constexpr char play_time[] = "play_time";
};


class ConnectionFactory {
 public:
  static constexpr auto insert_data = "insert_data"_zv;
  static constexpr auto update_data = "update_data"_zv;
  static constexpr auto get_data = "get_data"_zv;
  
  // for read 
  static constexpr size_t MaxItemReq = 100u; 

  explicit ConnectionFactory(const std::string& db_url) : m_url(db_url) {
    pqxx::connection conn(m_url);
    pqxx::work work(conn);
    work.exec(R"(
CREATE TABLE IF NOT EXISTS hall_of_fame (
    token varchar(32) CONSTRAINT token_constraint PRIMARY KEY,
    name varchar(32) NOT NULL,
    score integer NOT NULL,
    play_time float8 NOT NULL
);
)"_zv);
    work.commit();
  }
  ~ConnectionFactory() = default;

  ConnectionPool::ConnectionPtr operator()() {
    auto ptr = std::make_shared<ConnectionPool::ConnectionPtr::element_type>(m_url);

    ptr->prepare(insert_data, "INSERT INTO hall_of_fame (token, name, score, play_time) VALUES ($1, $2, $3, $4)"_zv);
    ptr->prepare(update_data, "UPDATE hall_of_fame SET name=$2, score=$3, play_time=$4 WHERE token=$1"_zv);
    ptr->prepare(get_data, "SELECT * FROM hall_of_fame ORDER BY score DESC, play_time ASC, name ASC OFFSET $1 LIMIT $2 "_zv);

    return ptr;
  }

 private:
  std::string m_url;
};

}  // namespace dbconn

#endif  // __CONN_FACT_H__