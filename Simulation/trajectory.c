#include "raylib.h"
#include "raymath.h"
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct {

  Vector2 init_pos;

  Vector2 size;

  float C1;
  float C2;
  float f;
  float wheel_r;
  float d;
  float mass;
  float J;

  float omega1_0;
  float omega2_0;

  float theta;
  float speed;

} micromouse;

float speed(micromouse m, float t) {
  return m.wheel_r * (m.C1 + m.C2) / (2 * m.f) +
         (m.wheel_r / 2) * (m.omega1_0 + m.omega2_0 - (m.C1 + m.C2) / m.f) *
             exp(-m.f * t / m.J);
}

float theta(micromouse m, float t) {
  return m.theta + ((m.wheel_r * (float)(m.C2 - m.C1)) / (m.d * m.f)) * t +
         (m.wheel_r / m.d) *
             ((m.omega2_0 - (m.C2 / m.f)) - (m.omega1_0 - (m.C1 / m.f))) *
             exp(-m.f * t / m.J);
}

int main() {
  const int screenWidth = 1450;
  const int screenHeight = 850;

  micromouse mouse = {0};

  // Sliders ----------------------------------

  const int slidermax = 100;

  bool onS1 = false;
  bool onS2 = false;

  Vector2 mousePos = {0};

  Rectangle slider1 = {10, 10, 20, slidermax + 4 + (slidermax / 10)};
  Rectangle cursor1 = {slider1.x + 2, slider1.y + slidermax + 2, 16,
                       slidermax / 10};

  Rectangle slider2 = {40, 10, 20, slidermax + 4 + (slidermax / 10)};
  Rectangle cursor2 = {slider2.x + 2, slider2.y + slidermax + 2, 16,
                       slidermax / 10};

  // Micromouse trajectory -------------------------

  mouse.init_pos = (Vector2){725, 300};
  mouse.theta = M_PI / 2;

  mouse.size = (Vector2){80, 100};

  Rectangle m = {mouse.init_pos.x - mouse.size.x / 2, mouse.init_pos.y,
                 mouse.size.x, mouse.size.y};

  mouse.mass = 0.1;
  mouse.d = 0.66;

  mouse.f = 0.05;

  mouse.wheel_r = 0.14;

  mouse.speed = 0.;

  mouse.J = 0.001;

  float tmax = 10000.; // Time searched to draw the trajectory curve
  float dt = 0.02f;
  Vector2 pos = mouse.init_pos;

  InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    // Update

    mousePos = GetMousePosition();

    if (CheckCollisionPointRec(mousePos, slider1) && !onS2) {
      onS1 = true;
    } else if (CheckCollisionPointRec(mousePos, slider2) && !onS1) {
      onS2 = true;
    } else if (!IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
      onS1 = false;
      onS2 = false;
    }

    if (onS1 && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
      mouse.C1 =
          0.01 *
          (((slider2.y + slidermax / 20 + 2) < mousePos.y &&
            mousePos.y < slider1.y + slidermax + slidermax / 20 + 2)
               ? slidermax - (mousePos.y - (slider1.y + slidermax / 20 + 2))
               : (mousePos.y < (slider1.y + slidermax / 20 + 2) ? slidermax
                                                                : 0));
    } else if (onS2 && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
      mouse.C2 =
          0.01 *
          (((slider2.y + slidermax / 20 + 2) < mousePos.y &&
            mousePos.y < slider2.y + slidermax + slidermax / 20 + 2)
               ? slidermax - (mousePos.y - (slider2.y + slidermax / 20 + 2))
               : (mousePos.y < (slider2.y + slidermax / 20 + 2) ? slidermax
                                                                : 0));
    }

    cursor1.y = slider1.y + (slidermax - 100 * mouse.C1) + 2;
    cursor2.y = slider2.y + (slidermax - 100 * mouse.C2) + 2;

    // Physics

    mouse.speed = mouse.wheel_r * (mouse.C1 + mouse.C2) / (2 * mouse.f);

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

    ClearBackground(RAYWHITE);

    // Trajectory

    pos = mouse.init_pos;

    for (float t = 0.0f; t < tmax; t += dt) {
      float s = speed(mouse, t);
      float th = theta(mouse, t);

      Vector2 next = {pos.x + s * cosf(th) * dt, pos.y - s * sinf(th) * dt};

      DrawLineEx(pos, next, 3.0f, BLUE);

      pos = next;
    }

    // Cursors

    DrawRectangleRec(slider1, GRAY);
    DrawRectangleRec(slider2, GRAY);

    DrawRectangleRec(cursor1, BLUE);
    DrawRectangleRec(cursor2, BLUE);

    DrawText(TextFormat("%.2f", mouse.C1), 15,
             slider1.y + slidermax + slidermax / 20 + 24, 10, BLACK);
    DrawText(TextFormat("%.2f", mouse.C2), 45,
             slider2.y + slidermax + slidermax / 20 + 24, 10, BLACK);

    DrawText(TextFormat("vlim = %.2f m/s", mouse.speed), mouse.init_pos.x - 200,
             mouse.init_pos.y, 20, BLACK);

    // Micromouse

    // DrawRectangleRec(m, DARKGREEN);

    EndDrawing();
  }

  CloseWindow();

  return 0;
}