#include "tetris_model.h"

#include <iostream>

namespace s21 {

void TetrisModel::UserInput(UserAction action_, bool hold) {
  user_action_ = action_;
  hold_ = hold;
}

GameInfo TetrisModel::UpdateCurrentState() {
  if (field_ == NULL) InitGameInfo();
  FiniteStateMachine();
  return game_info_;
}

void TetrisModel::FiniteStateMachine() {
  if (game_info_.pause != (int)RunningInfo::kPause) {
    switch (game_state_) {
      case GameState::kGameStart:
        GameStart();
        break;
      case GameState::kSpawn:
        Spawn();
        break;
      case GameState::kMove:
        Moving();
        break;
      case GameState::kShift:
        Shifting();
        break;
      case GameState::kAttach:
        Attach();
        break;
      case GameState::kGameOver:
        GameOver();
        break;
    }
  }

  ConvertField();

  if (user_action_ == UserAction::kPause ||
      game_info_.pause == (int)RunningInfo::kPause) {
    Pause();
  }

  if (user_action_ == UserAction::kTerminate) {
    FreeMemory();
    game_info_.pause = (int)RunningInfo::kExit;
  }

  user_action_ = UserAction::kNone;
}

void TetrisModel::InitGameInfo() {
  srand(time(NULL));

  field_ = (int **)calloc(TOTAL_Y_COUNT, sizeof(int *));
  for (int i = TOTAL_Y_START; i < TOTAL_Y_COUNT; i++)
    field_[i] = (int *)calloc(FIELD_X_COUNT, sizeof(int));

  game_info_.field = (int **)calloc(FIELD_Y_COUNT, sizeof(int *));
  for (int i = FIELD_Y_START; i < FIELD_Y_COUNT; i++)
    game_info_.field[i] = (int *)calloc(FIELD_X_COUNT, sizeof(int));

  game_info_.next = (int **)calloc(TETRA_Y_COUNT, sizeof(int *));
  for (int i = TETRA_A_START; i < TETRA_Y_COUNT; i++)
    game_info_.next[i] = (int *)calloc(TETRA_X_COUNT, sizeof(int));

  game_info_.score = 0;
  game_info_.high_score = 0;
  game_info_.level = 0;
  game_info_.speed = speed_list[0];
  game_info_.pause = (int)RunningInfo::kStart;

  game_state_ = GameState::kGameStart;

  timer_ = 0;

  LoadHighScore();
}

void TetrisModel::LoadHighScore() {
  FILE *file = fopen("tetris_high_score.txt", "r");
  if (file != NULL) {
    if (fscanf(file, "%d", &game_info_.high_score) != 1)
      game_info_.high_score = 0;
    fclose(file);
  }
}

void TetrisModel::SaveHighScore() {
  FILE *file = fopen("tetris_high_score.txt", "w");
  fprintf(file, "%d", game_info_.high_score);
  fclose(file);
}

void TetrisModel::GameStart() {
  if (user_action_ == UserAction::kStart) {
    game_info_.pause = (int)RunningInfo::kPlay;
    GenerateTetromine();
    game_state_ = GameState::kSpawn;
  }
}

void TetrisModel::Spawn() {
  PlaceTetromine();
  GenerateTetromine();
  game_state_ = GameState::kMove;
}

void TetrisModel::Moving() {
  if (user_action_ == UserAction::kRight) MoveRight();
  if (user_action_ == UserAction::kLeft) MoveLeft();
  if (user_action_ == UserAction::kUp) Rotate();

  timer_++;

  if (user_action_ == UserAction::kAction) {
    while (game_state_ != GameState::kAttach) Shifting();
    timer_ = 0;
  }

  if (timer_ >= game_info_.speed || user_action_ == UserAction::kDown) {
    game_state_ = GameState::kShift;
    timer_ = 0;
  }
}

void TetrisModel::Shifting() {
  bool can_shift = true;

  for (int x = FIELD_X_START; x < FIELD_X_COUNT; x++) {
    for (int y = TOTAL_Y_COUNT - 2; y >= TOTAL_Y_START; y--) {
      if (field_[y][x] >= MOVING && field_[y + 1][x] == 1) can_shift = false;
    }
    for (int y = TOTAL_Y_COUNT - 1; y >= TOTAL_Y_START + 1; y--) {
      if (field_[y][x] >= MOVING && y == TOTAL_Y_COUNT - 1) can_shift = false;
    }
  }

  if (can_shift) {
    for (int y = TOTAL_Y_COUNT - 1; y > TOTAL_Y_START; y--) {
      for (int x = FIELD_X_START; x < FIELD_X_COUNT; x++) {
        if (field_[y - 1][x] >= MOVING) {
          field_[y][x] = field_[y - 1][x];
          field_[y - 1][x] = EMPTY;
        }
      }
    }
    game_state_ = GameState::kMove;
  } else {
    game_state_ = GameState::kAttach;
  }
}

void TetrisModel::Attach() {
  for (int x = FIELD_X_START; x < FIELD_X_COUNT; x++) {
    for (int y = TOTAL_Y_START; y < TOTAL_Y_COUNT; y++) {
      if (field_[y][x] >= MOVING) field_[y][x] = 1;
    }
  }

  CheckForFullRows();
  CheckForGameOver();
}

void TetrisModel::GameOver() {
  if (game_info_.high_score < game_info_.score) {
    game_info_.high_score = game_info_.score;
    SaveHighScore();
  }

  game_info_.pause = (int)RunningInfo::kGameOver;

  if (user_action_ == UserAction::kStart) {
    FreeMemory();
    InitGameInfo();
  }
}

void TetrisModel::Pause() {
  if (user_action_ == UserAction::kPause) {
    if (game_info_.pause == (int)RunningInfo::kPlay) {
      game_info_.pause = (int)RunningInfo::kPause;
    } else if (game_info_.pause == (int)RunningInfo::kPause) {
      game_info_.pause = (int)RunningInfo::kPlay;
    }
  }
}

void TetrisModel::GenerateTetromine() {
  float r = (float)rand() / RAND_MAX;
  int i = (int)(r * 7);

  for (int y = TETRA_A_START; y < TETRA_Y_COUNT; y++) {
    for (int x = TETRA_A_START; x < TETRA_X_COUNT; x++) {
      game_info_.next[y][x] = tetromine_list[i][y][x];
    }
  }
}

void TetrisModel::PlaceTetromine() {
  for (int y = TETRA_A_START; y < TETRA_Y_COUNT; y++) {
    for (int x = TETRA_A_START; x < TETRA_X_COUNT; x++) {
      field_[y + TETRA_Y_OFFSET][x + TETRA_X_OFFSET] = game_info_.next[y][x];
    }
  }
}

void TetrisModel::MoveRight() {
  bool can_move = true;

  for (int x = FIELD_X_START; x < FIELD_X_COUNT; x++) {
    for (int y = TOTAL_Y_START; y < TOTAL_Y_COUNT; y++) {
      if (field_[y][x] >= MOVING && x == FIELD_X_COUNT - 1) {
        can_move = false;
      }
      if (field_[y][x] >= MOVING && x < FIELD_X_COUNT - 1 &&
          field_[y][x + 1] == 1) {
        can_move = false;
      }
    }
  }

  if (can_move) {
    for (int x = FIELD_X_COUNT - 1; x > FIELD_X_START; x--) {
      for (int y = TOTAL_Y_START; y < TOTAL_Y_COUNT; y++) {
        if (field_[y][x - 1] >= MOVING) {
          field_[y][x] = field_[y][x - 1];
          field_[y][x - 1] = EMPTY;
        }
      }
    }
  }
}

void TetrisModel::MoveLeft() {
  bool can_move = true;

  for (int x = FIELD_X_START; x < FIELD_X_COUNT; x++) {
    for (int y = TOTAL_Y_START; y < TOTAL_Y_COUNT; y++) {
      if (field_[y][x] >= MOVING && x == FIELD_X_START) {
        can_move = false;
      }
      if (field_[y][x] >= MOVING && x > FIELD_X_START &&
          field_[y][x - 1] == 1) {
        can_move = false;
      }
    }
  }

  if (can_move) {
    for (int x = FIELD_X_START; x < FIELD_X_COUNT - 1; x++) {
      for (int y = TOTAL_Y_START; y < TOTAL_Y_COUNT; y++) {
        if (field_[y][x + 1] >= MOVING) {
          field_[y][x] = field_[y][x + 1];
          field_[y][x + 1] = EMPTY;
        }
      }
    }
  }
}

void TetrisModel::Rotate() {
  int x_center = -1;
  int y_center = -1;

  for (int y = TEMP_FIELD_Y_START; y < TOTAL_Y_COUNT; y++) {
    for (int x = FIELD_X_START; x < FIELD_X_COUNT; x++) {
      if (field_[y][x] == CENTER) {
        x_center = x;
        y_center = y;
      }
    }
  }

  int temp[5][5];

  for (int x = 0; x < 5; x++) {
    for (int y = 0; y < 5; y++) {
      if (CheckInside(y, x, y_center, x_center) &&
          field_[y + y_center - 2][x + x_center - 2] >= MOVING) {
        temp[x][4 - y] = field_[y + y_center - 2][x + x_center - 2];
      } else {
        temp[x][4 - y] = 0;
      }
    }
  }

  bool can_rotate = true;

  for (int x = 0; x < 5; x++) {
    for (int y = 0; y < 5; y++) {
      if (temp[y][x] >= MOVING && CheckOutside(y, x, y_center, x_center))
        can_rotate = false;
      if (CheckInside(y, x, y_center, x_center) && temp[y][x] >= MOVING &&
          field_[y + y_center - 2][x + x_center - 2] == PLACED) {
        can_rotate = false;
      }
    }
  }

  if (can_rotate) {
    for (int x = 0; x < 5; x++) {
      for (int y = 0; y < 5; y++) {
        if (CheckInside(y, x, y_center, x_center) &&
            field_[y + y_center - 2][x + x_center - 2] != PLACED) {
          field_[y + y_center - 2][x + x_center - 2] = temp[y][x];
        }
      }
    }
  }
}

bool TetrisModel::CheckInside(int y, int x, int y_center, int x_center) {
  return y + y_center - 2 >= TOTAL_Y_START &&
         y + y_center - 2 < TOTAL_Y_COUNT &&
         x + x_center - 2 >= FIELD_X_START && x + x_center - 2 < FIELD_X_COUNT;
}
bool TetrisModel::CheckOutside(int y, int x, int y_center, int x_center) {
  return y + y_center - 2 < TOTAL_Y_START ||
         y + y_center - 2 >= TOTAL_Y_COUNT ||
         x + x_center - 2 < FIELD_X_START || x + x_center - 2 >= FIELD_X_COUNT;
}

void TetrisModel::CheckForGameOver() {
  bool is_game_over = false;

  for (int x = FIELD_X_START; x < FIELD_X_COUNT; x++) {
    if (field_[TEMP_FIELD_Y_START][x] == 1) is_game_over = true;
  }

  game_state_ = is_game_over ? GameState::kGameOver : GameState::kSpawn;
}

void TetrisModel::CheckForFullRows() {
  int full_rows = 0;

  for (int t = 0; t < 4; t++) {
    for (int y = TOTAL_Y_COUNT - 1; y > TOTAL_Y_START; y--) {
      bool full = true;

      for (int x = FIELD_X_START; x < FIELD_X_COUNT; x++) {
        if (field_[y][x] == 0) full = false;
      }

      if (full) {
        full_rows++;

        DestroyFullRows(y);
      }
    }
  }

  int score = 0;

  for (int i = 1; i <= 4; i++) {
    if (full_rows >= i) score = score * 2 + 100;
  }

  game_info_.score = game_info_.score + score;

  int level = game_info_.score / 600 > 10 ? 10 : game_info_.score / 600;

  game_info_.level = level;
  game_info_.speed = speed_list[level];
}

void TetrisModel::DestroyFullRows(int start) {
  for (int j = start; j > 0; j--) {
    for (int i = FIELD_X_START; i < FIELD_X_COUNT; i++) {
      field_[j][i] = field_[j - 1][i];
    }
  }
}

void TetrisModel::ConvertField() {
  for (int y = FIELD_Y_START; y < FIELD_Y_COUNT; y++) {
    for (int x = FIELD_X_START; x < FIELD_X_COUNT; x++) {
      game_info_.field[y][x] = field_[y + 2][x];
    }
  }
}

void TetrisModel::FreeMemory() {
  for (int i = TOTAL_Y_START; i < TOTAL_Y_COUNT; i++) free(field_[i]);
  free(field_);

  for (int i = FIELD_Y_START; i < FIELD_Y_COUNT; i++) free(game_info_.field[i]);
  free(game_info_.field);

  for (int i = TETRA_A_START; i < TETRA_Y_COUNT; i++) free(game_info_.next[i]);
  free(game_info_.next);
}

}  // namespace s21