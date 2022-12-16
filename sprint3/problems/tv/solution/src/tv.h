#ifndef __TV_H__
#define __TV_H__

#include <optional>

class Tv
{
public:
  Tv();

  ~Tv();

  bool IsTurnedOn() const;

  std::optional<int> GetChannel() const;

  void TurnOn();

  void TurnOff();

private:
  bool m_turn_on;
  int m_channel;
};




#endif // __TV_H__