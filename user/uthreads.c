#include "kernel/types.h"
#include "user/user.h"
#include "user/uthreads.h"

// if debug is 1, print debug messages
int debug = 0;

int current_tid = -1;
int main_tid = -1;
uthread_t uthreads[MAX_THREADS];

void yield(void) {
  int tid = mytid();
  // serach for next thread from tid + 1 to tid + MAX_THREADS
  for (int i = 0; i < MAX_THREADS; i++) {
    int next_tid = (tid + i + 1) % MAX_THREADS;
    if (uthreads[next_tid].used && uthreads[next_tid].state == UT_READY)
    {
      struct context *current_context = &(uthreads[tid].context);
      struct context *next_context = &(uthreads[next_tid].context);
      current_tid = next_tid;
      if (debug) printf("YIELD : Thread %d yielding to thread %d\n", tid, next_tid);
      swtch(current_context, next_context);
      return;
    }
  }
  // Return to main thread
  int next_tid = main_tid;
  struct context *current_context = &(uthreads[tid].context);
  struct context *next_context = &(uthreads[next_tid].context);
  current_tid = next_tid;
  swtch(current_context, next_context);
  return;
}

int make_uthread(void (*fun)()) {
  for (int tid = 0; tid < MAX_THREADS; tid++)
  {
    if (!uthreads[tid].used)
    {
      uthreads[tid].used = 1;
      uthreads[tid].context.ra = (uint64)fun;
      uthreads[tid].context.sp = (uint64)(uthreads[tid].stack + STACK_DEPTH);
      uthreads[tid].state = UT_READY;
      return tid;
    }
  }
  return -1;
}

void start_uthreads(void) {
  while (1) {
    int active_threads = 0;
    for(int tid = 0; tid < MAX_THREADS; tid++) {
      if (uthreads[tid].used && uthreads[tid].state == UT_READY) {
        if (debug) printf("Thread %d is used\n", tid);
        active_threads = 1;
        current_tid = tid;
        main_tid = tid;
        // call thread function
        if (debug) printf("Thread %d is ready\n", tid);
        void (*fun)() = (void (*)())(uthreads[tid].context.ra);
        fun();
        // (used must be already 0)
        uthreads[tid].used = 0;
        current_tid = -1;
        if (debug) printf("Thread %d returned\n", tid);
      }
    }
    if (!active_threads) {
      break;
    }
  }
  return;
}


int mytid(void) {
  return current_tid;
}

void uthread_exit(void) {
  int tid = mytid();
  uthreads[tid].used = 0;
  if (debug) printf("Thread %d exited\n", tid);
  yield();
  if (debug) printf("Thread %d finish exitting\n", tid);
}

void uthread_wait(void *a) {
  int tid = mytid();
  uthreads[tid].state = UT_SLEEP;
  uthreads[tid].waiting_on = a;
  yield();
}

void uthread_notify(int tid, void *a) {
  if (uthreads[tid].waiting_on == a) {
    uthreads[tid].state = UT_READY;
  }
  return;
}

void uthread_notify_all(void *a) {
  for (int tid = 0; tid < MAX_THREADS; tid++) {
    if (uthreads[tid].waiting_on == a) {
      uthreads[tid].state = UT_READY;
    }
  }
  return;
}
