#ifndef __MENU_H__
#define __MENU_H__

#include <iostream>
#include <string>
#include <vector>

class Menu {
 public:
  Menu(std::istream& is, std::ostream& os);

  ~Menu();

  template <typename Handler, typename... Args>
  void AddAction(const std::string& name, const std::string& description, Handler&& handler, Args&&... args);

  void Run();

  void ShowInstructions();

 private:
  std::istream& m_is;
  std::ostream& m_os;
};

template <typename Handler, typename... Args>
void Menu::AddAction(const std::string& name, const std::string& description, Handler&& handler, Args&&... args) {
}

#endif  // __MENU_H__