#include "snake.h"
#include <stdlib.h>

SnakeSegment *createSnake(int initialX, int initialY) {
  SnakeSegment *head = malloc(sizeof(SnakeSegment));
  head->position.x = initialX;
  head->position.y = initialY;
  head->next = NULL;
  return head;
}

bool addSegment(SnakeSegment **head, int x, int y) {

  if (head == NULL) {
    // The head pointer itself is NULL, which shouldn't happen
    return false;
  }

  // check for collision
  for (SnakeSegment *current = *head; current != NULL;
       current = current->next) {
    if (current->position.x == x && current->position.y == y) {
      return false;
    }
  }

  SnakeSegment *newSegment = malloc(sizeof(SnakeSegment));
  newSegment->position.x = x;
  newSegment->position.y = y;
  newSegment->next = *head;
  *head = newSegment;
  return true;
}

void removeTail(SnakeSegment *head) {
  SnakeSegment *current = head;
  SnakeSegment *prev = NULL;
  while (current->next != NULL) {
    prev = current;
    current = current->next;
  }
  free(current);
  prev->next = NULL;
}

Point directions[DIRECTION_COUNT] = {
    {0, -1}, // UP
    {0, 1},  // DOWN
    {-1, 0}, // LEFT
    {1, 0}   // RIGHT
};

int wrapCoordinate(int value, int max) {
  if (value < 0) {
    return max + value % max;
  } else {
    return value % max;
  }
}

// Use these functions to manage the snake in your game
