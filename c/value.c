#include <stdio.h>

#include "memory.h"
#include "value.h"

/*
initValueArray: Very similar to initChunk. Just creates a new array with everything set to 0 or null.
*/
void initValueArray(ValueArray* array) {
  array->values = NULL;
  array->capacity = 0;
  array->count = 0;
}

/*
writeValueArray: Very similar to writeChunk. Checks if there is memory space in the current capacity.
    - If there is space it adds it to the right spot and increments count.
    - If there isn't space, it creates a new larger array then         copies in all the values from the old array and adds in          the new value too.
*/
void writeValueArray(ValueArray* array, Value value) {
  if (array->capacity < array->count + 1) {
    int oldCapacity = array->capacity;
    array->capacity = GROW_CAPACITY(oldCapacity);
    array->values = GROW_ARRAY(Value, array->values,                                            oldCapacity, array->capacity);
  }

  array->values[array->count] = value;
  array->count++;
}

void freeValueArray(ValueArray* array) {
  FREE_ARRAY(Value, array->values, array->capacity);
  initValueArray(array);
}

void printValue(Value value) {
  printf("%g", value);
}