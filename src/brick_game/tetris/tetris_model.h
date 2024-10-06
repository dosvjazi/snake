#pragma once

#include "constants.h"

#define EMPTY 0
#define PLACED 1
#define MOVING 2
#define CENTER 3

#define FIELD_X_START 0
#define FIELD_X_COUNT 10

#define EXTRA_Y_START 0
#define EXTRA_Y_COUNT 2

#define TEMP_FIELD_Y_START 2
#define FIELD_Y_START 0
#define FIELD_Y_COUNT 20

#define TOTAL_Y_START 0
#define TOTAL_Y_COUNT 22

#define TETRA_A_START 0
#define TETRA_X_COUNT 4
#define TETRA_Y_COUNT 2
#define TETRA_X_OFFSET 3
#define TETRA_Y_OFFSET 2

namespace s21 {

class TetrisModel {
 private:
  enum class GameState {
    kGameStart,
    kSpawn,
    kMove,
    kShift,
    kAttach,
    kGameOver,
  };

  int **field_;
  GameInfo game_info_;
  GameState game_state_;
  UserAction user_action_;
  bool hold_;
  int timer_;

  const int speed_list[10] = {60, 45, 30, 20, 10, 5, 4, 3, 2, 1};
  const int tetromine_list[7][2][4] = {
      {{0, 0, 0, 0}, {2, 2, 3, 2}}, {{0, 2, 0, 0}, {0, 2, 3, 2}},
      {{0, 0, 0, 2}, {0, 2, 3, 2}}, {{0, 2, 2, 0}, {0, 2, 2, 0}},
      {{0, 2, 3, 0}, {0, 0, 2, 2}}, {{0, 0, 2, 0}, {0, 2, 3, 2}},
      {{0, 0, 3, 2}, {0, 2, 2, 0}},
  };

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
  void Attach();
  void GameOver();
  void Pause();

  void GenerateTetromine();
  void PlaceTetromine();

  void MoveRight();
  void MoveLeft();
  void Rotate();
  bool CheckInside(int y, int x, int y_center, int x_center);
  bool CheckOutside(int y, int x, int y_center, int x_center);

  void CheckForGameOver();
  void CheckForFullRows();
  void DestroyFullRows(int start);

  void ConvertField();
  void FreeMemory();
};

}  // namespace s21