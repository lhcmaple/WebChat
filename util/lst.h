#ifndef _LST_H_
#define _LST_H_

struct lst {
    lst *prev;
    lst *next;
};

#define INIT_LST(plst) \
{ \
    (plst)->prev = (plst); \
    (plst)->next = (plst); \
}
#define LST_EMPTY(plst) ((plst)->next == plst)
#define lst_entry(ptr,type,link) ( (type *) ((char *) ptr - (unsigned long) &((type *) 0)->link) )

//在ptr1后插入ptr2
static inline void __insert_after(lst *ptr1,lst *ptr2) {
    ptr2->next       = ptr1->next;
    ptr2->prev       = ptr1;
    ptr1->next->prev = ptr2;
    ptr1->next       = ptr2;
}

//在链表首插入ptr
static inline void insert_front(lst *plst,lst *ptr) {
    __insert_after(plst,ptr);
}

//在链表尾插入ptr
static inline void insert_back(lst *plst,lst *ptr)
{
    __insert_after(plst->prev,ptr);
}

//在ptr1后插入ptr2
static inline void insert_after(lst *ptr1,lst *ptr2) {
    __insert_after(ptr1,ptr2);
}

static inline void lst_del(lst *ptr)
{
    ptr->next->prev = ptr->prev;
    ptr->prev->next = ptr->next;
}

static inline lst *del_front(lst *plst)
{
    if(LST_EMPTY(plst))
        return NULL;
    lst *ret = plst->next;
    lst_del(ret);
    return ret;
}

static inline lst *del_back(lst *plst)
{
    if(LST_EMPTY(plst))
        return NULL;
    lst *ret = plst->prev;
    lst_del(ret);
    return ret;
}

#endif
