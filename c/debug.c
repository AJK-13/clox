#include <stdio.h>

#include "debug.h"
#include "value.h"

/*
disassembleChunk: Loops taking a chunk and disassembles it into bytecode which is sent to disassembleInstruction for as long as the chunk is there.
*/
void disassembleChunk(Chunk* chunk, const char* name) {
  printf("== %s ==\n", name); // Prints the chunk we are looking at.

  for (int offset = 0; offset < chunk->count;) {
    offset = disassembleInstruction(chunk, offset);
  }
}

/*
constantInstruction: 
    - Prints out the name of the opcode.
    - The "constant" value is the next chunk of opcode which           carries the value of the constant.
    - "printValue" gets the value at the certain index and              prints it.
    - Then it returns the opcode at index 2 higher because it          accounts for the opcode and the next opcode constant.
*/
static int constantInstruction(const char* name, Chunk* chunk, int offset) {
  uint8_t constant = chunk->code[offset + 1];
  printf("%-16s %4d '", name, constant);
  printValue(chunk->constants.values[constant]);
  printf("'\n");
  return offset + 2;
}

/*
simpleInstruction: Takes an opcode and prints the name then returns the next byte of opcode.
*/
static int simpleInstruction(const char* name, int offset) {
  printf("%s\n", name);
  return offset + 1;
}

/*
disassembleInstruction: Takes a piece of bytecode and disassembles it into certain instructions.
    - "instruction": goes to the address of the chunk and gets         the code at a certain offset index.
    - Checks which type of opcode instruction is and returns the       corresponding values or function.
*/
int disassembleInstruction(Chunk* chunk, int offset) {
  printf("%04d ", offset); // Prints out the byte of the specific instruction.
  if (offset > 0 && chunk->lines[offset] == chunk->lines[offset - 1]) {
    printf("   | ");
  } else {
    printf("%4d ", chunk->lines[offset]);
  }
  uint8_t instruction = chunk->code[offset];
  switch (instruction) {
    case OP_CONSTANT:
      return constantInstruction("OP_CONSTANT", chunk, offset);
    case OP_RETURN:
      return simpleInstruction("OP_RETURN", offset);
    default:
      printf("Unknown opcode %d\n", instruction);
      return offset + 1;
  }
}