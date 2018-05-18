=== libgettid ===

A small library to map `pthread_t` to linux thread ids.

To work, `syscall(__NR_gettid)` has to be called in the thread,
so libgetting has to be `LD_PRELOAD`-ed has it hooks into `pthread_create`
