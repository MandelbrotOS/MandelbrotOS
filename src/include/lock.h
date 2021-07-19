#ifndef __LOCK_H__
#define __LOCK_H__

// These are spinlocks. Not mutexs.

#define DECLARE_LOCK(name) volatile int name##Locked

#define LOCK(name)                                                             \
  while (!__sync_bool_compare_and_swap(&name##Locked, 0, 1))                   \
    ;                                                                          \
  __sync_synchronize();

#define UNLOCK(name)                                                           \
  __sync_synchronize();                                                        \
  name##Locked = 0;

#define MAKE_LOCK(name)                                                        \
  static DECLARE_LOCK(name);                                                   \
  LOCK(name);

#endif
