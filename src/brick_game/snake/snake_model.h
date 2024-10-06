#pragma once

#include "constants.h"

#define EMPTY 0
#define SNAKE 1
#define APPLE 2

#define FIELD_X_START 0
#define FIELD_X_COUNT 10

#define FIELD_Y_START 0
#define FIELD_Y_COUNT 20

namespace s21 {

class SnakeModel {
 private:
  enum class GameState {
    kGameStart,
    kSpawn,
    kMove,
    kShift,
    kEat,
    kGameOver,
    kGameWon,
  };

  struct SnakePart {
    int y;
    int x;
  };

  struct Snake {
    int y;
    int x;
    int y_dir;
    int x_dir;
    SnakePart next[199];
  };

  struct Apple {
    int y;
    int x;
  };

  const int speed_list[10] = {40, 35, 30, 25, 20, 18, 16, 14, 12, 10};

  GameInfo game_info_;
  GameState game_state_;
  UserAction user_action_;
  bool hold_;
  int timer_;
  Snake snake_;
  Apple apple_;

 public:
  void UserInput(UserAction action, bool hold);
  GameInfo UpdateCurrentState();

 private:
  void FiniteStateMachine();

  void InitGameInfo();

  void LoadHighScore();
  void SaveHighScore();

  void GameStart();
  void Spawn();
  void Moving();
  void Shifting();
  void Eat();
  void GameOver();
  void GameWon();
  void Pause();

  void DrawSnake();
  void GenerateApple();

  void CheckForGameWon();

  void FreeMemory();
};

}  // namespace s21