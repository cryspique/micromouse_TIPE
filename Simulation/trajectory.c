#include "raylib.h"
#include <math.h>
#include <stdlib.h>

int main() {
  const int screenWidth = 1450;
  const int screenHeight = 850;

  // Sliders ----------------------------------

  const int slidermax = 100;

  bool onS1 = false;
  bool onS2 = false;

  int C1 = 0;
  int C2 = 0;

  Vector2 mousePos = {0};

  Rectangle slider1 = {10, 10, 20, slidermax + 4 + (slidermax / 10)};
  Rectangle cursor1 = {slider1.x + 2, slider1.y + slidermax + 2, 16,
                       slidermax / 10};

  Rectangle slider2 = {40, 10, 20, slidermax + 4 + (slidermax / 10)};
  Rectangle cursor2 = {slider2.x + 2, slider2.y + slidermax + 2, 16,
                       slidermax / 10};

  // Micromouse trajectory -------------------------

  Vector2 pos = {725, 740};

  Vector2 size = {80, 100};

  Rectangle micromouse = {pos.x - size.x / 2, pos.y, size.x, size.y};

  float mass = 0.1;
  float d = 0.66;

  float wheel_r = 0.14;

  float speed = 0.;

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
      C1 =
          ((slider2.y + slidermax / 20 + 2) < mousePos.y &&
           mousePos.y < slider1.y + slidermax + slidermax / 20 + 2)
              ? slidermax - (mousePos.y - (slider1.y + slidermax / 20 + 2))
              : (mousePos.y < (slider1.y + slidermax / 20 + 2) ? slidermax : 0);
    } else if (onS2 && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
      C2 =
          ((slider2.y + slidermax / 20 + 2) < mousePos.y &&
           mousePos.y < slider2.y + slidermax + slidermax / 20 + 2)
              ? slidermax - (mousePos.y - (slider2.y + slidermax / 20 + 2))
              : (mousePos.y < (slider2.y + slidermax / 20 + 2) ? slidermax : 0);
    }

    cursor1.y = slider1.y + (slidermax - C1) + 2;
    cursor2.y = slider2.y + (slidermax - C2) + 2;

    // Physics

    speed = wheel_r * sqrtf(abs(C1 + C2) / mass);

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

    ClearBackground(RAYWHITE);

    // Cursors

    DrawRectangleRec(slider1, GRAY);
    DrawRectangleRec(slider2, GRAY);

    DrawRectangleRec(cursor1, BLUE);
    DrawRectangleRec(cursor2, BLUE);

    DrawText(TextFormat("%i", C1), 15,
             slider1.y + slidermax + slidermax / 20 + 24, 10, BLACK);
    DrawText(TextFormat("%i", C2), 45,
             slider2.y + slidermax + slidermax / 20 + 24, 10, BLACK);

    DrawText(TextFormat("v = %.2f m/s", speed), pos.x - 200, pos.y, 20, BLACK);

    // Micromouse

    DrawRectangleRec(micromouse, DARKGREEN);

    EndDrawing();
  }

  CloseWindow();

  return 0;
}