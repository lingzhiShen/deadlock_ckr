#ifndef _DEADLOCK_DETECTION_H
#define _DEADLOCK_DETECTION_H

#include <pthread.h>
#include <stdint.h>

#define MAX_LOCK 200

struct lock{
    uint64_t lock_id;
    uint64_t owner_id;

    int status;
};

struct locklist{
    struct lock locks[MAX_LOCK];
    int lock_count;
};

int add_lock(struct locklist *lklist, uint64_t lockid);
int search_lock(struct locklist *lklist, uint64_t lockid);

void init();

//int pthread_mutex_lock(pthread_mutex_t *mutex);
void before_lock(pthread_mutex_t *mutex);
void after_lock(pthread_mutex_t *mutex);

//int pthread_mutex_unlock(pthread_mutex_t *mutex);
void before_unlock(pthread_mutex_t *mutex);
void after_unlock(pthread_mutex_t *mutex);

#endif // _DEADLOCK_DETECTION_H
