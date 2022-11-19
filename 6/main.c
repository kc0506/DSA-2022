#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define MAX_N 1100000
#define DEBUG 0
#define HEAD 0
#define TAIL 1

typedef struct node{int value; struct node *next, *prev;} node;

int N, Q, S, R;

int in_path[MAX_N];
node ***G, *path;

int lca_table[MAX_N];

int next_int(){
    int c = getchar(), num = 0;
    while(!isdigit(c))
        c = getchar();
    while(isdigit(c))
        num = num * 10 + c - '0', c = getchar();
    return num;
}

void add_new_node(int index, int key){
    node *new_node = (node *) malloc(sizeof(node));
    new_node->value = key;
    new_node->next = NULL;
    if(G[index][HEAD] == NULL){
        G[index][HEAD] = G[index][TAIL] = new_node;
        new_node->next = new_node->prev = NULL;
    }
    else{
        G[index][TAIL]->next = new_node;
        new_node->prev = G[index][TAIL];
        G[index][TAIL] = new_node;
    }
}

void print_graph(){
    printf("Graph:\n");
    for(int i=1; i<=N; i++){
        node *cur = G[i][HEAD];
        while(cur){
            printf("%d ", cur->value);
            cur = cur->next;
        }
        printf("\n");
    }
}

void get_graph(){
    G = (node ***) malloc(sizeof(node **) * (N + 1));
    for(int i=1; i<=N; i++)
        G[i] = (node **) malloc(sizeof(node *) * 2);

    for(int i = 1; i <= N; i++)
        G[i][HEAD] = G[i][TAIL] = NULL;
    for(int i = 0; i < N-1; i++){
        int a = next_int(), b = next_int();
        add_new_node(a, b);
        add_new_node(b, a);
    }
}

int START, END;

void print_path(){
    node *cur = path;
    printf("Path:\n");
    while(cur != NULL){
        printf("%d->", cur->value);
        cur = cur->next;
    }
    printf("NULL\n");
    printf("In-path:\n");
    for(int i=1; i<=N; i++)
        printf("%d%c", in_path[i], " \n"[i == N]);
}

int dfs_path(int root, int prev){
    if(root == END){
        path = (node *) malloc(sizeof(node));
        path->next = path->prev = NULL, path->value = root;
        return (in_path[root] = 1);
    }


    node *cur = G[root][HEAD];
    while(cur != NULL){
        if(cur->value == prev){
            cur = cur->next;
            continue;
        }

        if(dfs_path(cur->value, root)){
            node *tmp = (node *) malloc(sizeof(node));
            tmp->value = root, tmp->next = path, tmp->prev = NULL, path->prev = tmp;
            path = tmp;
            return (in_path[root] = 1);
        }
        else
            cur = cur->next;
    }
    return 0;
}

void get_path(){
    // get path from R to S
    for(int i=1; i<=N; i++)
        in_path[i] = 0;

    path = NULL;
    dfs_path(START, -1);
}

void dfs_lca(int root, int prev, int lca){
    lca_table[root] = lca;

    node *cur = G[root][HEAD];
    while(cur != NULL){
        if(cur->value == prev){
            cur = cur->next;
            continue;
        }
        dfs_lca(cur->value, root, lca);
        cur = cur->next;
    }
}

void print_table(){
    printf("Table:\n");
    for(int i=1; i<=N; i++)
        printf("%d%c", lca_table[i], " \n"[i == N]);
}

void build_table(){
    node *cur = path;
    while(cur != NULL){
        int root = cur->value;
        node *nei = G[root][HEAD];

        lca_table[root] = root;
        while(nei != NULL){
            if(in_path[nei->value]){
                nei = nei->next;
                continue;
            }

            dfs_lca(nei->value, root, root);
            nei = nei->next;
        }
        cur = cur->next;
    }
}

void solve(){
    get_graph();
    get_path();
    // print_path(); return;
    build_table();
    // print_table();

    for(int i=0; i<Q; i++){
        printf("%d\n", lca_table[next_int()]);
    }
}

int main()
{
    N = next_int(), Q = next_int(), END = S = next_int(), START = R = next_int();

    solve();
    if(DEBUG){
        print_graph();
    }
}
