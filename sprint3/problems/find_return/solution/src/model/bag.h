#ifndef __BAG_H__
#define __BAG_H__

#include <vector>

#include "loot.h"

namespace model {

class Bag {
 public:
  Bag() {}
  ~Bag() {}

  void SetMaxSize(size_t size);

  size_t Size() const noexcept;

  size_t Capacity() const noexcept;

  bool AddLoot( const Loot& loot);

  bool Full() const noexcept;


 private:
  std::vector<Loot> m_loots;
  size_t m_max_size;
};
}  // namespace model

#endif  // __BAG_H__