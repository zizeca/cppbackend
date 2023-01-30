#ifndef __CONN_FACT_H__
#define __CONN_FACT_H__

#include <pqxx/connection.hxx>
#include <pqxx/zview.hxx>

#include "conn_pool.h"

namespace dbconn {

using namespace std::literals;
using pqxx::operator"" _zv;

class ConnectionFactory {
 public:
  explicit ConnectionFactory(const std::string& db_url) : m_url(db_url) {
    pqxx::connection conn(m_url);
    pqxx::work work(conn);
    work.exec(R"(
CREATE TABLE IF NOT EXISTS hall_of_fame (
    id UUID CONSTRAINT id_constraint PRIMARY KEY,
    name varchar(100) UNIQUE NOT NULL,
    score integer NOT NULL,
    play_time float8 NOT NULL
);
)"_zv);
    work.commit();
  }
  ~ConnectionFactory() = default;

  ConnectionPool::ConnectionPtr operator()() {
    auto ptr = std::make_shared<ConnectionPool::ConnectionPtr::element_type>(m_url);
    return ptr;
  }

 private:
  std::string m_url;
};

}  // namespace conn

#endif  // __CONN_FACT_H__