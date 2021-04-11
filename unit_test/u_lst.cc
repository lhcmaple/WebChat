#include <stdio.h>

#include "../util/lst.h"

struct data {
    int val;
    lst link;
};

int main(int argc,char *argv[])
{
    lst t;
    INIT_LST(&t);
    if(LST_EMPTY(&t))
        printf("empty\n");
    for(int i=0;i<100;++i)
    {
        data *d=new data;
        d->val=i;
        insert_front(&t,&d->link);
    }
    while(!LST_EMPTY(&t))
    {
        lst *tmp=del_back(&t);
        data *d=lst_entry(tmp,data,link);
        printf("%d ",d->val);
        delete d;
    }
    printf("\n");
    return 0;
}