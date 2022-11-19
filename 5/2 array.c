#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>
// #include "price.h"

#define MOD ((int) 1e7 + 9)
#define LARGER(a) (a >= MOD)
#define MAX_A 1124
#define MAX_N 1124
#define MAX_K 1100000
#define MAX_DAY 1e9
#define TOP 1
#define LEFT(a) (a * 2)
#define RIGHT(a) (a * 2 + 1)
#define PARENT(a) (a / 2)
#define CHECK 0
#define DETAIL 0
#define GET_MIN 1
#define GET_MAX 0


typedef long long int ll;
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

unsigned long long price(unsigned long long s, unsigned long long t);

int A, Q, N;
int heap_size, poped_index, max_k_index, order_clock = 0, day_table[MAX_A * MAX_N], S[MAX_A];
ll heap[MAX_A * MAX_N], order[MAX_A * MAX_N], index_table[MAX_A * MAX_N], next_table[MAX_A * MAX_N];  // price, in priority Q
ll result[MAX_K];

int hash_small[MOD] = {0}, hash_large[MOD] = {0};

/*
Range of index: 0 ~ AN-1
*/

int get_index(int pos){ return index_table[pos];}
int get_day(int index){ return (day_table[index] - 1) * N + (index % N) + 1;}
int next_day(int index){ return day_table[index] + 1;}
void day_inc(int index){ day_table[index] = next_day(index);}
int get_s(int index){ return S[index / N];}
ll get_price(int index){
    if(get_day(index) <= MAX_DAY)
        return price(get_s(index), get_day(index));
    return -1;
}
ll get_next_price(int index){ return price(get_s(index), next_day(index));}

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
            printf("p: %lld, s: %d, order: %lld; ", heap[i], S[index_table[i]], order[i]);
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
    for(int i = 0; i < A * N; i++){
        heap[i + 1] = get_price(i), order[i + 1] = ++order_clock,
        index_table[i + 1] = i, next_table[i + 1] = get_next_price(i);
        day_inc(i);
    }
    heap_size = A * N;

    for(int i = heap_size / 2; i >= TOP; i--)
        heapify(i);
}

void insert(int index, ll p){
    if(CHECK)
        printf("insert (%d, %d) = %lld\n", get_s(index), get_day(index), p);

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

    poped_index = index_table[TOP];
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
            printf("After insertion:\n");
            print_heap();
        }

    }
}

// From here is for subtask 5 & 6
int cur_find_k;

int paeh_size, poped_index_paeh, order_clock_paeh = 0, day_paeh[MAX_N];
ll paeh[MAX_N], order_paeh[MAX_N], index_paeh[MAX_N], next_paeh[MAX_N];  // price, in priority Q

int get_day_paeh(int index);
ll price_paeh(int s, int index);
ll next_price_paeh(int s, int index);

void swap_paeh(int a, int b);
int cmp_paeh(int a, int b);
void paehify(int index);
void build_paeh(int s);
void insert_paeh(int s, int index, ll p);
ll pop_paeh();

ll reduction_result[MAX_K];

void reduction(int s, int k){
    // Use the same algorithm as subtask4

}

ll query_subtask6(int s, int k){
    // index from 0 ~ N

    for(int index = 0; index <= N; index++){
        day_paeh[index] = 1;
    }
    build_paeh(s);

    ll res;
    for(int i=0; i<k; i++){

        res = pop_paeh();

        ll p = price_paeh(s, poped_index_paeh);
        if(get_day_paeh(poped_index) <= MAX_DAY)
            insert_paeh(s, poped_index_paeh, p);
    }
    return res;
}

