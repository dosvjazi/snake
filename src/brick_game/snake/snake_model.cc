#include "snake_model.h"

#include <iostream>

namespace s21 {

void SnakeModel::UserInput(UserAction action_, bool hold) {
  user_action_ = action_;
  hold_ = hold;
}

GameInfo SnakeModel::UpdateCurrentState() {
  if (game_info_.field == NULL) InitGameInfo();
  FiniteStateMachine();
  return game_info_;
}

void SnakeModel::FiniteStateMachine() {
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
      case GameState::kEat:
        Eat();
        break;
      case GameState::kGameOver:
        GameOver();
        break;
      case GameState::kGameWon:
        GameWon();
        break;
    }
  }

  DrawSnake();

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

void SnakeModel::InitGameInfo() {
  srand(time(NULL));

  game_info_.field = (int **)calloc(FIELD_Y_COUNT, sizeof(int *));
  for (int i = FIELD_Y_START; i < FIELD_Y_COUNT; i++)
    game_info_.field[i] = (int *)calloc(FIELD_X_COUNT, sizeof(int));

  game_info_.score = 0;
  game_info_.high_score = 0;
  game_info_.level = 0;
  game_info_.speed = speed_list[0];
  game_info_.pause = (int)RunningInfo::kStart;

  game_state_ = GameState::kGameStart;

  snake_.y = 8;
  snake_.x = 5;
  snake_.y_dir = -1;
  snake_.x_dir = 0;
  for (int i = 0; i < 199; i++) snake_.next[i] = {.y = -1, .x = -1};
  snake_.next[0] = {.y = 9, .x = 5};
  snake_.next[1] = {.y = 10, .x = 5};
  snake_.next[2] = {.y = 11, .x = 5};
  apple_.y = 8;
  apple_.x = 5;

  timer_ = 0;

  LoadHighScore();
}

void SnakeModel::LoadHighScore() {
  FILE *file = fopen("snake_high_score.txt", "r");
  if (file != NULL) {
    if (fscanf(file, "%d", &game_info_.high_score) != 1)
      game_info_.high_score = 0;
    fclose(file);
  }
}

void SnakeModel::SaveHighScore() {
  FILE *file = fopen("snake_high_score.txt", "w");
  fprintf(file, "%d", game_info_.high_score);
  fclose(file);
}

void SnakeModel::GameStart() {
  if (user_action_ == UserAction::kStart) {
    game_info_.pause = (int)RunningInfo::kPlay;
    game_state_ = GameState::kSpawn;
  }
}

void SnakeModel::Spawn() {
  GenerateApple();
  game_state_ = GameState::kMove;
}

void SnakeModel::Moving() {
  if (user_action_ == UserAction::kUp && snake_.y_dir == 0) {
    game_state_ = GameState::kShift;
    timer_ = 0;
    snake_.y_dir = -1;
    snake_.x_dir = 0;
  }
  if (user_action_ == UserAction::kDown && snake_.y_dir == 0) {
    game_state_ = GameState::kShift;
    timer_ = 0;
    snake_.y_dir = 1;
    snake_.x_dir = 0;
  }
  if (user_action_ == UserAction::kLeft && snake_.x_dir == 0) {
    game_state_ = GameState::kShift;
    timer_ = 0;
    snake_.y_dir = 0;
    snake_.x_dir = -1;
  }
  if (user_action_ == UserAction::kRight && snake_.x_dir == 0) {
    game_state_ = GameState::kShift;
    timer_ = 0;
    snake_.y_dir = 0;
    snake_.x_dir = 1;
  }

  timer_++;

  if (timer_ >= game_info_.speed || user_action_ == UserAction::kAction) {
    game_state_ = GameState::kShift;
    timer_ = 0;
  }
}

