#define MAX_THREADS 16

// 新しくスレッドを表す構造体
typedef struct {
  void (*fun)();
  int proc_id;  // プロセスIDを持つ
  int used;
  uint64 context[16];
} uthread_t;

int make_uthread(void (*fun)());
void start_uthreads();
void yield();
int mytid();

