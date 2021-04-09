#ifndef _THRDPOOL_H_
#define _THRDPOOL_H_

#include <pthread.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <memory.h>

#include "../util/lst.h"
#include "../util/rbtree.h"
#include "../util/types.h"

typedef struct {
    pthread_t pid;
    int epfd;
    int task_event[2]; //管道事件
    int ntasks;
    rb_node link;
    rb_root rb_task;
} thrd_t;

typedef struct {
    int fd;
    rb_node link;
} task_t;

typedef struct {
    int fd;
    lst link;
} taskfd_t;

class thrdpool {
private:
    rb_root rb_thrd;
    pthread_mutex_t rb_mutex;
    // pthread_key_t key;

    lst tasklst;
    pthread_mutex_t t_mutex;

    int isterminate;

private:
    int init_lock();
    int destroy_lock();

public:
    thrdpool();
    int create_threads(size_t);
    void terminate();
    ~thrdpool();

    friend void *__routine(void *arg);
};

#endif
