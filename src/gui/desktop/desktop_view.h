#pragma once

#include <QApplication>
#include <QCoreApplication>
#include <QDebug>
#include <QKeyEvent>
#include <QPainter>
#include <QPalette>
#include <QSizePolicy>
#include <QTimer>
#include <QWidget>

#include "base_controller.h"
#include "base_view.h"
#include "constants.h"

namespace s21 {

class DesktopView : public BaseView, public QWidget {
 private:
  BaseController *controller_;

 public:
  DesktopView(BaseController *c) : controller_(c) {
    setWindowFlags(Qt::Widget | Qt::MSWindowsFixedSizeDialogHint);
    setMinimumSize(380, 400);
    setMaximumSize(380, 400);
    setFixedSize(380, 400);
    setWindowTitle("Brick Game");
    startTimer(17);
    QPalette pal = QPalette();
    pal.setColor(QPalette::Window, QColor(223, 223, 223, 255));
    setAutoFillBackground(true);
    setPalette(pal);
    setAttribute(Qt::WA_DeleteOnClose);
    show();
  };
  ~DesktopView() { delete controller_; };

  void MainLoop() override;

 private:
  int key_;
  GameInfo game_info_;

 protected:
  void timerEvent(QTimerEvent *) override;
  void keyPressEvent(QKeyEvent *) override;
  void paintEvent(QPaintEvent *) override;
  UserAction InputToAction(int input);
};

}  // namespace s21