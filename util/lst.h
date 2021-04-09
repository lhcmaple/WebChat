#ifndef _LST_H_
#define _LST_H_

struct lst {
    lst *prev;
    lst *next;
};

#define LST_INIT {NULL,NULL}
#define INIT_LST(plst) \
{ \
    (plst)->prev = (plst); \
    (plst)->next = (plst); \
}
#define LST_EMPTY(plst) ((plst)->next == plst)
#define lst_entry(ptr,type,link) ((type *) (ptr - &((type *) 0)->link))

//在ptr1后插入ptr2
static inline void __insert_after(lst *ptr1,lst *ptr2) {
    ptr2->next       = ptr1->next;
    ptr1->next->prev = ptr2;
    ptr1->next       = ptr2;
    ptr2->prev       = ptr1;
}

//在链表首插入ptr
inline void push_front(lst *plst,lst *ptr) {
    __insert_after(plst,ptr);
}

//在链表尾插入ptr
inline void push_back(lst *plst,lst *ptr)
{
    __insert_after(plst->prev,ptr);
}

//在ptr1后插入ptr2
inline void insert_after(lst *ptr1,lst *ptr2) {
    __insert_after(ptr1,ptr2);
}

inline void lst_del(lst *ptr)
{
    ptr->next->prev = ptr->prev;
    ptr->prev->next = ptr->next;
}

inline lst *pop_front(lst *plst)
{
    if(LST_EMPTY(plst))
        return NULL;
    lst_del(plst->next);
    return plst->next;
}

inline lst *pop_back(lst *plst)
{
    if(LST_EMPTY(plst))
        return NULL;
    lst_del(plst->prev);
    return plst->prev;
}

#endif
