#pragma once

namespace s21 {

class BaseView {
 public:
  virtual ~BaseView() {};

  virtual void MainLoop() = 0;
};

}  // namespace s21