ll recur(int new_stock, int st1, int st2, int k){
    // printf("st1 = %d, st2 = %d, k = %d\n", st1, st2, k);

    // Since we already handle the extreme case: one's min > one's max
    assert(st1 <= MAX_DAY && st2 <= cur_find_k);

    if(k == 1)
        return (price(new_stock, st1) < result[st2]) ?price(new_stock, st1) :result[st2];

    // Now we want to find which is smaller:
    // k/2 th of arr1 or arr2?
    // If arr1[k/2] is smaller, then there are less than 2 * (k/2) elements smaller;
    // i.e. it can't be kth element. Vice versa.
    int cur = k / 2, size = cur_find_k - st2 + 1;
    // printf("cur = %d\n", cur);

    // if any of arr (in this case, only result) has < k/2 size, then directly use last one.
    if(size < cur){
        ll ele1 = price(new_stock, st1 + cur - 1), ele2 = result[cur_find_k];
        if(ele1 < ele2)
            return recur(new_stock, st1 + cur, st2, k - cur);
        else  // There are k - size left to be found
            return price(new_stock, st1 + k - size - 1);
    }

    ll half1 = price(new_stock, st1 + cur - 1), half2 = result[st2 + cur - 1];
    // printf("half1 (%d) = %lld; half2 (%d) = %lld\n", st1 + cur - 1, half1, st2 + cur - 1, half2);
    if(half1 < half2)
        return recur(new_stock, st1 + cur, st2, k - cur);
    else
        return recur(new_stock, st1, st2 + cur, k - cur);
}

ll query_subtask5(int new_stock, int k){

    ll new_min = price(new_stock, 1);

    // Optimized
    if(new_min > result[k])
        return result[k];
    if(price(new_stock, k) < result[1])
        return price(new_stock, k);

    return recur(new_stock, 1, 1, k);
}

ll query(){

    int new_stock = next_int(), k = next_int();
    cur_find_k = k;

    if(k > max_k_index)
        find_new_res(k - max_k_index);

    if(new_stock == 0)
        return result[k];
    if(LARGER(new_stock) && hash_large[new_stock % MOD])
        return result[k];
    if(!LARGER(new_stock) && hash_small[new_stock % MOD])
        return result[k];

    if(N == 1)
        return query_subtask5(new_stock, k);
    else
        return query_subtask6(new_stock, k);
}

void init(){
    max_k_index = 0;

    for(int index = 0; index < N * A; index++){
        day_table[index] = 1;
    }
    build_heap();

    if(CHECK){
        printf("Initail heap:\n");
        print_heap();
    }
}

void solve_subtask1(){
    for(int i=0; i<Q; i++){
        int x = next_int();
        printf("%lld\n", price(S[0], next_int()));
    }
}

void solve(){
    if(A == 1 && N == 1){
        solve_subtask1();
        return;
    }
    init();
    for(int i=0; i<Q; i++)
        printf("%lld\n", query());
}

int main()
{
    A = next_int(), Q = next_int(), N = next_int();

    for(int i=0; i<A; i++){
        S[i] = next_int();
        if(S[i] >= MOD)
            hash_large[S[i] % MOD] = 1;
        else
            hash_small[S[i] % MOD] = 1;
    }

    solve();
}

unsigned long long price(unsigned long long s, unsigned long long t){
	unsigned long long p, x = (s ^ (s >> 30)) * 0xbf58476d1ce4e5b9ULL;
	p = (s - 1) / 1000000ULL + 1ULL;
	x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
	x = x ^ (x >> 31);
	unsigned long long la = 0, lb = 0, ra = 0xffffffffffefULL, rb = 0xffffffffffefULL,
		ma, mb, na, nb, y = x, ta = (t > (p >> 1)) ? (t - (p >> 1)) : 0, tb = t + (p >> 1), tr = t / p;

	for(int i = 28; i >= 1; i -= 3){
		ma = la + (((ra - la) * (x >> 48)) >> 16);
		mb = lb + (((rb - lb) * (y >> 48)) >> 16);
		if((1ULL << (i + 2)) & ta) la = ma;
		else ra = ma;
		if((1ULL << (i + 2)) & tb) lb = mb;
		else rb = mb;
		ma = la + (((ra - la) * ((x & 0xffff00000000ULL) >> 32)) >> 16);
		mb = lb + (((rb - lb) * ((y & 0xffff00000000ULL) >> 32)) >> 16);
		if((1ULL << (i + 1)) & ta) la = ma;
		else ra = ma;
		if((1ULL << (i + 1)) & tb) lb = mb;
		else rb = mb;
		ma = la + (((ra - la) * ((x & 0xffff0000ULL) >> 16)) >> 16);
		mb = lb + (((rb - lb) * ((y & 0xffff0000ULL) >> 16)) >> 16);
		x = (x >> 21) ^ (ta & (1ULL << i)) ^ (x * 0xc0ffee123456789ULL);
		y = (y >> 21) ^ (tb & (1ULL << i)) ^ (y * 0xc0ffee123456789ULL);
		if((1ULL << i) & ta) la = ma;
		else ra = ma;
		if((1ULL << i) & tb) lb = mb;
		else rb = mb;
	}

	ma = la + (((ra - la) * (x >> 48)) >> 16);
	mb = lb + (((rb - lb) * (y >> 48)) >> 16);
	if(1ULL & ta) la = ma;
	else ra = ma;
	if(1ULL & tb) lb = mb;
	else rb = mb;
	y = (y ^ (y >> 30)) * 0xbf58476d1ce4e5b9ULL;
	x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
	x = x ^ (y << 13);

	return 	la + tr + (((lb - la) * (x >> 48)) >> 16);
}

