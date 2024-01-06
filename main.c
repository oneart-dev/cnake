#include "raylib5/raylib.h"
#include "raylib5/raymath.h"
#include "snake.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void DrawTopStatusBar(int vsw, int vsh) {
  const int lrPadding = 10;
  int tbPadding = 5;

  const char *text = "press [ESC] to exit";
  int fontSize = 20;
  int textWidth = MeasureText(text, fontSize);

  DrawFPS(vsw - 100 - lrPadding, tbPadding);

  tbPadding += fontSize + 5;
  DrawText("Press [ESC] to exit", vsw - textWidth - lrPadding, tbPadding, 20,
           WHITE);

  const char *text2 = "Press [R] to restart";
  int fontSize2 = 20;
  int textWidth2 = MeasureText(text2, fontSize2);
  DrawText(text2, lrPadding, tbPadding, fontSize2, WHITE);
}

int main(void) {
  const int virtualScreenWidth = 1600;
  const int vsw = virtualScreenWidth;
  const int virtualScreenHeight = 900;
  const int vsh = virtualScreenHeight;

  const float targetAspectRatio = 16.0f / 9.0f;
  const int rows = 15;
  const int cols = 15;
  const int cellSize = 30;
  const int padding = 100;

  SnakeSegment *head = createSnake(rand() % cols, rand() % rows);
  Direction direction = RIGHT;

  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  SetTargetFPS(60);
  InitWindow(0, 0, "Suck it up, buttercup!");

  RenderTexture2D target =
      LoadRenderTexture(virtualScreenWidth, virtualScreenHeight);

  srand(time(NULL));

  // Triangle triangles[rows * cols];
  // triangles[0] = randomTriangle(vsw, vsh);
  // int triangleCount = 1;

  float moveInterval = 0.5f; // Move every 0.5 seconds
  double lastMoveTime = GetTime();
  int size = 0;

  while (!WindowShouldClose()) {

    BeginTextureMode(target);
    ClearBackground(DARKBROWN);
    DrawTopStatusBar(vsw, vsh);

    if (IsKeyPressed(KEY_UP)) {
      direction = UP;
    } else if (IsKeyPressed(KEY_DOWN)) {
      direction = DOWN;
    } else if (IsKeyPressed(KEY_LEFT)) {
      direction = LEFT;
    } else if (IsKeyPressed(KEY_RIGHT)) {
      direction = RIGHT;
    }

    double currentTime = GetTime();
    if (currentTime - lastMoveTime >= moveInterval) {

      printf("next move\n");
      printf("currentPosition: %d, %d\n", head->position.x, head->position.y);

      Point nextPosition = {
          wrapCoordinate(head->position.x + directions[direction].x, cols),
          wrapCoordinate(head->position.y + directions[direction].y, rows),
      };

      printf("nextPosition: %d, %d\n", nextPosition.x, nextPosition.y);

      size++;
      if (!addSegment(&head, nextPosition.x, nextPosition.y)) {
        printf("Game over!\n");
        size = 0;
        head = createSnake(rand() % cols, rand() % rows);
      }
      if (size > 8) {
        removeTail(head);
      }

      printf("newPosition: %d, %d\n", head->position.x, head->position.y);
      lastMoveTime = currentTime;
    }

    for (int i = 0; i < rows; ++i) {
      for (int j = 0; j < cols; ++j) {
        int x = j * cellSize + padding;
        int y = i * cellSize + padding;
        DrawRectangleLines(x, y, cellSize, cellSize, LIGHTGRAY);
      }
    }

    for (SnakeSegment *current = head; current != NULL;
         current = current->next) {
      int x = current->position.x * cellSize + padding;
      int y = current->position.y * cellSize + padding;
      DrawRectangle(x, y, cellSize, cellSize, GREEN);
    }

    if (IsKeyPressed(KEY_R)) {
      head = createSnake(rand() % cols, rand() % rows);
    }

    // if (IsKeyPressed(KEY_R)) {
    //   triangles[triangleCount] = randomTriangle(vsw, vsh);
    //   triangleCount++;
    //   printf("triangleCount: %d\n", triangleCount);
    // }

    // for (int i = 0; i < triangleCount; i++) {
    //   Triangle it = triangles[i];
    //   DrawTriangle(it.v1, it.v2, it.v3, it.color);
    // }
    EndTextureMode();

    BeginDrawing();
    ClearBackground(BLACK);
    float scale = fminf((float)GetScreenWidth() / virtualScreenWidth,
                        (float)GetScreenHeight() / virtualScreenHeight);
    Vector2 position = {(GetScreenWidth() - virtualScreenWidth * scale) * 0.5f,
                        (GetScreenHeight() - virtualScreenHeight * scale) *
                            0.5f};

    // Draw the scaled virtual screen
    // DrawTextureEx(target.texture, position, 0.0f, scale, WHITE);

    Rectangle sourceRec = {0, 0, (float)target.texture.width,
                           (float)-target.texture.height};
    Rectangle destRec = {position.x, position.y, target.texture.width * scale,
                         target.texture.height * scale};
    Vector2 origin = {0, 0};
    DrawTexturePro(target.texture, sourceRec, destRec, origin, 0.0f, WHITE);

    EndDrawing();

    if (IsKeyPressed(KEY_ESCAPE)) {
      break;
    }
  }

  UnloadRenderTexture(target);
  CloseWindow();

  return 0;
}
