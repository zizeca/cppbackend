#include "tv.h"


  /*
   * Возвращает информацию о том, включен телевизор или нет.
   */
 bool TV::IsTurnedOn() const noexcept {
    return is_turned_on_;
  }

  /*
   * Возвращает номер выбранного канала или std::nullopt, если телевизор выключен.
   */
std::optional<int> TV::GetChannel() const noexcept {
    return is_turned_on_ ? std::optional{channel_} : std::nullopt;
  }

  /*
   * Включает телевизор, если он был выключен. Если телевизор уже включен, ничего не делает.
   * При включении выбирается тот номер канала, который был выбран перед последним выключением.
   * При самом первом включении выбирает канал #1.
   */
  void TV::TurnOn() noexcept {
    is_turned_on_ = true;
  }

  /*
   * Выключает телевизор, если он был включен. Если телевизор уже выключен, ничего не делает.
   */
  void TV::TurnOff() noexcept {
    is_turned_on_ = false;
  }

  /*
   * Выбирает канал channel.
   * Ранее выбранный канал запоминается и может быть восстановлен методом SelectLastViewedChannel.
   * Если номер канала совпадает с ранее выбранным каналом, метод ничего не делает.
   * Если телевизор выключен, выбрасывает исключение std::logic_error.
   * Если номер канала за пределами диапазона MIN_CHANNEL, MAX_CHANNEL, выбрасывает out_of_range.
   */
  void TV::SelectChannel(int channel) {
    /* Реализуйте самостоятельно этот метод и напишите тесты для него */
    assert(!"TODO: Implement TV::SelectChannel");
  }

  /*
   * Выбирает номер канала, который был выбран перед последним вызовом SelectChannel.
   * Многократный последовательный вызов SelectChannel переключает два последних выбранных канала.
   * Если телевизор выключен, выбрасывает исключение std::logic_error.
   */
  void TV::SelectLastViewedChannel() {
    /* Реализуйте самостоятельно этот метод и напишите тесты для него */
    assert(!"TODO: Implement TV::SelectLastViewedChannel");
  }