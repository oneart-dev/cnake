#ifndef SNAKE_H
#define SNAKE_H

#include "raylib5/raylib.h"

typedef struct Point {
  int x;
  int y;
} Point;

typedef struct SnakeSegment {
  Point position;
  struct SnakeSegment *next;
} SnakeSegment;

SnakeSegment *createSnake(int initialX, int initialY);
bool addSegment(SnakeSegment **head, int x, int y);
void removeTail(SnakeSegment *head);

int wrapCoordinate(int coordinate, int max);

typedef enum {
  UP,
  DOWN,
  LEFT,
  RIGHT,
  DIRECTION_COUNT // Count of directions
} Direction;

extern Point directions[DIRECTION_COUNT]; // Declare the directions array

#endif // SNAKE_H
