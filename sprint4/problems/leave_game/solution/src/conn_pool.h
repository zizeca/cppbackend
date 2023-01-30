#ifndef __CONN_POOL_H__
#define __CONN_POOL_H__

#include <pqxx/pqxx>
#include <pqxx/connection.hxx>

#include <cassert>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>

class ConnectionPool : public std::enable_shared_from_this<ConnectionPool> {
 public:
  using PoolType = ConnectionPool;
  using PoolTypePtr = std::shared_ptr<PoolType>;
  using ConnectionPtr = std::shared_ptr<pqxx::connection>;

  class ConnectionWrapper {
   public:
    ConnectionWrapper(std::shared_ptr<pqxx::connection>&& conn, PoolTypePtr pool) noexcept
        : m_conn{std::move(conn)}, m_pool{pool} {
    }

    ConnectionWrapper(const ConnectionWrapper&) = delete;
    ConnectionWrapper& operator=(const ConnectionWrapper&) = delete;

    ConnectionWrapper(ConnectionWrapper&&) = default;
    ConnectionWrapper& operator=(ConnectionWrapper&&) = default;

    pqxx::connection& operator*() const& noexcept {
      return *m_conn;
    }
    pqxx::connection& operator*() const&& = delete;

    pqxx::connection* operator->() const& noexcept {
      return m_conn.get();
    }

    ~ConnectionWrapper() {
      if (m_conn) {
        m_pool->ReturnConnection(std::move(m_conn));
      }
    }

   private:
    std::shared_ptr<pqxx::connection> m_conn;
    PoolTypePtr m_pool;
  };

  // ConnectionFactory is a functional object returning std::shared_ptr<pqxx::connection>
  template <typename ConnectionFactory>
  ConnectionPool(size_t capacity, ConnectionFactory&& connection_factory) {
    m_conn_arr.reserve(capacity);
    for (size_t i = 0; i < capacity; ++i) {
      m_conn_arr.emplace_back(connection_factory());
    }
  }

  ConnectionWrapper GetConnection() {
    std::unique_lock lock{mutex_};

    // Блокируем текущий поток и ждём, пока m_cond_var не получит уведомление и не освободится
    // хотя бы одно соединение
    m_cond_var.wait(lock, [this] {
      return used_connections_ < m_conn_arr.size();
    });
    // После выхода из цикла ожидания мьютекс остаётся захваченным

    return {std::move(m_conn_arr[used_connections_++]), shared_from_this()};
  }

 private:
  void ReturnConnection(ConnectionPtr&& conn) {
    // Возвращаем соединение обратно в пул
    {
      std::lock_guard lock{mutex_};
      assert(used_connections_ != 0);
      m_conn_arr[--used_connections_] = std::move(conn);
    }
    // Уведомляем один из ожидающих потоков об изменении состояния пула
    m_cond_var.notify_one();
  }

  std::mutex mutex_;
  std::condition_variable m_cond_var;
  std::vector<ConnectionPtr> m_conn_arr;
  size_t used_connections_ = 0;
};

#endif  // __CONN_POOL_H__