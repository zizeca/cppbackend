#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include <iostream>

#include "tv.h"
#include "menu.h"



class Controller {
 public:
  Controller(Tv& tv, Menu& menu);



 private:
  Tv& m_tv;
  Menu& m_menu;

 private:
  void ShowInfo(std::istream& input, std::ostream& output) const;

  void TurnOn(std::istream& input, std::ostream& output);

  void SelectChannel(std::istream& input, std::ostream& output);

  void SelectPreviousChannel(std::istream& input, std::ostream& output);

};

#endif  // __CONTROLLER_H__