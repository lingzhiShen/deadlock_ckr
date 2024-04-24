#include "deadlock_graph.h"

int search_vertex(struct graph *g, struct resource *v)
{
    int i;
    //printf("search_vertex 11111111111111111\n");
    for (i = 0; i < g->procs_num; ++i) {
        if (g->processes[i].res.pid == v->pid) {
            //printf("search_vertex 2222222222222222\n");
            return i;
        }
    }


    return -1;
}

int add_vertex(struct graph *g, struct resource *v)
{
    //printf("add_vertex 11111111111111111\n");
    if (search_vertex(g, v) == -1)
    {
        g->processes[g->procs_num].res.type = PROCESS;
        g->processes[g->procs_num].res.pid = v->pid;
        g->processes[g->procs_num].pnext = NULL;

        g->procs_num++;
    }
    //printf("add_vertex 22222222222222222\n");

    return 0;
}


int del_vertex(struct graph *g, struct resource *v)
{
    int idx = search_vertex(g, v);
    if (idx != -1)
    {
        for (idx; idx < g->procs_num-1; ++idx)
        {
            g->processes[idx] = g->processes[idx+1];
        }
        g->procs_num--;
    }
    return 0;
}

int search_edge(struct graph *g, struct resource *fromv, struct resource *tov)
{
    int idx = search_vertex(g, fromv);
    if (idx != -1)
    {
        struct vertex *v = g->processes[idx].pnext;
        while(v != NULL)
        {
            if (v->res.pid == tov->pid)
            {
                return 1;
            }
            v = v->pnext;
        }
    }
    return 0;
}

int add_edge(struct graph *g, struct resource *fromv, struct resource *tov)
{
    add_vertex(g, fromv);
    add_vertex(g, tov);

    struct vertex *to = (struct vertex *)malloc(sizeof(struct vertex));
    if (to == NULL)
    {
        return -1;
    }
    to->res.pid = tov->pid;
    to->res.type = PROCESS;
    to->pnext = NULL;

    int idx = search_vertex(g, fromv);
    struct vertex *v = &(g->processes[idx]);
    while (v->pnext != NULL)
    {
        v = v->pnext;
    }
    v->pnext = to;

    return 0;
}

int del_edge(struct graph *g, struct resource *fromv, struct resource *tov)
{
    int idxi = search_vertex(g, fromv);
    int idxj = search_vertex(g, tov);
    if (idxi != -1 && idxj != -1)
    {
        struct vertex *pv = &g->processes[idxi];
        struct vertex *v = g->processes[idxi].pnext;
        while(v != NULL)
        {
            if (v->res.pid == tov->pid)
            {
                pv->pnext = v->pnext;
                free(v);

            }
            pv = v;
            v = v->pnext;
        }
    }

    return 0;
}

int deadlock = 0;
int visited[MAX_VERTEX] = {0};
int path[MAX_VERTEX] = {-1};
int k = 0;

void printf_deadlock_path(struct graph *g)
{
    int i;
    for (i = 0; i < k; ++i)
    {
        printf("pid(%d)  -->  ", g->processes[path[i]].res.pid);
    }
    printf("\n");
}

int DFS(struct graph *g, struct resource *v)
{
    int idx = search_vertex(g, v);
    if (visited[idx] == 1)
    {
        deadlock = 1;
        path[k++] = idx;
        printf_deadlock_path(g);
        return 0;
    }

    visited[idx] = 1;
    path[k++] = idx;

    struct vertex *vv = &(g->processes[idx]);
    while(vv->pnext != NULL)
    {
        DFS(g, &(vv->pnext->res));
        k--;

        vv = vv->pnext;
    }
    return 0;
}

int search_for_cycle(struct graph *g)
{
    int i;
    for (i = 0; i < g->procs_num; ++i)
    {
        k = 0;
        int j;
        for (j = 0; j < MAX_VERTEX; ++j)
        {
            visited[j] = 0;
            path[j] = -1;
        }

        DFS(g, &(g->processes[i].res));
        if (deadlock == 1)
        {
            break;
        }
    }
    return 0;
}

#if 0
int main()
{
    struct graph g;
    g.procs_num = 0;
    g.lock_num = 0;

    struct resource v1;
    v1.pid = 1;
    add_vertex(&g, &v1);
//printf("111111111111\n");
    struct resource v2;
    v2.pid = 2;
    add_vertex(&g, &v2);
//printf("2222222222\n");
    add_edge(&g, &v1, &v2);
    //add_edge(&g, &v2, &v1);
//printf("333333333333\n");
    struct resource v3;
    v3.pid = 3;
    add_vertex(&g, &v3);

    add_edge(&g, &v2, &v3);
    add_edge(&g, &v3, &v1);

    struct resource v4;
    v4.pid = 4;
    add_vertex(&g, &v4);
    add_edge(&g, &v1, &v4);
    add_edge(&g, &v4, &v1);

    search_for_cycle(&g);

    return 0;
}
#endif
