/*
 * Pong, a simple ping-pong game built with Raylib.
 *
 * Xemb (emsendrecv@gmail.com)
 * MIT License 2026
 */

#include <raylib.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

typedef struct Circle {
  int x, y;
  float r;
} Circle;

typedef struct Game {
  // Ball & Tray
  Circle circle;
  Vector2 velocity;
  Rectangle tray;

  // Timer
  double timer;
  double dead;
  double mainly_duration;
  double toggle_speed_diff;

  float factor;
  float mainly_factor;

  bool over;
} Game;

static void ResetCirclePosition(Circle *circle) {
  circle->x = WINDOW_WIDTH / 2;
  circle->y = 100;
}

static void ResetTrayPosition(Rectangle *tray) {
  tray->x = WINDOW_WIDTH / 2 - tray->width / 2;
  tray->y = WINDOW_HEIGHT - tray->height;
}

static void CheckMajorSpeed(Game *game) {
  game->toggle_speed_diff = GetTime() - game->toggle_speed_diff;
  if (game->toggle_speed_diff > game->mainly_duration) {
    game->mainly_duration = game->toggle_speed_diff;
    game->mainly_factor = game->factor;
  }
}

static void UpdateGame(Game *game) {
  float dt = GetFrameTime();
  int speed = 300 * game->factor;

  game->circle.x += speed * game->velocity.x * dt;
  game->circle.y += speed * game->velocity.y * dt;

  // Collision
  if (CheckCollisionCircleRec((Vector2){game->circle.x, game->circle.y},
                              game->circle.r, game->tray)) {
    game->velocity.y *= -1;
  }
  // In case of sticking in the wall, we have to
  // intensionally move away the circle
  if (game->circle.x + game->circle.r >= WINDOW_WIDTH) {
    game->circle.x -= 5;
    game->velocity.x *= -1;
  }
  if (game->circle.x - game->circle.r <= 0) {
    game->circle.x += 5;
    game->velocity.x *= -1;
  }
  if (game->circle.y - game->circle.r <= 0) {
    game->circle.y += 5;
    game->velocity.y *= -1;
  }

  // Control
  if (IsKeyDown(KEY_A) && game->tray.x >= 0) {
    game->tray.x -= speed * dt;
  }
  if (IsKeyDown(KEY_D) && game->tray.x + game->tray.width <= WINDOW_WIDTH) {
    game->tray.x += speed * dt;
  }

  // We collect statistics for major speed factor
  // and show it after game-over
  if (IsKeyPressed(KEY_W)) {
    CheckMajorSpeed(game);
    game->factor += 0.5;
  }
  if (IsKeyPressed(KEY_S) && game->factor > 1.0) {
    CheckMajorSpeed(game);
    game->factor -= 0.5;
  }

  // Over
  if (!game->over && game->circle.y - game->circle.r >= WINDOW_HEIGHT) {
    game->over = true;
    game->dead = GetTime();
    CheckMajorSpeed(game);
  }

  if (game->over) {
    if (IsKeyPressed(KEY_SPACE)) {
      game->timer = GetTime();
      game->factor = 1.0;
      game->over = false;
      ResetCirclePosition(&game->circle);
      ResetTrayPosition(&game->tray);
    }
  }
}

static void DrawGame(Game *game) {
  if (!game->over) {
    DrawCircle(game->circle.x, game->circle.y, game->circle.r, PINK);
    DrawRectangleRec(game->tray, GREEN);

    DrawText(
        TextFormat("%.1fs SPEED:%.1fx", GetTime() - game->timer, game->factor),
        50, 50, 20, BLACK);
  } else {
    DrawText(
        TextFormat(
            "YOU SURVIVED FOR %.1lfs MAINLY AT %.1fx\nPRESS SPACE TO RETRY",
            game->dead - game->timer, game->mainly_factor),
        100, 100, 30, BLACK);
  }
  DrawText(TextFormat("[DEBUG] LAST DIFF:%.1lf", game->toggle_speed_diff), 500,
           20, 20, BLACK);
}

int main(void) {
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Pong");
  SetTargetFPS(100);

  Game game;

  game.circle.r = 30;
  ResetCirclePosition(&game.circle);

  /*
   * The speed is related to both
   * ball and tray
   */
  game.velocity.x = GetRandomValue(0, 1) ? 1.0 : -1.0;
  game.velocity.y = 1.0;

  // Tray
  game.tray.height = 20;
  game.tray.width = 2 * game.circle.r;
  ResetTrayPosition(&game.tray);

  // Timer
  game.timer = GetTime();
  game.dead = 0.0;
  game.mainly_duration = 0.0;
  game.toggle_speed_diff = GetTime();

  game.over = false;
  game.factor = 1.0;
  game.mainly_factor = game.factor;

  while (!WindowShouldClose()) {
    // --- Update ---
    UpdateGame(&game);

    // --- Draw ---
    BeginDrawing();
    ClearBackground(GRAY);
    DrawGame(&game);
    EndDrawing();
  }

  CloseWindow();
  return 0;
}