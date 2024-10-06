#pragma once

#include "constants.h"

namespace s21 {

class BaseController {
 public:
  virtual ~BaseController() {};

  virtual void UserInput(UserAction action, bool hold) = 0;
  virtual GameInfo UpdateCurrentState() = 0;
};

}  // namespace s21