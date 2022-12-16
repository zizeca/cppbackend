#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include "tv.h"
#include "menu.h"

class Controller {
 public:
  Controller(const Tv& tv, const Menu& menu);

 private:
  Tv& m_tv;
  Menu& m_menu;
};

#endif  // __CONTROLLER_H__