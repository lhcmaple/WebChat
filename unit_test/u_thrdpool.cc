#include <iostream>
#include <pthread.h>
#include <unistd.h>

#include "../thrdpool/thrdpool.h"

pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;

void routine1(void *arg)
{
    long long *p=(long long *)arg;
    // pthread_mutex_lock(&mutex);
    (*p)+=1;
    // pthread_mutex_unlock(&mutex);
}

void routine2(void *arg)
{
    long long *p=(long long *)arg;
    // pthread_mutex_lock(&mutex);
    (*p)+=2;
    // pthread_mutex_unlock(&mutex);
}

void routine3(void *arg)
{
    long long *p=(long long *)arg;
    // pthread_mutex_lock(&mutex);
    (*p)+=3;
    // pthread_mutex_unlock(&mutex);
}

void routine5(void *arg)
{
    long long *p=(long long *)arg;
    // pthread_mutex_lock(&mutex);
    (*p)+=5;
    // pthread_mutex_unlock(&mutex);
}

int main(int argc,char *argv[])
{
    thrdpool pool;
    pool.create_threads(10);
    long long record = 0,target = 1e10;
    for(long long i=0;i<(target/10);++i)
    {
        pool.push_task(routine1,&record);
        pool.push_task(routine2,&record);
        pool.push_task(routine3,&record);
        pool.push_task(routine5,&record);
        std::cout<<record<<std::endl;
        pause();
    }
    std::cout<<(record == target)<<std::endl;
    return 0;
}