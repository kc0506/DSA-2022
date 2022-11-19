#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>
#include "price.h"

#define MAX_A 1124
#define MAX_K 1000100
#define MAX_DAY 1e9
#define TOP 1
#define LEFT(a) (a * 2)
#define RIGHT(a) (a * 2 + 1)
#define PARENT(a) (a / 2)
#define CHECK 0
#define DETAIL 0

typedef long long int ll;
typedef struct node{int index; struct node *next, *prev;} node;

void swap(ll *a, ll *b){
    ll tmp = *a;
    *a = *b;
    *b = tmp;
}

int next_int(){
    int c = getchar(), num = 0;
    while(!isdigit(c))
        c = getchar();
    while(isdigit(c))
        num = num * 10 + c - '0', c = getchar();
    return num;
}

node *new_node(int index){
    node *new_node = (node *) malloc(sizeof(node));
    new_node->index = index, new_node->next = new_node->prev = NULL;
    return new_node;
}

// unsigned long long price(unsigned long long s, unsigned long long t);
// ll price(int s, int t){return s + t;}

int A, Q, N;
int heap_size, poped_index, max_k_index, order_clock = 0, day_table[MAX_A], S[MAX_A];
ll heap[MAX_A], order[MAX_A], index_table[MAX_A], next_table[MAX_A];  // price, in priority Q
ll  result[MAX_K];

int get_index(int pos){ return index_table[pos];}
int get_day(int index){ return day_table[index];}
ll get_price(int index){
    if(get_day(index) <= MAX_DAY)
        return price(S[index], get_day(index));
    return 0;
}
ll get_next_price(int index){ return price(S[index], get_day(index) + 1);}

void print_day(){
    for(int i=0; i<A; i++)
        printf("%d%c", day_table[i] - 1, " \n"[i == A - 1]);
}

void day_inc(int index){
    day_table[index]++;
}

void swap_heap(int a, int b){
    swap(&(heap[a]), &(heap[b]));
    swap(&(order[a]), &(order[b]));
    swap(&(index_table[a]), &(index_table[b]));
    swap(&(next_table[a]), &(next_table[b]));
}

void print_heap(){
    printf("----------\nsize = %d:\n", heap_size);
    int i = 1, j = 2;
    for(; i <= heap_size; i++){
        if(i == j){
            printf("\n");
            j *= 2;
        }
        if(DETAIL)
            printf("p: %lld, s: %d, order: %d; ", heap[i], S[index_table[i]], order[i]);
        else
            printf("%lld ", heap[i]);
    }
    printf("\n----------\n");
}

int cmp_heap(int a, int b){
    if(heap[a] > heap[b])
        return 1;
    if(heap[a] < heap[b])
        return -1;

    if(next_table[a] > next_table[b])
        return 1;
    if(next_table[a] < next_table[b])
        return -1;

    if(order[a] > order[b])
        return 1;
    if(order[a] < order[b])
        return -1;

    return 0;
}

void heapify(int index){
    int cur = index;

    while(cur <= heap_size){

        int left = LEFT(cur), right = RIGHT(cur), next = cur;

        if(left <= heap_size)
            if(cmp_heap(left, cur) == -1)
                next = left;
        if(right <= heap_size)
            if(cmp_heap(right, next) == -1)
                next = right;

        if(next == cur)
            break;
        swap_heap(cur, next);
        cur = next;
    }
    return;
}

void build_heap(){
    for(int i = 0; i < A; i++){
        heap[i + 1] = get_price(i), order[i + 1] = ++order_clock,
        index_table[i + 1] = i, next_table[i + 1] = get_next_price(i);
        day_inc(i);
    }
    heap_size = A;

    for(int i = heap_size / 2; i >= TOP; i--)
        heapify(i);
}

void insert(int index, ll p){
    if(CHECK)
        printf("insert (%d, %d) = %d\n", S[index], day_table[index], p);

    day_inc(index);
    heap_size++;
    heap[heap_size] = p, order[heap_size] = ++order_clock,
    index_table[heap_size] = index, next_table[heap_size] = get_next_price(index);

    int cur = PARENT(heap_size);
    while(cur >= TOP){
        heapify(cur);
        cur = PARENT(cur);
    }
}

