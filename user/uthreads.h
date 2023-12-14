#define MAX_THREADS 16
#define STACK_DEPTH 512

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

typedef enum { UT_READY, UT_SLEEP } uthread_state_t;

// define uthread_t
typedef struct {
  void (*fun)();
  int used;
  struct context context;
  uint64 stack[STACK_DEPTH];
  uthread_state_t state;
  void *waiting_on;
} uthread_t;


// functions
void swtch(struct context*, struct context*);
int make_uthread(void (*fun)());
void start_uthreads();
void yield();
int mytid();
void uthread_exit();
void uthread_wait(void *a);
void uthread_notify(int tid, void *a);
void uthread_notify_all(void *a);


