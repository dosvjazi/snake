#include "desktop_view.h"

#include <iostream>

namespace s21 {

void DesktopView::timerEvent(QTimerEvent *e) {
  Q_UNUSED(e);
  controller_->UserInput(InputToAction(key_), true);
  game_info_ = controller_->UpdateCurrentState();
  key_ = 0;
  repaint();
}

UserAction DesktopView::InputToAction(int input) {
  UserAction action = UserAction::kNone;

  if (input == Qt::Key_Up)
    action = UserAction::kUp;
  else if (input == Qt::Key_Down)
    action = UserAction::kDown;
  else if (input == Qt::Key_Left)
    action = UserAction::kLeft;
  else if (input == Qt::Key_Right)
    action = UserAction::kRight;
  else if (input == Qt::Key_Space)
    action = UserAction::kAction;
  else if (input == Qt::Key_P)
    action = UserAction::kPause;
  else if (input == Qt::Key_Escape)
    action = UserAction::kTerminate;
  else if (input == Qt::Key_Return)
    action = UserAction::kStart;

  return action;
}

void DesktopView::keyPressEvent(QKeyEvent *e) { key_ = e->key(); }

void DesktopView::paintEvent(QPaintEvent *e) {
  if (game_info_.pause == (int)RunningInfo::kExit)
    QCoreApplication::exit();
  else {
    Q_UNUSED(e);
    QPainter qp(this);
    QFont font = qp.font();
    font.setPixelSize(20);
    qp.setFont(font);
    qp.setBrush(Qt::white);
    qp.setPen(Qt::white);

    if (game_info_.field != NULL) {
      for (int x = 0; x < 10; x++) {
        for (int y = 0; y < 20; y++) {
          if (game_info_.field[y][x] != 0) {
            qp.setBrush(Qt::white);
            qp.setPen(Qt::white);
          } else {
            qp.setBrush(Qt::black);
            qp.setPen(Qt::black);
          }
          qp.drawRect(x * 20, y * 20, 20, 20);
        }
      }

      qp.setPen(Qt::black);

      qp.drawText(200, 20, 180, 20, Qt::AlignCenter, "LEVEL");
      qp.drawText(200, 45, 180, 20, Qt::AlignCenter,
                  QString::number(game_info_.level + 1));

      qp.drawText(200, 85, 180, 20, Qt::AlignCenter, "SCORE");
      qp.drawText(200, 110, 180, 20, Qt::AlignCenter,
                  QString::number(game_info_.score));

      qp.drawText(200, 150, 180, 20, Qt::AlignCenter, "HIGH SCORE");
      qp.drawText(200, 175, 180, 20, Qt::AlignCenter,
                  QString::number(game_info_.high_score));

      if (game_info_.next != NULL) {
        qp.drawText(200, 215, 180, 20, Qt::AlignCenter, "NEXT");

        for (int y = 0; y < 2; y++) {
          for (int x = 0; x < 4; x++) {
            if (game_info_.next[y][x] != 0) {
              qp.setBrush(Qt::black);
              qp.setPen(Qt::black);
            } else {
              qp.setBrush(QColor(223, 223, 223, 255));
              qp.setPen(QColor(223, 223, 223, 255));
            }
            qp.drawRect(12 * 20 + x * 20, 260 + y * 20, 20, 20);
          }
        }
      }

      qp.setPen(Qt::black);

      char *status = (char *)"PLAYING!";

      if (game_info_.pause == (int)RunningInfo::kGameOver)
        status = (char *)"GAME OVER!";
      else if (game_info_.pause == (int)RunningInfo::kPause)
        status = (char *)"PAUSE!";
      else if (game_info_.pause == (int)RunningInfo::kStart)
        status = (char *)"PRESS START!";
      else if (game_info_.pause == (int)RunningInfo::kGameWon)
        status = (char *)"GAME WON!!";

      qp.drawText(200, 340, 180, 20, Qt::AlignCenter, status);
    }
  }
}

void DesktopView::MainLoop() {}

}  // namespace s21