int get_day_paeh(int index){ return (day_paeh[index] - 1) * N + (index - 1) % N + 1;}

ll price_paeh(int s, int index){
    if(index == 0)
        return result[day_paeh[index]];
    else if(get_day_paeh(index) <= MAX_DAY)
        return price(s, get_day_paeh(index));
    else
        return -1;
}

ll next_price_paeh(int s, int index){
    if(index == 0)
        return result[day_paeh[index] + 1];
    else if(get_day_paeh(index) + N <= MAX_DAY)
        return price(s, get_day_paeh(index) + N);
    else
        return -1;
}

void swap_paeh(int a, int b){
    swap(&(paeh[a]), &(paeh[b]));
    swap(&(order_paeh[a]), &(order_paeh[b]));
    swap(&(index_paeh[a]), &(index_paeh[b]));
    swap(&(next_paeh[a]), &(next_paeh[b]));
}

int cmp_paeh(int a, int b){
    if(paeh[a] > paeh[b]) return 1;
    if(paeh[a] < paeh[b]) return -1;
    if(next_paeh[a] > next_paeh[b]) return 1;
    if(next_paeh[a] < next_paeh[b]) return -1;
    if(order_paeh[a] > order_paeh[b]) return 1;
    if(order_paeh[a] < order_paeh[b]) return -1;
    return 0;
}

void paehify(int index){
    int cur = index;
    while(cur <= paeh_size){
        int left = LEFT(cur), right = RIGHT(cur), next = cur;
        if(left <= paeh_size)
            if(cmp_paeh(left, cur) == -1)
                next = left;
        if(right <= paeh_size)
            if(cmp_paeh(right, next) == -1)
                next = right;

        if(next == cur)
            break;
        swap_paeh(cur, next);
        cur = next;
    }
}

void insert_paeh(int s, int index, ll p){
    day_paeh[index]++;
    paeh_size++;
    paeh[paeh_size] = p, order_paeh[paeh_size] = ++order_clock_paeh,
    index_paeh[paeh_size] = index, next_paeh[paeh_size] = next_price_paeh(s, index);

    int cur = PARENT(paeh_size);
    while(cur >= TOP){
        paehify(cur);
        cur = PARENT(cur);
    }
}

ll pop_paeh(){
    ll min_p = paeh[TOP];

    poped_index_paeh = index_paeh[TOP];
    swap_paeh(TOP, paeh_size--);
    paehify(TOP);

    return min_p;
}

void build_paeh(int s){
    for(int i = 0; i <= N; i++){
        int pos = i + 1;
        paeh[pos] = price_paeh(s, i), order_paeh[pos] = ++order_clock_paeh,
        index_paeh[pos] = i, next_paeh[pos] = next_price_paeh(s, i);
        day_paeh[i]++;
    }
    paeh_size = N + 1;

    for(int i = paeh_size / 2; i >= TOP; i--)
        paehify(i);
}

