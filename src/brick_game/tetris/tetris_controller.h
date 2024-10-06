#pragma once

#include "base_controller.h"
#include "constants.h"
#include "tetris_model.h"

namespace s21 {

class TetrisController : public BaseController {
 private:
  TetrisModel *model_;

 public:
  TetrisController() { model_ = new TetrisModel(); };
  ~TetrisController() { delete model_; };

  void UserInput(UserAction action, bool hold) override;
  GameInfo UpdateCurrentState() override;
};

}  // namespace s21