void SnakeModel::Shifting() {
  bool can_shift = true;

  if (snake_.y + snake_.y_dir == -1 || snake_.y + snake_.y_dir == 20 ||
      snake_.x + snake_.x_dir == -1 || snake_.x + snake_.x_dir == 10) {
    can_shift = false;
  } else {
    int next_y = snake_.y;
    int next_x = snake_.x;

    snake_.y += snake_.y_dir;
    snake_.x += snake_.x_dir;

    for (int i = 0; i < 199; i++) {
      if (snake_.next[i].y != -1) {
        int last_y = snake_.next[i].y;
        int last_x = snake_.next[i].x;

        snake_.next[i].y = next_y;
        snake_.next[i].x = next_x;

        next_y = last_y;
        next_x = last_x;
      }
    }

    for (int i = 0; i < 199; i++) {
      if (snake_.next[i].y == snake_.y && snake_.next[i].x == snake_.x)
        can_shift = false;
    }
  }

  if (can_shift) {
    if (snake_.y == apple_.y && snake_.x == apple_.x) {
      game_state_ = GameState::kEat;
    } else {
      game_state_ = GameState::kMove;
    }
  } else {
    game_state_ = GameState::kGameOver;
  }
}

void SnakeModel::Eat() {
  bool eaten = false;

  for (int i = 0; i < 198; i++) {
    if (snake_.next[i + 1].y == -1 && snake_.next[i].y != -1 && !eaten) {
      snake_.next[i + 1].y = snake_.next[i].y;
      snake_.next[i + 1].x = snake_.next[i].x;
      eaten = true;
    }
  }

  game_info_.score += 1;
  game_info_.level = game_info_.score / 5;
  game_info_.speed = speed_list[game_info_.level];

  game_state_ = GameState::kSpawn;
}

void SnakeModel::GameOver() {
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

void SnakeModel::GameWon() {
  if (game_info_.high_score < game_info_.score) {
    game_info_.high_score = game_info_.score;
    SaveHighScore();
  }

  game_info_.pause = (int)RunningInfo::kGameWon;

  if (user_action_ == UserAction::kStart) {
    FreeMemory();
    InitGameInfo();
  }
}

void SnakeModel::Pause() {
  if (user_action_ == UserAction::kPause) {
    if (game_info_.pause == (int)RunningInfo::kPlay) {
      game_info_.pause = (int)RunningInfo::kPause;
    } else if (game_info_.pause == (int)RunningInfo::kPause) {
      game_info_.pause = (int)RunningInfo::kPlay;
    }
  }
}

void SnakeModel::DrawSnake() {
  for (int y = FIELD_Y_START; y < FIELD_Y_COUNT; y++) {
    for (int x = FIELD_X_START; x < FIELD_X_COUNT; x++) {
      game_info_.field[y][x] = EMPTY;
    }
  }
  for (int y = FIELD_Y_START; y < FIELD_Y_COUNT; y++) {
    for (int x = FIELD_X_START; x < FIELD_X_COUNT; x++) {
      if (snake_.y == y && snake_.x == x) game_info_.field[y][x] = SNAKE;
      for (int i = 0; i < 199; i++) {
        if (snake_.next[i].y == y && snake_.next[i].x == x)
          game_info_.field[y][x] = SNAKE;
      }
    }
  }
  game_info_.field[apple_.y][apple_.x] = APPLE;
}

void SnakeModel::GenerateApple() {
  DrawSnake();

  float r = (float)rand() / RAND_MAX;
  int i = (int)(r * FIELD_X_COUNT * FIELD_Y_COUNT);

  if (game_info_.field[i / FIELD_X_COUNT][i % FIELD_X_COUNT] == 0) {
    apple_.y = i / FIELD_X_COUNT;
    apple_.x = i % FIELD_X_COUNT;
  } else {
    GenerateApple();
  }
}

void SnakeModel::CheckForGameWon() {
  bool is_game_won = true;

  for (int y = FIELD_Y_START; y < FIELD_Y_COUNT; y++) {
    for (int x = FIELD_X_START; x < FIELD_X_COUNT; x++) {
      if (game_info_.field[y][x] != 1) is_game_won = false;
    }
  }

  game_state_ = is_game_won ? GameState::kGameWon : GameState::kSpawn;
}

void SnakeModel::FreeMemory() {
  for (int i = FIELD_Y_START; i < FIELD_Y_COUNT; i++) free(game_info_.field[i]);
  free(game_info_.field);
}

}  // namespace s21