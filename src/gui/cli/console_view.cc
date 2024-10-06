#include "console_view.h"

#include <stdio.h>
#include <unistd.h>

namespace s21 {

void ConsoleView::MainLoop() {
  initscr();
  keypad(stdscr, true);
  noecho();
  cbreak();
  nodelay(stdscr, true);
  curs_set(0);
  set_escdelay(0);
  timeout(0);

  clear();
  refresh();

  GameInfo game_info = controller_->UpdateCurrentState();

  while (game_info.pause != (int)RunningInfo::kExit) {
    flushinp();
    napms(17);

    char input = getch();

    controller_->UserInput(InputToAction(input), true);

    UpdateField(game_info);
    UpdateInfo(game_info);

    game_info = controller_->UpdateCurrentState();
  }

  mvprintw(20, 25, " PEACE OUT! ");
  refresh();
  napms(1000);
  nodelay(stdscr, false);
  refresh();
  endwin();
}

void ConsoleView::UpdateField(GameInfo game_info) {
  PrintRectangle(0, 21, 0, 21);
  for (int x = 0; x < 10; x++) {
    for (int y = 0; y < 20; y++) {
      if (game_info.field[y][x] != 0) {
        mvaddch(y + 1, 2 * x + 1, (char)0x2588);
        mvaddch(y + 1, 2 * x + 2, (char)0x2588);
      } else {
        mvaddch(y + 1, 2 * x + 1, ' ');
        mvaddch(y + 1, 2 * x + 2, ' ');
      }
    }
  }
}

void ConsoleView::UpdateInfo(GameInfo game_info) {
  PrintRectangle(0, 3, 24, 37);

  mvprintw(1, 26, "LEVEL");
  mvprintw(2, 26, "%d", game_info.level + 1);

  PrintRectangle(4, 7, 24, 37);

  mvprintw(5, 26, "SCORE");
  mvprintw(6, 26, "%d", game_info.score);

  PrintRectangle(8, 11, 24, 37);

  mvprintw(9, 26, "HIGH SCORE");
  mvprintw(10, 26, "%d", game_info.high_score);

  PrintRectangle(12, 18, 24, 37);

  if (game_info.next != NULL) {
    mvprintw(13, 29, "NEXT");

    for (int y = 0; y < 2; y++) {
      for (int x = 0; x < 4; x++) {
        if (game_info.next[y][x] != 0) {
          mvaddch(15 + y, 27 + x * 2, (char)0x2588);
          mvaddch(15 + y, 27 + x * 2 + 1, (char)0x2588);
        } else {
          mvaddch(15 + y, 27 + x * 2, ' ');
          mvaddch(15 + y, 27 + x * 2 + 1, ' ');
        }
      }
    }
  }

  mvprintw(20, 25, "  PLAYING!  ");

  PrintRectangle(19, 21, 24, 37);

  if (game_info.pause == (int)RunningInfo::kGameOver)
    mvprintw(20, 25, " GAME OVER! ");
  else if (game_info.pause == (int)RunningInfo::kPause)
    mvprintw(20, 25, "   PAUSE!   ");
  else if (game_info.pause == (int)RunningInfo::kStart)
    mvprintw(20, 25, "PRESS START!");
  else if (game_info.pause == (int)RunningInfo::kGameWon)
    mvprintw(20, 25, "!!GAME WON!!");
}

void ConsoleView::PrintRectangle(int top_y, int bottom_y, int left_x,
                                 int right_x) {
  mvaddch(top_y, left_x, ACS_ULCORNER);
  mvaddch(bottom_y, right_x, ACS_LRCORNER);
  mvaddch(top_y, right_x, ACS_URCORNER);
  mvaddch(bottom_y, left_x, ACS_LLCORNER);

  for (int i = left_x + 1; i < right_x; i++) {
    mvaddch(top_y, i, ACS_HLINE);
    mvaddch(bottom_y, i, ACS_HLINE);
  }

  for (int i = top_y + 1; i < bottom_y; i++) {
    mvaddch(i, left_x, ACS_VLINE);
    mvaddch(i, right_x, ACS_VLINE);
  }
}

UserAction ConsoleView::InputToAction(int input) {
  UserAction action = UserAction::kNone;

  if (input == UP_BUTTON)
    action = UserAction::kUp;
  else if (input == DOWN_BUTTON)
    action = UserAction::kDown;
  else if (input == LEFT_BUTTON)
    action = UserAction::kLeft;
  else if (input == RIGHT_BUTTON)
    action = UserAction::kRight;
  else if (input == ACTION_BUTTON)
    action = UserAction::kAction;
  else if (input == PAUSE_BUTTON)
    action = UserAction::kPause;
  else if (input == TERMINATE_BUTTON)
    action = UserAction::kTerminate;
  else if (input == START_BUTTON)
    action = UserAction::kStart;

  return action;
}

}  // namespace s21