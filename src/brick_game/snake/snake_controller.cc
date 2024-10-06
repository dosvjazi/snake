#include "snake_controller.h"

namespace s21 {

void SnakeController::UserInput(UserAction action, bool hold) {
  model_->UserInput(action, hold);
}

GameInfo SnakeController::UpdateCurrentState() {
  return model_->UpdateCurrentState();
}

}  // namespace s21
