// Optimized ver.

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>
#include "price.h"

#include <time.h>

#define PRINT_TIME 0

#define MIN(a, b) ((a < b) ?a :b)
#define MAX(a, b) ((a > b) ?a :b)
#define MOD ((int) 1e9 / 2)
#define MAX_A 1224
#define MAX_N 1224
#define MAX_K 1200000
#define MAX_DAY 1e9
#define TOP 1
#define LEFT(a) (a * 2)
#define RIGHT(a) (a * 2 + 1)
#define PARENT(a) (a / 2)
#define CHECK 0
#define DETAIL 0
#define LESS 0
#define GREATER 1
#define THIS_SHOULD_NOT_HAPPEN 0

typedef long long int ll;

clock_t StartTime;

void SuicideRE(){
    int *a = NULL;
    *a = 1;
}
void SuicideTLE(){
    while(1) printf("1");
}
void SuicideSE(){
    system("pause");
}

void swap(ll *a, ll *b){
    *b = *a + *b - (*a = *b);
}

int next_int(){
    int c = getchar(), num = 0;
    while(!isdigit(c))
        c = getchar();
    while(isdigit(c))
        num = num * 10 + c - '0', c = getchar();
    return num;
}

// unsigned long long price(unsigned long long s, unsigned long long t);

int A, Q, N;
int heap_size, poped_index, max_k_index, day_table[MAX_A * MAX_N], S[MAX_A];
ll heap[MAX_A * MAX_N], index_table[MAX_A * MAX_N];  // price, in priority Q
ll result[MAX_K];

/*
Range of index: 0 ~ AN-1
*/

int get_index(int pos){ return index_table[pos];}
int get_day(int index){
    return (day_table[index] - 1) * N + (index % N) + 1;
}
int next_day(int index){ return day_table[index] + 1;}
void day_inc(int index){ day_table[index] = next_day(index);}
int get_s(int index){ return S[index / N];}
ll get_price(int index){
    if(get_day(index) <= MAX_DAY)
        return price(get_s(index), get_day(index));
    return -1;
}

void swap_heap(int a, int b){
    swap(&(heap[a]), &(heap[b]));
    swap(&(index_table[a]), &(index_table[b]));
}

int cmp_heap(int a, int b){
    if(heap[a] > heap[b])
        return 1;
    else
        return -1;
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
        day_table[i] = 1, index_table[i + 1] = i,
        heap[i + 1] = get_price(i), day_table[i]++;
    }

    heap_size = A * N;

    for(int i = heap_size / 2; i >= TOP; i--)
        heapify(i);
}

