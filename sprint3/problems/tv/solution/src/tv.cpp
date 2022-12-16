#include "tv.h"

Tv::Tv() : m_turn_on(false), m_channel(1) 
{
}

Tv::~Tv() {
}

bool Tv::IsTurnedOn() const {
  return m_turn_on;
}

std::optional<int> Tv::GetChannel() const {
  if(m_turn_on){
    return m_channel;
  }
  return std::nullopt;
}

void Tv::TurnOn() {
  m_turn_on = true;
  if(m_channel <= 0) {
    m_channel = 1;
  }
}

void Tv::TurnOff() {
  m_turn_on = false;
}
