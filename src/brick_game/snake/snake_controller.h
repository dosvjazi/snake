#pragma once

#include "base_controller.h"
#include "constants.h"
#include "snake_model.h"

namespace s21 {

class SnakeController : public BaseController {
 private:
  SnakeModel *model_;

 public:
  SnakeController() { model_ = new SnakeModel(); };
  ~SnakeController() { delete model_; };

  void UserInput(UserAction action, bool hold) override;
  GameInfo UpdateCurrentState() override;
};

}  // namespace s21