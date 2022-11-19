#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>
#include "price.h"

#define MAX_A 1025
#define INF 10000
#define LEFT(a) (2 * a)
#define RIGHT(a) (2 * a + 1)
#define MAX(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })
#define MIN(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })


typedef long long int ll;

typedef struct pair{
    int num, p;
} pair;

pair *make_pair(int num, int p){
    pair *tmp = (pair *) malloc(sizeof(pair));
    tmp->num = num, tmp->p = p;
    return tmp;
}

int A, Q, N, S[MAX_A];

// ll price(int s, int t){return t; }

void clear(){
    int c;
    while((c = getchar()) != EOF);
}

int next_int(){
    int c = getchar(), num = 0;
    while(!isdigit(c))
        c = getchar();
    while(isdigit(c))
        num = num * 10 + c - '0', c = getchar();
    return num;
}

void solve_subtask1(){
    /*
    Since N = 1, p(t) are strictly increasing
    Hence k-th smallest is p(t = k)
    */
    int new_stock = next_int(), k = next_int();
    return;
    price(S[0], k);
}

typedef struct node{
    ll value;
    struct node *next, *prev;
} node;

typedef struct stock{
    node *head, *tail;
    int count;
    ll p;
} stock;

typedef struct heap{
    stock *arr[MAX_A];
    int size;
} heap;

stock *build_stock(int num, int p){
    stock *tmp = (stock *) malloc(sizeof(stock));
    tmp->count = 1, tmp->p = p;
    tmp->head = tmp->tail = (node *) malloc(sizeof(node));
    tmp->head->value = num, tmp->head->next = tmp->head->prev = NULL;
    return tmp;
}

void print_heap(heap *H){
    printf("heap: (size = %d)\n", H->size);
    for(int i = 1; i <= H->size; i++){
        printf("p: %d, count: %d\n", H->arr[i]->p, H->arr[i]->count);
        node *cur = H->arr[i]->head;
        while(cur != NULL){
            printf("%d -> ", cur->value, S[cur->value]);
            cur = cur->next;
        }
        printf("NULL\n");
    }
}

void swap_heap(heap *H, int a, int b){
    stock *tmp = H->arr[a];
    H->arr[a] = H->arr[b];
    H->arr[b] = tmp;
}

void merge_heap(heap *H, int cur, int next){
    // merge cur to next
    assert(H->arr[cur]->p == H->arr[next]->p);
    H->arr[next]->count += H->arr[cur]->count;
    H->arr[next]->tail->next = H->arr[cur]->head;
    H->arr[cur]->head->prev = H->arr[next]->tail;
    H->arr[next]->tail = H->arr[cur]->tail;
}

void heapify(heap *H, int index){
    int cur = index;

    while(cur <= H->size){
        int left = LEFT(cur), right = RIGHT(cur), next = cur, merge = 0;

        if(left <= H->size)
            if(H->arr[left]->p == H->arr[cur]->p)
                merge = 1, next = left;
            else if(H->arr[left]->p < H->arr[cur]->p)
                next = left;
        if(right <= H->size)
            if(H->arr[right]->p == H->arr[cur]->p)
                merge = 1, next = right;
            else if(!merge && H->arr[right]->p < H->arr[next]->p)
                next = right;

        if(merge){
            merge_heap(H, cur, next);
            H->arr[cur] = H->arr[H->size--];
            // printf("cur = %d:\n", cur);
            // print_heap(H);
        }
        else{
            if(next == cur)
                break;
            swap_heap(H, cur, next);
            cur = next;
        }
    }
    return;
}

heap *build_heap(stock *arr[], int size){
    heap *H = (heap *) malloc(sizeof(heap));
    for(int i = 0; i < size; i++){
        H->arr[i + 1] = arr[i];  // start from 1
        printf("H: %d, arr: %d\n", H->arr[i + 1]->count, arr[i]->count);
    }
    H->size = size;
    for(int i=1; i<= H->size; i++){
        printf("%d ", H->arr[i]->count);
    }
    printf("\n");

    printf("Totally not heapify:\n");
    print_heap(H);

    for(int i = H->size / 2; i >= 1; i--){
        heapify(H, i);
    }
    return H;
}

void insert(heap *H, stock *key){
    H->size++;

    H->arr[H->size] = key;
    swap_heap(H, 1, H->size);
    // printf("before heapify:\n");
    // print_heap(H);

    heapify(H, 1);
    // printf("after heapify:\n");
    // print_heap(H);
}

pair *pop(heap *H){
    assert(H->arr[1]->tail != NULL);

    int p = H->arr[1]->p;
    node *cur = H->arr[1]->tail;

    H->arr[1]->count--;
    H->arr[1]->tail = cur->prev;
    if(H->arr[1]->count == 0){
        assert(H->arr[1]->tail == NULL);
        H->arr[1] = H->arr[H->size--];
        heapify(H, 1);
        // return make_pair(0, 0);
    }
    else
        H->arr[1]->tail->next = NULL;

    pair *res = make_pair(cur->value, p);
    free(cur);
    return res;
}

ll query(heap *H, int new_stock, int k, int *day){
    ll res;
    // printf("Initial:\n");
    // print_heap(&H);

    for(int i = 0; i < k; i++){
        // printf("size = %d\n", H.size);
        pair *tmp = pop(H);
        // printf("num\n", tmp.num);
        // printf("\nround %d:\n", i+1);
        // printf("p = %lld\n", tmp->p);
        res = tmp->p;

        // printf("before insertion:\n");
        // print_heap(H);

        printf("num = %d, day = %d\n", tmp->num, day[tmp->num]);
        insert(H, build_stock(tmp->num, price(S[tmp->num], ++day[tmp->num])));
        printf("after insertion:\n");
        print_heap(H);
        // printf("res = %d\n", res);
    }
    return res;
}

void solve_subtask2(){
    int *day = (int *) malloc(sizeof(int) * A);
    stock **arr = (stock **) malloc(sizeof(stock *) * A);
    for(int i=0; i<A; i++){
        day[i] = 1;
        arr[i] = build_stock(i, price(S[i], 1));
        printf("count(%d) = %d\n", arr[i]->p, arr[i]->count);
        arr[i]->count = 1;
        printf("now it's %d\n", arr[i]->count);
    }

    heap *H = build_heap(arr, A);

    printf("Initial:\n");
    print_heap(H);

    for(int i=0; i<Q; i++){
        int new_stock = next_int(), k = next_int();
        printf("res = %lld\n", query(H, new_stock, k, day));
    }
}

void solve(){
    if(A == 1)
        solve_subtask1();
    else if(N == 1)
        solve_subtask2();
    else
        return;
}


int main()
{
    A = next_int(), Q = next_int(), N = next_int();

    for(int i=0; i<A; i++)
        S[i] = next_int();

    solve();
    next_int();
}





