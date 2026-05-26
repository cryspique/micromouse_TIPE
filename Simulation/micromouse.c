#include "raylib.h"
#include "raymath.h"
#include <math.h>

/*
8x10 cm
Ecart roues : 6.6 cm
Diamètre des roues : 27.5 cm
Masse : 100 g
*/

typedef struct {
  Vector2 position;

  Vector2 velocity;
  Vector2 acceleration;

  float angle;

  float angularVelocity;
  float angularAcceleration;

  float mass;

  float wheelDist;

  float leftMotor; // Les valeurs dans les arduino pour bouger les roues
  float rightMotor;

  Vector2 size;

} Mouse;

int main(void) {

  Vector2 window = {1200, 800};

  InitWindow(window.x, window.y, "Micromouse Physics");

  SetTargetFPS(60);

  Mouse mouse = {0};

  mouse.position = (Vector2){600, 400};
  mouse.mass = 1.0f;
  mouse.wheelDist = 66.0f;

  mouse.size = (Vector2){100, 80};

  const float maxSpeed = 150.0f;

  while (!WindowShouldClose()) {
    float dt = GetFrameTime();

    mouse.leftMotor = 0;
    mouse.rightMotor = 0;

    // Avancer
    if (IsKeyDown(KEY_W)) {
      mouse.leftMotor += maxSpeed;
      mouse.rightMotor += maxSpeed;
    }

    // Reculer
    if (IsKeyDown(KEY_S)) {
      mouse.leftMotor -= maxSpeed;
      mouse.rightMotor -= maxSpeed;
    }

    // Gauche
    if (IsKeyDown(KEY_A)) {
      mouse.leftMotor += maxSpeed * 0.5f;
      mouse.rightMotor -= maxSpeed * 0.5f;
    }

    // Droite
    if (IsKeyDown(KEY_D)) {
      mouse.leftMotor -= maxSpeed * 0.5f;
      mouse.rightMotor += maxSpeed * 0.5f;
    }

    // Calcul du mouvement ----------------------------------

    float vL = mouse.leftMotor;
    float vR = mouse.rightMotor;

    float v = (vR + vL) / 2.0f; // Modèle cinématique différentiel
    float omega = (vR - vL) / mouse.wheelDist;

    mouse.angle += omega * dt;

    Vector2 forward = {cosf(mouse.angle), sinf(mouse.angle)};

    mouse.position.x += forward.x * v * dt;
    mouse.position.y += forward.y * v * dt;

    // Dessin ------------------------------------------------

    BeginDrawing();

    ClearBackground(WHITE);

    // Corps
    Rectangle body = {mouse.position.x, mouse.position.y, mouse.size.x,
                      mouse.size.y};

    DrawRectanglePro(body, (Vector2){mouse.size.x / 2, mouse.size.y / 2},
                     mouse.angle * RAD2DEG, GRAY);

    // Flèche pour la vitesse
    float speed = fabsf(v);
    float arrowlength = 10.0f + speed * 0.05f;

    Vector2 speedarrow = {mouse.position.x + cosf(mouse.angle) * arrowlength,
                          mouse.position.y + sinf(mouse.angle) * arrowlength};

    DrawLineEx(mouse.position, speedarrow, 4, RED);

    DrawText(TextFormat("Speed: %.2f", fabsf(v)), 20, 20, 20, BLACK);

    EndDrawing();
  }

  CloseWindow();

  return 0;
}