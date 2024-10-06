#include "tetris_controller.h"

namespace s21 {

void TetrisController::UserInput(UserAction action, bool hold) {
  model_->UserInput(action, hold);
}

GameInfo TetrisController::UpdateCurrentState() {
  return model_->UpdateCurrentState();
}

}  // namespace s21
