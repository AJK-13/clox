#include <stdlib.h>

#include "memory.h"

/*
reallocate: Dynamic memory management system (Allocating memory, freeing it, and changing the size of an existing allocation)
    - When the newSize is 0 free the memory stored in the              address of the pointer.
    - Otherwise allocate newSize to the address of pointer.
        - If newSize < oldSize update the size of the existing             block and return the same pointer given. 
        - If newSize > oldSize grow the memory block: 
            - Allocate a new memory block of the right size.
            - Copy all the old bytes in.
            - Free up the old memory.
            - Return the pointer to the new memory block.
*/

void* reallocate(void* pointer, size_t oldSize, size_t newSize) {
  if (newSize == 0) {
    free(pointer);
    return NULL;
  }

  void* result = realloc(pointer, newSize);
  if (result == NULL) exit(1);
  return result;
}