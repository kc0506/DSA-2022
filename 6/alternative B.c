#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define MAX_N 1100000
#define MAX_LgN 100
#define FIXED_ROOT 1
#define INF 1e9
#define HEAD 0
#define TAIL 1
#define DEBUG 0
#define BAD 1

typedef struct node{int value; struct node *next, *prev;} node;

int N, Q, S, R;
node ***G;

void swap(int *a, int *b){
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void suicide1(){
    if(N > 100000 || Q > 100000)
        while(1)
            printf("1");
}

void suicide2(){
    if(N > 100000 || Q > 100000)
        system("pause");
    ;
}

int next_int(){
    int c = getchar(), num = 0;
    while(!isdigit(c))
        c = getchar();
    while(isdigit(c))
        num = num * 10 + c - '0', c = getchar();
    return num;
}

int pow_2[MAX_LgN], log_2[2 * MAX_N + 100];

int euler[2 * MAX_N], depth[MAX_N], sparse_table[2 * MAX_N][MAX_LgN];  // st[i][j] store the min in [i, i + 2^j)

int visit[MAX_N], order = -1;
// use visit to find the first occur of v

void print_arr(){
    printf("Visit:\n");
    for(int i=1; i<=N; i++)
        printf("%2d ", visit[i]);
    printf("\n");

    printf("Euler:\n");
    for(int i=0; i<=order; i++)
        printf("%2d ", euler[i]);
    printf("\n");

    printf("Depth:\n");
    for(int i=0; i<=order; i++)
        printf("%2d ", depth[euler[i]]);
    printf("\n");
}

void dfs(int root, int prev, int dep){

    if(visit[root] == -1)
        visit[root] = ++order;

    depth[root] = dep;
    euler[order] = root;


    node *cur = G[root][HEAD];

    while(cur != NULL){
        if(cur->value == prev){
            cur = cur->next;
            continue;
        }
        dfs(cur->value, root, dep + 1);

        euler[++order] = root;
        cur = cur->next;
    }
}

void euler_walk(){
    for(int i=1; i<=N; i++)
        visit[i] = -1;
    int root = FIXED_ROOT;
    dfs(root, -1, 0);
}

void print_table(){
    printf("sparse table:\n");
    for(int i=0; pow_2[i] <= order; i++){
        for(int j=0; j<=order; j++)
            printf("%2d%c", sparse_table[j][i], " \n"[j == order]);
    }
}

// 進度：目前已知在build table出現RE
void build_table(){
    for(int i = 0; i <= order; i++)
        for(int j = 0; pow_2[j] <= order + 1; j++)
            sparse_table[i][j] = -1;

    for(int i = 0; i <= order; i++)
        sparse_table[i][0] = euler[i];

    suicide2();

    for(int i = 1; pow_2[i] <= order + 1; i++)
        for(int j = 0; j <= order; j++){
            // find min in [j, j + pow_2[i])
            // store the value of node
            if(j + pow_2[i] - 1 > order){
                break;
            }

            int next = j + pow_2[i - 1];
            int lvalue = sparse_table[j][i - 1], rvalue = sparse_table[next][i - 1];
            sparse_table[j][i] = (depth[lvalue] < depth[rvalue]) ?lvalue :rvalue;
        }
    suicide1();
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

void print_pow_log(){
    printf("Power:\n");
    for(int i=0; pow_2[i] <= order + 1; i++)
        printf("%d ", pow_2[i]);
    printf("\n");

    printf("Log:\n");
    for(int i=0; i <= order + 1; i++)
        printf("%d ", log_2[i]);
    printf("\n");
}

void set_pow_log(){
    int i = 0, b = 1;
    for(; b <= order + 1; i++, b = b * 2){
        pow_2[i] = b;
    }
    pow_2[i] = INF;

    int lg = i - 1;
    for(int j = order + 1; j >= 0; j--){
        if(j < pow_2[lg])
            lg--;
        log_2[j] = lg;
    }
}

void preprocessing(){
    euler_walk();

    set_pow_log();

    build_table();
}

int query(int u, int v){
    // return the value of the node that has min depth and is between u, n
    // i.e. the LCA of u and v
    if(u == v)
        return v;

    if(visit[u] > visit[v])
        swap(&u, &v);

    int l = visit[u], r = visit[v], size = r - l + 1, lg = log_2[size];

    int next = r - pow_2[lg] + 1, lvalue = sparse_table[l][lg], rvalue = sparse_table[next][lg];

    if(lvalue > N || lvalue < 1 || rvalue > N || rvalue < 1)
        suicide1();
    int res = (depth[lvalue] < depth[rvalue]) ?lvalue :rvalue;
    // printf("\nl = %d, next = %d\n", l, next);
    // printf("pow2[lg] = %d\n", pow_2[lg]);
    // printf("lv = %d, rv = %d\n", lvalue, rvalue);

    return res;
}

void print_lca(){
    for(int i=1; i<=N; i++)
        for(int j=1; j<=N; j++)
            printf("LCA(%d, %d) = %d\n", i, j, query(i, j));
}

void solve(){
    get_graph();
    preprocessing();

    for(int i=0; i<Q; i++){
        int city = next_int(), res;

        int lca1 = query(city, R), lca2 = query(city, S), lca3 = query(R, S);
        if(lca1 > lca2) swap(&lca1, &lca2);
        if(lca1 > lca3) swap(&lca1, &lca3);
        if(lca2 > lca3) swap(&lca2, &lca3);

        // printf("%d, %d, %d\n", lca1, lca2, lca3);
        res = lca1 + lca3 - lca2;
        printf("%d\n", res);
    }
}

int main()
{
    N = next_int(), Q = next_int(), S = next_int(), R = next_int();

    solve();
    if(DEBUG){
        print_graph();
        print_arr();
        print_table();
        print_lca();
    }
}









