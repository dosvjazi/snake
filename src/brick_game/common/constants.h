#pragma once

namespace s21 {

enum class RunningInfo {
  kStart,
  kPlay,
  kPause,
  kGameOver,
  kGameWon,
  kExit,
};

enum class UserAction {
  kNone = 0,
  kStart,
  kPause,
  kUp,
  kDown,
  kRight,
  kLeft,
  kAction,
  kTerminate
};

struct GameInfo {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
};

}  // namespace s21
