#ifndef _GRAPH_H
#define _GRAPH_H

#include <stdint.h>
#include <stddef.h>
#include <malloc.h>
#include <stdio.h>

#define MAX_VERTEX  200
#define MAX_LOCK    200

enum resource_type{ PROCESS=0, LOCK=1 };

struct resource {
    enum resource_type type;
    uint64_t pid;
};

struct vertex{
    struct resource res;
    struct vertex *pnext;
};

struct graph{
    struct vertex processes[MAX_VERTEX];
    int procs_num;

    struct resource locks[MAX_LOCK];
    int lock_num;
};

int search_vertex(struct graph *g, struct resource *v);
int add_vertex(struct graph *g, struct resource *v);
int del_vertex(struct graph *g, struct resource *v);
int search_edge(struct graph *g, struct resource *fromv, struct resource *tov);
int add_edge(struct graph *g, struct resource *fromv, struct resource *tov);
int del_edge(struct graph *g, struct resource *fromv, struct resource *tov);
int DFS(struct graph *g, struct resource *v);

#endif // _GRAPH_H
