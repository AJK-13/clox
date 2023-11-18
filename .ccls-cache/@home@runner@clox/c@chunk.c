#include <stdlib.h>

#include "chunk.h"
#include "memory.h"

/* 
initChunk: Initial Chunk
    - Gets a parameter of type Chunk which is a pointer.
    - "chunk->count = 0;".
        - dereferences the chunk pointer so it gets the actual             value instead of the address then sets the "count"               value to 0.
    -  Starts completely empty (count, capactiy, and code all set to 0 or null) .
        - Sets up the array of constant values too.
        - Sets up line values all to 0 too.
*/
void initChunk(Chunk* chunk) {
  chunk->count = 0;
  chunk->capacity = 0;
  chunk->code = NULL;
  chunk->lineCount = 0;
  chunk->lineCapacity = 0;
  chunk->lines = NULL;
  initValueArray(&chunk->constants);
}

/*
freeChunk: Pretty self-explanitory, just free up the space in the memory
*/
void freeChunk(Chunk* chunk) {
  FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
  FREE_ARRAY(LineStart, chunk->lines, chunk->lineCapacity);
  freeValueArray(&chunk->constants);
  initChunk(chunk);
}

/*
writeChunk: Create a new chunk
    - Gets a parameter of type Chunk which is a pointer, and a parameter of type uint8_t which is a byte.
    - First we check to see if the chunk capacity is unused:
        - If the capacity isn't full then we store byte in the            code address and increment count by 1.
        - If the capacity is full then we store the old                  capacity in a variable called "oldCapacity" and then           we grow the capacity and make room for the new byte.
    - From the 2nd "if" statement we check to see if the line        for the byte we are currently on is the same current           line we don't need to create a new one.  
    - From the 3rd "if" statement we check to see if it is the       first byte or if it is on a new line, in which we grow         the array to fit.
*/
void writeChunk(Chunk* chunk, uint8_t byte, int line) {
  if (chunk->capacity < chunk->count + 1) {
    int oldCapacity = chunk->capacity;
    chunk->capacity = GROW_CAPACITY(oldCapacity);
    chunk->code = GROW_ARRAY(uint8_t, chunk->code, oldCapacity, chunk->capacity);
  }

  chunk->code[chunk->count] = byte;
  chunk->count++;

  // See if we're still on the same line.
  if (chunk->lineCount > 0 && chunk->lines[chunk->lineCount].line == line) {
    return;
  }

  // Append a new LineStart.
  if (chunk->lineCapacity < chunk->lineCount + 1) {
    int oldCapacity = chunk->lineCapacity;
    chunk->lineCapacity = GROW_CAPACITY(oldCapacity);
    chunk->lines = GROW_ARRAY(LineStart, chunk->lines, oldCapacity, chunk->lineCapacity);
  }

  LineStart* lineStart = &chunk->lines[chunk->lineCount++];
  lineStart->offset = chunk->count - 1;
  lineStart->line = line;
}

/*
addConstant: Add a constant to the array. 
    - After the constant is added, return the index (current -       1) in which the constant was added to.
*/
int addConstant(Chunk* chunk, Value value) {
  writeValueArray(&chunk->constants, value);
  return chunk->constants.count - 1;
} 

/*
getLine: (Chapter 14 challenge)
  - Searches for which line contains the bytecode "offset" and     returns that value.
*/
int getLine(Chunk* chunk, int instruction) {
  int start = 0;
  int end = chunk->lineCount - 1;
  for (;;) {
    int mid = (start + end) / 2;
    LineStart* line = &chunk->lines[mid];
    if (instruction < line->offset) {
      end = mid - 1;
    } else if (mid == chunk->lineCount - 1 || instruction < chunk->lines[mid + 1].offset) {
      return line->line;
    } else {
      start = mid + 1;
    }
  }
}