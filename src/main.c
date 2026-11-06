#include <raylib.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

typedef struct Circle {
  int x, y;
  float r;
} Circle;

static void ResetCirclePosition(Circle *circle) {
  circle->x = WINDOW_WIDTH / 2;
  circle->y = 100;
}

static void ResetTrayPosition(Rectangle *tray) {
  tray->x = WINDOW_WIDTH / 2 - tray->width / 2;
  tray->y = WINDOW_HEIGHT - tray->height;
}

int main(void) {
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Pong");
  SetTargetFPS(100);

  // Ball
  Circle circle;
  circle.r = 30;
  ResetCirclePosition(&circle);

  /*
   * The speed is related to both
   * ball and tray
   */
  Vector2 velocity = {GetRandomValue(0, 1) ? 1.0 : -1.0, 1.0};

  // Tray
  Rectangle tray;
  tray.height = 20;
  tray.width = 2 * circle.r;
  ResetTrayPosition(&tray);

  // Timer
  double timer = GetTime(), dead = 0.0;
  bool over = false;
  float factor = 1.0;

  while (!WindowShouldClose()) {
    float dt = GetFrameTime();
    int speed = 300 * factor;

    // --- Update ---
    circle.x += speed * velocity.x * dt;
    circle.y += speed * velocity.y * dt;

    // Collision
    if (CheckCollisionCircleRec((Vector2){circle.x, circle.y}, circle.r,
                                tray)) {
      velocity.y *= -1;
    }
    // In case of sticking in the wall, we have to
    // intensionally move away the circle
    if (circle.x + circle.r >= WINDOW_WIDTH) {
      circle.x -= 5;
      velocity.x *= -1;
    }
    if (circle.x - circle.r <= 0) {
      circle.x += 5;
      velocity.x *= -1;
    }
    if (circle.y - circle.r <= 0) {
      circle.y += 5;
      velocity.y *= -1;
    }

    // Control
    if (IsKeyDown(KEY_A) && tray.x >= 0) {
      tray.x -= speed * dt;
    }
    if (IsKeyDown(KEY_D) && tray.x + tray.width <= WINDOW_WIDTH) {
      tray.x += speed * dt;
    }
    if (IsKeyPressed(KEY_W)) {
      factor += 0.5;
    }
    if (IsKeyPressed(KEY_S) && factor > 1.0) {
      factor -= 0.5;
    }

    // Over
    if (!over && circle.y - circle.r >= WINDOW_HEIGHT) {
      over = true;
      dead = GetTime();
    }

    if (over) {
      if (IsKeyPressed(KEY_SPACE)) {
        timer = GetTime();
        factor = 1.0;
        over = false;
        ResetCirclePosition(&circle);
        ResetTrayPosition(&tray);
      }
    }

    // --- Draw ---
    BeginDrawing();

    if (!over) {
      DrawCircle(circle.x, circle.y, circle.r, PINK);
      DrawRectangleRec(tray, GREEN);

      DrawText(TextFormat("%.1fs SPEED:%.1fx", GetTime() - timer, factor), 50,
               50, 20, BLACK);
    } else {
      DrawText(TextFormat("YOU SURVIVED FOR %.1lfs\nPRESS SPACE TO RETRY",
                          dead - timer),
               100, 100, 30, BLACK);
    }

    ClearBackground(GRAY);
    EndDrawing();
  }

  CloseWindow();
  return 0;
}