#include "raylib5/raylib.h"
#include "snake.h"
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

void gameOverScreen(int vsw, int vsh) {
  const char *text = "Game Over";
  int fontSize = 40;
  int textWidth = MeasureText(text, fontSize);
  DrawText("Game Over", vsw / 2 - textWidth / 2, 150, fontSize, RED);
}

typedef enum { GAME_PLAYING, GAME_OVER } GameState;

GameState gameState = GAME_PLAYING;

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
    if (currentTime - lastMoveTime >= moveInterval &&
        gameState == GAME_PLAYING) {

      Point nextPosition = {
          wrapCoordinate(head->position.x + directions[direction].x, cols),
          wrapCoordinate(head->position.y + directions[direction].y, rows),
      };

      size++;
      if (!addSegment(&head, nextPosition.x, nextPosition.y)) {
        printf("Game over!\n");
        gameState = GAME_OVER;
        size = 0;
        head = createSnake(rand() % cols, rand() % rows);
      }
      if (size > 8) {
        removeTail(head);
      }

      printf("newPosition: %d, %d\n", head->position.x, head->position.y);
      lastMoveTime = currentTime;
    }

    if (gameState == GAME_OVER) {
      gameOverScreen(vsw, vsh);
    }

    if (IsKeyPressed(KEY_R)) {
      head = createSnake(rand() % cols, rand() % rows);
      gameState = GAME_PLAYING;
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

    EndTextureMode();

    BeginDrawing();
    ClearBackground(BLACK);
    float scale = fminf((float)GetScreenWidth() / virtualScreenWidth,
                        (float)GetScreenHeight() / virtualScreenHeight);
    Vector2 position = {(GetScreenWidth() - virtualScreenWidth * scale) * 0.5f,
                        (GetScreenHeight() - virtualScreenHeight * scale) *
                            0.5f};

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
