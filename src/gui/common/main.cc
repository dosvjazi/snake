#ifdef QT_GUI_LIB
#include <QApplication>
#endif
#include <iostream>
#include <limits>

#include "base_controller.h"
#include "base_view.h"
#ifdef QT_GUI_LIB
#include "desktop_view.h"
#else
#include "console_view.h"
#endif
#include "snake_controller.h"
#include "tetris_controller.h"

namespace s21 {

void BeginPlay() {
  BaseController *controller;
  BaseView *view;
  int choice = -1;
  while (choice != 3) {
    std::cout << "Please select a game!" << std::endl;
    std::cout << "1: Tetris." << std::endl;
    std::cout << "2: Snake." << std::endl;
    std::cout << "3: Exit the brick game." << std::endl;
    std::cin >> choice;
    if (!std::cin) {
      std::cout << "Wrong input! Try again." << std::endl;
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      continue;
    }
#ifdef QT_GUI_LIB
    int argc_ = 0;
    QApplication app(argc_, NULL);
#endif
    switch (choice) {
#ifdef QT_GUI_LIB

      case 1:
        controller = new TetrisController();
        view = new DesktopView(controller);
        view->MainLoop();
        app.exec();
        delete view;
        std::cout << "Exited the game" << std::endl;
        break;
      case 2:
        controller = new SnakeController();
        view = new DesktopView(controller);
        view->MainLoop();
        app.exec();
        delete view;
        std::cout << "Exited the game" << std::endl;
        break;
#else
      case 1:
        controller = new TetrisController();
        view = new ConsoleView(controller);
        view->MainLoop();
        delete view;
        std::cout << "Exited the game" << std::endl;
        break;
      case 2:
        controller = new SnakeController();
        view = new ConsoleView(controller);
        view->MainLoop();
        delete view;
        std::cout << "Exited the game" << std::endl;
        break;
#endif
      case 3:
        std::cout << "change da world" << std::endl;
        std::cout << "my final message. Goodbye" << std::endl;
        break;
      default:
        std::cout << "Wrong input! Try again." << std::endl;
        break;
    }
  }
}

}  // namespace s21

int main(void) {
  s21::BeginPlay();
  return 0;
}