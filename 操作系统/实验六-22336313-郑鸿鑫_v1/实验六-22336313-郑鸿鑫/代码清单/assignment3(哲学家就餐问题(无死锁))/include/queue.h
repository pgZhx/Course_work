#include "os_type.h"
struct QUEUE{
    int max_size;
    int front,tail;
    int queue[10];
    QUEUE();
    bool empty();
    bool full ();
    bool push(int item);
    bool pop();
    void show();
    void init();
};
