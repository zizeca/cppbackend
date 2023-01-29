#ifndef __CONN_FACT_H__
#define __CONN_FACT_H__

#include <pqxx/connection.hxx>

#include "conn_pool.h"

class ConnectionFactory
{
public:
  explicit ConnectionFactory(const std::string & db_url) : m_url(db_url) {}
  ~ConnectionFactory() = default;


  ConnectionPool::ConnectionPtr operator()() {
    auto ptr = std::make_shared<ConnectionPool::ConnectionPtr::element_type>(m_url);
    return ptr;
  }

private:
  std::string m_url;

};




#endif // __CONN_FACT_H__