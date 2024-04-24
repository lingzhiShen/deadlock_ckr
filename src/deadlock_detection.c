#include "deadlock_detection.h"
#include "deadlock_graph.h"

struct graph g_graph;
struct locklist g_locklist;

void init()
{
    g_graph.lock_num = 0;
    g_graph.procs_num = 0;
    g_locklist.lock_count = 0;
}

int add_lock(struct locklist *lklist, uint64_t lockid)
{
    lklist->locks[lklist->lock_count].lock_id = lockid;
    lklist->locks[lklist->lock_count].owner_id = pthread_self();
    lklist->locks[lklist->lock_count].status = 1;

    lklist->lock_count++;

    return 0;
}

int search_lock(struct locklist *lklist, uint64_t lockid)
{
    int i;
    for (i = 0; i < lklist->lock_count; ++i)
    {
        if ((lklist->locks[i].status == 1) && (lklist->locks[i].lock_id == lockid))
        {
            return i;
        }
    }
    return -1;
}

void before_lock(pthread_mutex_t *mutex)
{
    int idx = search_lock(&g_locklist, (uint64_t)mutex);
    if (idx != -1)
    {
        struct resource from;
        from.pid = pthread_self();
        from.type = PROCESS;
        add_vertex(&g_graph, &from);

        struct resource to;
        to.pid = g_locklist.locks[idx].owner_id;
        to.type = PROCESS;
        add_vertex(&g_graph, &to);

        add_edge(&g_graph, &from, &to);
    }
}

void after_lock(pthread_mutex_t *mutex)
{
    pthread_t selfid = pthread_self();

    int idx = search_lock(&g_locklist, (uint64_t)mutex);
    if (idx == -1)
    {
        struct resource v;
        v.pid = selfid;
        v.type = PROCESS;
        add_vertex(&g_graph, &v);
        add_lock(&g_locklist, (uint64_t)mutex);
    }
    else
    {
        struct resource from;
        from.pid = selfid;
        from.type = PROCESS;

        struct resource to;
        to.pid = g_locklist.locks[idx].owner_id;
        to.type = PROCESS;

        del_edge(&g_graph, &from, &to);

        g_locklist.locks[idx].owner_id = selfid;
    }
}

void before_unlock(pthread_mutex_t *mutex)
{
    //
}

void after_unlock(pthread_mutex_t *mutex)
{
    int idx = search_lock(&g_locklist, (uint64_t)mutex);
    g_locklist.locks[idx].status = 0;
}