void insert(int index, ll p){

    day_inc(index);
    heap_size++;
    heap[heap_size] = p,
    index_table[heap_size] = index;

    int cur = heap_size;
    while(PARENT(cur) >= TOP){
        if(cmp_heap(cur, PARENT(cur)) == -1)
            swap_heap(cur, PARENT(cur));
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
    for(int i=0; i<k; i++){

        result[++max_k_index] = pop();

        int day = get_day(poped_index);
        if(day <= MAX_DAY && day < k + N){
            ll p = get_price(poped_index);
            insert(poped_index, p);
        }
    }
}

void init(){
    max_k_index = 0;
    build_heap();
}


// From here is sub5
int cur_find_k;
ll recur(int new_stock, int st1, int st2, int k){
    // Since we already handle the extreme case: one's min > one's max
    assert(st1 <= MAX_DAY && st2 <= cur_find_k);

    if(k == 1)
        return (price(new_stock, st1) < result[st2]) ?price(new_stock, st1) :result[st2];

    // Now we want to find which is smaller:
    // k/2 th of arr1 or arr2?
    // If arr1[k/2] is smaller, then there are less than 2 * (k/2) elements smaller;
    // i.e. it can't be kth element. Vice versa.
    int cur = k / 2, size = cur_find_k - st2 + 1;

    // if any of arr (in this case, only result) has < k/2 size, then directly use last one.
    if(size < cur){
        ll ele1 = price(new_stock, st1 + cur - 1), ele2 = result[cur_find_k];
        if(ele1 < ele2)
            return recur(new_stock, st1 + cur, st2, k - cur);
        else  // There are k - size left to be found
            return price(new_stock, st1 + k - size - 1);
    }

    ll half1 = price(new_stock, st1 + cur - 1), half2 = result[st2 + cur - 1];
    if(half1 < half2)
        return recur(new_stock, st1 + cur, st2, k - cur);
    else
        return recur(new_stock, st1, st2 + cur, k - cur);
}

ll query_subtask5(int new_stock, int k){
    cur_find_k = k;
    ll new_min = price(new_stock, 1);

    // Optimized
    if(new_min > result[k])
        return result[k];
    if(price(new_stock, k) < result[1])
        return price(new_stock, k);

    return recur(new_stock, 1, 1, k);
}


// From here is sub6

ll Select(ll Arr[], int p, int q, int k){
    if(k == 1){
        ll min = Arr[p];
        for(int i = p + 1; i <= q; i++)
            min = MIN(min, Arr[i]);
        return min;
    }
    if(k == q - p + 1){
        ll max = Arr[p];
        for(int i = p + 1; i <= q; i++)
            max = MAX(max, Arr[i]);
        return max;
    }

    int Index = p - 1, Count;
    for(int i = p; i < q; i++)
        if(Arr[i] <= Arr[q])
            swap(&(Arr[++Index]), &(Arr[i]));
    swap(&(Arr[++Index]), &(Arr[q]));
    Count = Index - p + 1;
    if(Count == k)
        return Arr[Index];
    else if(Count < k)
        return Select(Arr, Index + 1, q, k - Count);
    else
        return Select(Arr, p, Index - 1, k);
}

ll Arr2[MAX_K + MAX_N];

ll FindKthOfS(int s, int k){
    int i, j;
    ll Arr[MAX_N * 5];
    for(i = MAX(0, k - N + 1), j = 1; i <= k + N - 1; i++, j++)
        Arr[j] = price(s, i);

    ll res1 = Select(Arr, 1, j - 1, MIN(N, k));
    return res1;
}
int ccc = 0;
ll Recur6(int s, int st1, int st2, int k){

    int debug = 1;
    ccc++;
    // if(ccc > 1) return 0;

    if(k == 1)
        return (FindKthOfS(s, st1) < result[st2]) ?FindKthOfS(s, st1) :result[st2];

    // Now we want to find which is smaller:
    // k/2 th of arr1 or arr2?
    // If arr1[k/2] is smaller, then there are less than 2 * (k/2) elements smaller;
    // i.e. it can't be kth element. Vice versa.
    int cur = k / 2, size = cur_find_k - st2 + 1;

    // if any of arr (in this case, only result) has < k/2 size, then directly use last one.
    if(size < cur){
        ll ele1 = FindKthOfS(s, st1 + cur - 1), ele2 = result[cur_find_k];
        if(ele1 < ele2)
            return Recur6(s, st1 + cur, st2, k - cur);
        else  // There are k - size left to be found
            return FindKthOfS(s, st1 + k - size - 1);
    }

    ll half1 = FindKthOfS(s, st1 + cur - 1), half2 = result[st2 + cur - 1];

    if(half1 < half2)
        return Recur6(s, st1 + cur, st2, k - cur);
    else
        return Recur6(s, st1, st2 + cur, k - cur);
}


ll QuerySub6(int s, int k){
    cur_find_k = k;
    return Recur6(s, 1, 1, k);
}

ll query(){
    int new_stock = next_int(), k = next_int();

    if(max_k_index == 0)
        find_new_res(1000010);

    if(new_stock == 0)
        return result[k];
    else if(N == 1)
        return query_subtask5(new_stock, k);
    else
        return QuerySub6(new_stock, k);
}

void solve_subtask1(){
    // A trivial case
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
    StartTime = clock();

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


