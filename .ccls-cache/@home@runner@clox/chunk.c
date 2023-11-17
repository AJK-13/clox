#include <stdlib.h>

#include "chunk.h"
#include "memory.h"

/* 
initChunk: Initial Chunk
    - Gets a parameter of type Chunk which is a pointer.
    - "chunk->count = 0;".
        - dereferences the chunk pointer so it gets the actual             value instead of the address then sets the "count"               value to 0.
    -  Starts completely empty (count, capactiy, and code all set to 0 or null) .
*/
void initChunk(Chunk* chunk) {
  chunk->count = 0;
  chunk->capacity = 0;
  chunk->code = NULL;
}

/*
writeChunk: Create a new chunk
    - Gets a parameter of type Chunk which is a pointer, and a parameter of type uint8_t which is a byte.
    - First we check to see if the chunk capacity is unused:
        - If the capacity isn't full then we store byte in the             code address and increment count by 1.
        - If the capacity is full then we store the old capacity           in a variable called "oldCapacity" and then we grow              the capacity and make room for the new byte.
*/
void writeChunk(Chunk* chunk, uint8_t byte) {
  if (chunk->capacity < chunk->count + 1) {
    int oldCapacity = chunk->capacity;
    chunk->capacity = GROW_CAPACITY(oldCapacity);
    chunk->code = GROW_ARRAY(uint8_t, chunk->code, oldCapacity, chunk->capacity);
  }

  chunk->code[chunk->count] = byte;
  chunk->count++;
}