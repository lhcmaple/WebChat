#ifndef _THRDPOOL_H_
#define _THRDPOOL_H_

#include <pthread.h>

#include "../util/lst.h"
#include "../util/debug.h"

typedef void (*task_func)(void *);

typedef struct {
    pthread_t pid;
    lst link;
} thrd_t;

typedef struct {
    task_func routine;
    void *context;
    lst link;
} task_t;

class thrdpool {
private:

    lst thrdlst;
    lst tasklst;

    pthread_mutex_t th_mutex;
    pthread_mutex_t tk_mutex;
    pthread_cond_t  tk_cond;

    bool isterminate;

private:
    int init_lock();
    int destroy_lock();
    void terminate();

public:
    thrdpool();
    ~thrdpool();

    int create_threads(size_t);
    void push_task(task_func routine,void *context);

    friend void *__routine(void *arg);
};

#endif

