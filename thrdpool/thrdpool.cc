#include "thrdpool.h"

thrd_t *rb_search(rb_root *root,int val)
{
    rb_node *n = root->rb_node;
	thrd_t * page;

	while (n)
	{
		page = rb_entry(n, thrd_t, link);

		if (val < page->ntasks)
			n = n->rb_left;
		else if (val>page->ntasks)
			n = n->rb_right;
		else
			return page;
	}
	return NULL;
}

thrd_t *__rb_insert(rb_root *root,int val,rb_node *node)
{
    rb_node **p=&root->rb_node;
    rb_node *parent=NULL;
    thrd_t *page;
	while (*p)
	{
		parent = *p;
		page = rb_entry(parent, thrd_t, link);

		if (val < page->ntasks)
			p = &(*p)->rb_left;
		else
			p = &(*p)->rb_right;
	}

	rb_link_node(node, parent, p);

	return NULL;
}

thrd_t *rb_insert(rb_root *root,int val,rb_node *node)
{
    thrd_t *ret;
    if((ret=__rb_insert(root,val,node))==NULL)
    {
        rb_insert_color(node,root);
    }
    return ret;
}

void *__routine(void *arg)
{
    thrdpool *pool      = (thrdpool *) arg;
    epoll_event event;
    thrd_t *thrd        = new thrd_t;
    epoll_event *events = new epoll_event[N_EPOLL_SIZE+1];

    thrd->rb_task = RB_ROOT;
    thrd->pid     = pthread_self();
    thrd->epfd    = epoll_create(N_EPOLL_SIZE);
    thrd->ntasks  = 0;
    pipe(thrd->task_event);

    pthread_mutex_lock(&pool->rb_mutex);
    rb_insert(&pool->rb_thrd,thrd->ntasks,&thrd->link);
    pthread_mutex_unlock(&pool->rb_mutex);

    memset(&event,0,sizeof(event));
    event.events |= (EPOLLIN | EPOLLET);
    event.data.fd = thrd->task_event[0];
    epoll_ctl(thrd->epfd,EPOLL_CTL_ADD,thrd->task_event[0],&event);

    while(true)
    {
        int ready = epoll_wait(thrd->epfd,events,N_EPOLL_SIZE+1,1000);
        if(pool->isterminate)
            break;
        for(int i = 0; i < ready; ++i)
        {
            int fd = events[i].data.fd;
            if(fd == thrd->task_event[0])
            {
                char signal;
                read(fd,&signal,1);
                if(signal == 'E')
                    goto TERMINATE;
                else if(signal=='T')
                {
                    task_t *tk = new task_t;
                    pthread_mutex_lock(&pool->t_mutex);
                    lst *tmp = pop_back(&pool->tasklst);
                    pthread_mutex_unlock(&pool->t_mutex);
                    tk->fd = lst_entry(tmp,taskfd_t,link)->fd;
                    delete tmp;
                    rb_insert(&thrd->rb_task,tk->fd,&tk->link);

                    memset(&event,0,sizeof(event));
                    event.events |= (EPOLLIN | EPOLLET);
                    event.data.fd=tk->fd;
                    epoll_ctl(thrd->epfd,EPOLL_CTL_ADD,tk->fd,&event);
                }
            }
            else
            {
                // rb_node *node = rb_search(&thrd->rb_task,fd);

            }
        }
    }

    TERMINATE:

    pthread_mutex_lock(&pool->rb_mutex);
    close(thrd->task_event[0]);
    close(thrd->task_event[1]);
    delete thrd;
    delete[] events;
    close(thrd->epfd);
    pthread_mutex_unlock(&pool->rb_mutex);

    pthread_detach(pthread_self());
    return 0;
}

thrdpool::thrdpool()
{
    rb_thrd     = RB_ROOT;
    INIT_LST(&tasklst);
    isterminate = false;
    init_lock();
}

thrdpool::~thrdpool()
{
    terminate();
    destroy_lock();
}

int thrdpool::create_threads(size_t nthreads)
{
    pthread_t pid;

    // if(pthread_key_create(&key,NULL) != 0)
        // return -1;
    while(nthreads--)
    {
        if(pthread_create(&pid,NULL,__routine,this) != 0)
        {
            terminate();
            // pthread_key_delete(key);
            return -1;
        }
    }
    return 0;
}

void thrdpool::terminate()
{
    isterminate=true;
    pthread_mutex_lock(&rb_mutex);
    rb_node *node = rb_first(&rb_thrd);
    while(node != NULL)
    {
        thrd_t *thrd = rb_entry(node,thrd_t,link);
        write(thrd->task_event[1],SIGNAL_EXIT,1);
        node = rb_next(node);
    }
    pthread_mutex_unlock(&rb_mutex);
    
}

int thrdpool::init_lock()
{
    pthread_mutex_init(&t_mutex, NULL);
    pthread_mutex_init(&rb_mutex,NULL);
    return 0;
}

int thrdpool::destroy_lock()
{
    if(pthread_mutex_destroy(&t_mutex) != 0)
        return -1;
    if(pthread_mutex_destroy(&rb_mutex) !=0)
        return -2;
    return 0;
}
