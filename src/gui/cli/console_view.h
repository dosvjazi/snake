#pragma once

#include <ncurses.h>

#include "base_controller.h"
#include "base_view.h"
#include "constants.h"

#define UP_BUTTON 3
#define DOWN_BUTTON 2
#define LEFT_BUTTON 4
#define RIGHT_BUTTON 5
#define ACTION_BUTTON ' '
#define PAUSE_BUTTON 'p'
#define TERMINATE_BUTTON '\e'
#define START_BUTTON '\n'

namespace s21 {

class ConsoleView : public BaseView {
 private:
  BaseController *controller_;

  void UpdateField(GameInfo game_info);
  void UpdateInfo(GameInfo game_info);
  void PrintRectangle(int top_y, int bottom_y, int left_x, int right_x);
  UserAction InputToAction(int input);

 public:
  ConsoleView(BaseController *c) : controller_(c) {};
  ~ConsoleView() { delete controller_; };

  void MainLoop() override;
};

}  // namespace s21