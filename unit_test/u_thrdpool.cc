#include <iostream>
#include <pthread.h>
#include <unistd.h>

#include "../thrdpool/thrdpool.h"

pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;

int count[4];

void routine1(void *arg)
{
    // printf("*1*\n");
    long long *p=(long long *)arg;
    pthread_mutex_lock(&mutex);
    (*p)+=1;
    count[0]++;
    pthread_mutex_unlock(&mutex);
}

void routine2(void *arg)
{
    // printf("*2*\n");
    long long *p=(long long *)arg;
    pthread_mutex_lock(&mutex);
    (*p)+=2;
    count[1]++;
    pthread_mutex_unlock(&mutex);
}

void routine3(void *arg)
{
    // printf("*3*\n");
    long long *p=(long long *)arg;
    pthread_mutex_lock(&mutex);
    (*p)+=2;
    count[2]++;
    pthread_mutex_unlock(&mutex);
}

void routine5(void *arg)
{
    // printf("*5*\n");
    long long *p=(long long *)arg;
    pthread_mutex_lock(&mutex);
    (*p)+=5;
    count[3]++;
    pthread_mutex_unlock(&mutex);
}

int main(int argc,char *argv[])
{
    thrdpool pool;
    pool.create_threads(100);
    long long record = 0,target = 1e7;
    for(long long i=0;i<(target/10);++i)
    // for(long long i=0;i<1;++i)
    {
        pool.push_task(&routine1,&record);
        
        pool.push_task(&routine2,&record);
        
        pool.push_task(&routine3,&record);
        
        pool.push_task(&routine5,&record);
        
        std::cout<<record<<std::endl;
    }
    sleep(5);
    for(int i=0;i<4;++i)
        std::cout<<count[i]<<" ";
    std::cout<<std::endl;
    std::cout<<(record == target)<<std::endl;
    return 0;
}
