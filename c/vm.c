/*
Value Stack Manipulator: 
  - The main idea of this is that it is a Stack that suits our purposes        best becase the first element in the stack is the last one out.
  - It uses "push" and "pop" to add and remove elements from the stack         respectively.
*/

#include <stdio.h>

#include "common.h"
#include "compiler.h"
#include "debug.h"
#include "vm.h"

VM vm;

static void resetStack() {
  vm.stackTop = vm.stack;
}

void initVM() {
  resetStack();
}

void freeVM() {
  
}

void push(Value value) {
  *vm.stackTop = value; // Since "stackTop" points just past the last used                            element in the stack, it stores "value" in the                             next available slot.
  vm.stackTop++;        // Increment the "stackTop" so that it points just                            past the last used element, which we just                                  inserted.
}

Value pop() {
  vm.stackTop--;        // Decrement "stackTop" so that it points to the                              last used element, instead of one ahead it. We                             don't have to explicitly remove it.
  return *vm.stackTop;
}

/* InterpretResult: Heart of the VM. 
    - Constantly loops through each instruction and does the corresponding        execution. 
    - This process takes the bytecode and executes it, "dispatching" or           "decoding" the instruction.
    - TODO: look up “direct threaded code”, “jump table”, and “computed goto” these are all optimizations that may increase performance.
*/
static InterpretResult run() {
  #define READ_BYTE() (*vm.ip++)
  #define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
  #define BINARY_OP(op) \
  do { \
    double b = pop(); \
    double a = pop(); \
    push(a op b); \
  } while (false)

    for (;;) {
#ifdef DEBUG_TRACE_EXECUTION
      printf("          ");
      for (Value* slot = vm.stack; slot < vm.stackTop; slot++) {
        printf("[ ");
        printValue(*slot);
        printf(" ]");
      }
      printf("\n");
      disassembleInstruction(vm.chunk,
         (int)(vm.ip - vm.chunk->code));
#endif
      uint8_t instruction;
      switch(instruction = READ_BYTE()) {
        case OP_CONSTANT: {
          Value constant = READ_CONSTANT();
          push(constant);
          break;
        }
        case OP_ADD:      BINARY_OP(+); break;
        case OP_SUBTRACT: BINARY_OP(-); break;
        case OP_MULTIPLY: BINARY_OP(*); break;
        case OP_DIVIDE:   BINARY_OP(/); break;
        case OP_NEGATE:   push(-pop()); break;
        // { *vm.stackTop = -*vm.stackTop; break; }
        case OP_RETURN: {
          printValue(pop());
          printf("\n");
          return INTERPRET_OK;
        }
      }
    }
  
  #undef READ_BYTE
  #undef READ_CONSTANT
  #undef BINARY_OP
}

// InterpretResult interpret(const char* source) {
//   Chunk chunk;
//   initChunk(&chunk);

//   if (!compile(source, &chunk)) {
//     freeChunk(&chunk);
//     return INTERPRET_COMPILE_ERROR;
//   }

//   vm.chunk = &chunk;
//   vm.ip = vm.chunk->code; 
//   // ^-- vm.ip always points to the next instruction that hasn't been read       yet, not the current one.

//   InterpretResult result = run();

//   freeChunk(&chunk);
//   return result;
// }

InterpretResult interpret(const char* source) {
  compile(source);
  return INTERPRET_OK;
}