// user_threads.c
#include "kernel/types.h"
#include "user/user.h"  
#include "kernel/stat.h"
// # include "kernel/defs.h"
#include "user/uthreads.h"

uthread_t uthreads[MAX_THREADS];

// 他のスレッドに実行を譲る
// void yield(void) {
//   int tid = mytid();
//   if (tid > 0 && tid < MAX_THREADS && uthreads[tid].used) {
//     // スレッドを切り替える
//     swtch(&uthreads[tid].context, &uthreads[0].context);
//   }
// }

void yield(void) {
  // 未実装
}


int make_uthread(void (*fun)()) {
  int proc_id = getpid();

  for (int tid = 1; tid < MAX_THREADS; tid++) {
    if (!uthreads[tid].used) {
      uthreads[tid].fun = fun;
      uthreads[tid].proc_id = proc_id; // プロセスIDを設定
      uthreads[tid].used = 1;
      uthreads[tid].context[0] = (uint64)fun; // スレッドの開始アドレスを設定
      return tid;
    }
  }
  return -1;  // スレッドの作成ができない場合
}

void start_uthreads(void) {
  int proc_id = getpid();
  // get tid
  int tid = mytid();
  if (tid > 0 && tid < MAX_THREADS && uthreads[tid].used && uthreads[tid].proc_id == proc_id) {
    uthreads[tid].fun();  // スレッドを起動
  }
}

int mytid(void) {
  int proc_id = getpid();
  
  // プロセスごとに異なるtidを返す
  for (int tid = 1; tid < MAX_THREADS; tid++) {
    if (uthreads[tid].used && uthreads[tid].proc_id == proc_id) {
      return tid;
    }
  }

  return -1;
}


