#include <gtest/gtest.h>

#include "snake_model.h"
#include "tetris_model.h"

using namespace s21;

TEST(Tetris, All) {
  TetrisModel* model = new TetrisModel();
  model->UserInput(UserAction::kStart, true);
  GameInfo game_info = model->UpdateCurrentState();
  model->UserInput(UserAction::kAction, true);
  game_info = model->UpdateCurrentState();
  model->UserInput(UserAction::kDown, true);
  game_info = model->UpdateCurrentState();
  model->UserInput(UserAction::kRight, true);
  game_info = model->UpdateCurrentState();
  model->UserInput(UserAction::kLeft, true);
  game_info = model->UpdateCurrentState();
  model->UserInput(UserAction::kUp, true);
  game_info = model->UpdateCurrentState();
  while (game_info.pause != (int)RunningInfo::kGameOver) {
    game_info = model->UpdateCurrentState();
    model->UserInput(UserAction::kAction, true);
  }
  model->UserInput(UserAction::kStart, true);
  model->UpdateCurrentState();
  model->UserInput(UserAction::kStart, true);
  game_info = model->UpdateCurrentState();
  model->UserInput(UserAction::kPause, true);
  game_info = model->UpdateCurrentState();
  model->UserInput(UserAction::kTerminate, true);
  game_info = model->UpdateCurrentState();
  EXPECT_EQ(game_info.pause, (int)RunningInfo::kExit);
}

TEST(Snake, All) {
  SnakeModel* model = new SnakeModel();
  model->UserInput(UserAction::kStart, true);
  GameInfo game_info = model->UpdateCurrentState();
  model->UserInput(UserAction::kLeft, true);
  game_info = model->UpdateCurrentState();
  model->UserInput(UserAction::kUp, true);
  game_info = model->UpdateCurrentState();
  model->UserInput(UserAction::kRight, true);
  game_info = model->UpdateCurrentState();
  model->UserInput(UserAction::kDown, true);
  game_info = model->UpdateCurrentState();
  while (game_info.pause != (int)RunningInfo::kGameOver) {
    game_info = model->UpdateCurrentState();
    model->UserInput(UserAction::kAction, true);
  }
  model->UserInput(UserAction::kStart, true);
  model->UpdateCurrentState();
  model->UserInput(UserAction::kStart, true);
  game_info = model->UpdateCurrentState();
  model->UserInput(UserAction::kPause, true);
  game_info = model->UpdateCurrentState();
  model->UserInput(UserAction::kTerminate, true);
  game_info = model->UpdateCurrentState();
  EXPECT_EQ(game_info.pause, (int)RunningInfo::kExit);
}

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}