ll pop(){
    ll min_p = heap[TOP];
    int index = index_table[TOP];

    poped_index = index;

    swap_heap(TOP, heap_size--);
    heapify(TOP);

    return min_p;
}

void find_new_res(int k){
    // wont be used twice if Q == 1

    for(int i=0; i<k; i++){

        result[++max_k_index] = pop();

        if(CHECK){
            printf("Round %d:\n", max_k_index);
            print_heap();
        }

        int day = get_day(poped_index);
        ll p = get_price(poped_index);
        if(day <= MAX_DAY)
            insert(poped_index, p);

        if(CHECK){
            printf("After insertion:\n", S[poped_index], day, p);
            print_heap();
        }

    }
}

ll query(){

    int new_stock = next_int(), k = next_int();
    if(k > max_k_index)
        find_new_res(k - max_k_index);
    return result[k];
}

void init(){
    max_k_index = 0;

    for(int i=0; i<A; i++)
        day_table[i] = 1;
    build_heap();

    if(CHECK)
        print_heap();
}

void solve_subtask1(){
    // solve_subtask2();
    // return;

    for(int i=0; i<Q; i++){
        int x = next_int();
        printf("%lld\n", price(S[0], next_int()));
    }
}

void solve_subtask2(){
    init();
    for(int i=0; i<Q; i++)
        printf("%lld\n", query());
}


void solve_subtask3(){

}

void solve(){
    if(A == 1 && N == 1)
        solve_subtask1();
    else if(N == 1)
        solve_subtask2();
    else if(A == 1 && Q == 1)
        solve_subtask3();
    else
        return;
}

int main()
{
    A = next_int(), Q = next_int(), N = next_int();

    for(int i=0; i<A; i++)
        S[i] = next_int();

    solve();
}


// unsigned long long price(unsigned long long s, unsigned long long t){
// 	unsigned long long p, x = (s ^ (s >> 30)) * 0xbf58476d1ce4e5b9ULL;
// 	p = (s - 1) / 1000000ULL + 1ULL;
// 	x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
// 	x = x ^ (x >> 31);
// 	unsigned long long la = 0, lb = 0, ra = 0xffffffffffefULL, rb = 0xffffffffffefULL,
// 		ma, mb, na, nb, y = x, ta = (t > (p >> 1)) ? (t - (p >> 1)) : 0, tb = t + (p >> 1), tr = t / p;

// 	for(int i = 28; i >= 1; i -= 3){
// 		ma = la + (((ra - la) * (x >> 48)) >> 16);
// 		mb = lb + (((rb - lb) * (y >> 48)) >> 16);
// 		if((1ULL << (i + 2)) & ta) la = ma;
// 		else ra = ma;
// 		if((1ULL << (i + 2)) & tb) lb = mb;
// 		else rb = mb;
// 		ma = la + (((ra - la) * ((x & 0xffff00000000ULL) >> 32)) >> 16);
// 		mb = lb + (((rb - lb) * ((y & 0xffff00000000ULL) >> 32)) >> 16);
// 		if((1ULL << (i + 1)) & ta) la = ma;
// 		else ra = ma;
// 		if((1ULL << (i + 1)) & tb) lb = mb;
// 		else rb = mb;
// 		ma = la + (((ra - la) * ((x & 0xffff0000ULL) >> 16)) >> 16);
// 		mb = lb + (((rb - lb) * ((y & 0xffff0000ULL) >> 16)) >> 16);
// 		x = (x >> 21) ^ (ta & (1ULL << i)) ^ (x * 0xc0ffee123456789ULL);
// 		y = (y >> 21) ^ (tb & (1ULL << i)) ^ (y * 0xc0ffee123456789ULL);
// 		if((1ULL << i) & ta) la = ma;
// 		else ra = ma;
// 		if((1ULL << i) & tb) lb = mb;
// 		else rb = mb;
// 	}

// 	ma = la + (((ra - la) * (x >> 48)) >> 16);
// 	mb = lb + (((rb - lb) * (y >> 48)) >> 16);
// 	if(1ULL & ta) la = ma;
// 	else ra = ma;
// 	if(1ULL & tb) lb = mb;
// 	else rb = mb;
// 	y = (y ^ (y >> 30)) * 0xbf58476d1ce4e5b9ULL;
// 	x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
// 	x = x ^ (y << 13);

// 	return 	la + tr + (((lb - la) * (x >> 48)) >> 16);
// }