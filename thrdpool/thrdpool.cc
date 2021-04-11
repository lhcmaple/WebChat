#include "thrdpool.h"

void *__routine(void *arg)
{
    thrdpool *pool=(thrdpool *)arg;

    thrd_t *thrd=new thrd_t;
    thrd->pid=pthread_self();

    pthread_mutex_lock(&pool->th_mutex);
    if(pool->isterminate)
    {
        pthread_mutex_unlock(&pool->th_mutex);
        delete thrd;
        pthread_detach(pthread_self());
        return 0;
    }
    insert_back(&pool->thrdlst,&thrd->link);
    pthread_mutex_unlock(&pool->th_mutex);

    while(true)
    {
        pthread_mutex_lock(&pool->tk_mutex);
        DBP("%lu\n",pthread_self());
        while(LST_EMPTY(&pool->tasklst) && !pool->isterminate)
            pthread_cond_wait(&pool->tk_cond,&pool->tk_mutex);
        if(pool->isterminate)
            break;
        lst *ptr  = del_back(&pool->tasklst);
        pthread_mutex_unlock(&pool->tk_mutex);
        task_t *t = lst_entry(ptr,task_t,link);
        DBP("%lu\n",pthread_self());
        t->routine(t->context);
        DBP("%lu\n",pthread_self());
        delete t;
    }
    pthread_mutex_unlock(&pool->tk_mutex);

    return 0;
}

thrdpool::thrdpool()
{
    INIT_LST(&thrdlst);
    INIT_LST(&tasklst);
    init_lock();
    isterminate = false;
}

thrdpool::~thrdpool()
{
    terminate();
    destroy_lock();
}

int thrdpool::create_threads(size_t nthreads)
{
    pthread_t pid;

    while(nthreads--)
    {
        DBP("%ld\n",nthreads);
        if(pthread_create(&pid,NULL,__routine,this) != 0)
        {
            terminate();
            return -1;
        }
    }

    DBP("create_threads finished\n");
    return 0;
}

void thrdpool::terminate()
{
    pthread_mutex_lock(&th_mutex);
    isterminate=true;
    pthread_mutex_unlock(&th_mutex);
    pthread_cond_broadcast(&tk_cond);
    while(!LST_EMPTY(&thrdlst))
    {
        lst *ptr     = del_front(&thrdlst);
        thrd_t *thrd = lst_entry(ptr,thrd_t,link);
        pthread_join(thrd->pid,NULL);
        delete thrd;
    }
}

int thrdpool::init_lock()
{
    pthread_mutex_init(&tk_mutex, NULL);
    pthread_mutex_init(&th_mutex, NULL);
    pthread_cond_init(&tk_cond,NULL);
    return 0;
}

int thrdpool::destroy_lock()
{
    if(pthread_mutex_destroy(&tk_mutex) != 0)
        return -1;
    if(pthread_cond_destroy(&tk_cond) !=0)
        return -2;
    if(pthread_mutex_destroy(&th_mutex) != 0)
        return -3;
    return 0;
}

void thrdpool::push_task(task_func routine,void *context)
{
    task_t *t  = new task_t;
    t->context = context;
    t->routine = routine;
    pthread_mutex_lock(&tk_mutex);
    insert_front(&tasklst,&t->link);
    pthread_mutex_unlock(&tk_mutex);
    pthread_cond_signal(&tk_cond);
}
