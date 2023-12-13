#define MAX_THREADS 16

// Saved registers for kernel context switches. (from kernel/proc.h)
struct context {
  uint64 ra;
  uint64 sp;

  // callee-saved
  uint64 s0;
  uint64 s1;
  uint64 s2;
  uint64 s3;
  uint64 s4;
  uint64 s5;
  uint64 s6;
  uint64 s7;
  uint64 s8;
  uint64 s9;
  uint64 s10;
  uint64 s11;
};

// swtch.S (from kernel/defs.h)
void swtch(struct context*, struct context*);
#define STACK_DEPTH 512

// define uthread_t
typedef struct {
  void (*fun)();
  // identifer of process
  int used;
  struct context context;
  uint64 stack[STACK_DEPTH];
} uthread_t;

int make_uthread(void (*fun)());
void start_uthreads();
void yield();
int mytid();
void uthread_exit();

