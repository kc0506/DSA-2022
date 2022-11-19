#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>
// #include "price.h"

#define MIN(a, b) ((a < b) ?a :b)
#define MAX(a, b) ((a > b) ?a :b)
#define MOD ((int) 1e7 + 9)
#define MAX_A 1124
#define MAX_N 1124
#define MAX_K 1100000
#define MAX_DAY 1e9
#define TOP 1
#define LEFT(a) (a * 2)
#define RIGHT(a) (a * 2 + 1)
#define PARENT(a) (a / 2)
#define GP(a) (PARENT(PARENT(a)))
#define LGC(a) (LEFT(LEFT(a)))
#define GGC(a) (RIGHT(RIGHT(a)))
#define CHECK 0
#define DETAIL 0
#define GET_MIN 1
#define GET_MAX 0
#define MIN_LEVEL 0
#define MAX_LEVEL 1
#define LESS 0
#define GREATER 1
#define FIRST 0
#define SECOND 1
#define LHALF 0
#define GHALF 1
#define GHALF2 2

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

unsigned long long price(unsigned long long s, unsigned long long t);
// ll price(int s, int t){return s + t;}

int A, Q, N;
int max_index, heap_size, poped_index, max_k_index, order_clock = 0, day_table[MAX_A * MAX_N], S[MAX_A];
ll heap[MAX_A * MAX_N], order[MAX_A * MAX_N], index_table[MAX_A * MAX_N], next_table[MAX_A * MAX_N];  // price, in priority Q
ll result[MAX_K];

int hash_small[MOD] = {0}, hash_large[MOD] = {0};

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
ll get_next_price(int index){ return price(get_s(index), next_day(index));}

void print_day(){
    for(int i = 0; i < N * A; i++)
        printf("%d%c", day_table[i] - 1, " \n"[i == A * N - 1]);
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

int not_closed, st[MAX_N], end[MAX_N], mid[MAX_N];
ll sum_deap;

int get_day_deap(int index, int day){ return (day - 1) * N + (index - 1) % N + 1;}  // 6
int get_mid_index_deap(int index){
    if(st[index] == end[index])
        return st[index];
    return st[index] + mid[index] - 1;
}
ll get_price_deap(int s, int index, int day){
    if(index == 0)
        return result[day];
    return price(s, get_day_deap(index, day));
}

// Deap
int deap_size, clock_deap = 0, level_deap[MAX_N];
ll deap[MAX_N], order_deap[MAX_N], index_deap[MAX_N];

void print_deap();
void swap_deap(int a, int b);
int cmp_deap(int a, int b);
void search(int index);
void update(int index, int cut);

void init_deap();
void verify_deap(int index);
void insert_deap(int s, int index);
int pop_min_deap();
int pop_max_deap();

// D&C

ll recur_sub6(int s, int k){

    if(not_closed == 1)
        for(int i=0; i<=N; i++)
            if(st[i] <= end[i])
                return get_price_deap(s, i, st[i] + k - 1);

    int index, cut, check = 0;

    if(sum_deap < k)
        index = pop_min_deap(), k = k - mid[index], cut = LHALF;
    else if(sum_deap > k)
        index = pop_max_deap(), cut = GHALF;
    else
        index = pop_max_deap(), cut = GHALF2;


    if(cut == GHALF2 && st[index] == end[index]){
        ll cur = get_price_deap(s, index, get_mid_index_deap(index));

        end[index]--, not_closed--, mid[index] = 0;
        sum_deap--;

        ll redu = recur_sub6(s, k);
        return MIN(cur, redu);
    }
    update(index, cut);
    insert_deap(s, index);

    return recur_sub6(s, k);
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

ll query_subtask6(int s, int k){

    // Optimized
    int is_min = 1;
    for(int i = 1; i <= N; i++)
        if(get_price_deap(s, i, 1) < result[k]);
            is_min = 0;
    if(is_min)
        return result[k];

    for(int i = 0; i <= N; i++)
        st[i] = 1;

    end[0] = k;
    int thresh = (MAX_DAY / N) * N;
    for(int i = 1; i <= N; i++)
        if(get_day_deap(i, k) <= MAX_DAY)
            end[i] = k;
        else if(i + thresh <= MAX_DAY)
            end[i] = (MAX_DAY / N) + 1;
        else
            end[i] = (MAX_DAY / N);

    for(int i=0; i<=N; i++)
        mid[i] = (end[i] - st[i] + 1) / 2;

    sum_deap = 0;
    for(int i=0; i<=N; i++)
        sum_deap += mid[i];

    not_closed = N + 1;

    init_deap();
    for(int i = 0; i <= N; i++)
        insert_deap(s, i);

    return recur_sub6(s, k);
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

    if(new_stock == 0 || hash_large[new_stock % MOD] || hash_small[new_stock % MOD])
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


void print_deap(){
    printf("size = %d\n", deap_size);
    int i=1, b=2;
    for(; i <= deap_size; i++){
        if(i == b){
            printf("\n");
            b *= 2;
        }
        if(level_deap[i] == MIN_LEVEL)
            printf("(MIN) ");
        if(level_deap[i] == MAX_LEVEL)
            printf("(MAX) ");
        printf("%lld ", deap[i]);
    }
    printf("\n");
}

void swap_deap(int a, int b){
    swap(&(deap[a]), &(deap[b]));
    swap(&(order_deap[a]), &(order_deap[b]));
    swap(&(index_deap[a]), &(index_deap[b]));
}

int cmp_deap(int a, int b){
    if(deap[a] > deap[b]) return GREATER;
    if(deap[a] < deap[b]) return LESS;
    if(order_deap[a] > order_deap[b]) return GREATER;
    if(order_deap[a] < order_deap[b]) return LESS;
    return -1;
}

void search(int index){
    printf("Searching %d\n", index);
    printf("size = %d\n", deap_size);
    for(int i=TOP; i<=deap_size; i++)
        if(index_deap[i] == index){
            printf("%d, %lld\n", i, deap[i]);
            printf("parent: %lld, left=%d: %lld, right: %lld\n", deap[PARENT(i)],LEFT(i), deap[LEFT(i)], deap[RIGHT(i)]);
        }
    printf("\n");
}

void update(int index, int cut){

    int check = 0;

    if(check)
        printf("b: (%d, %d) %d\n", st[index], end[index], mid[index]);

    if(st[index] > end[index])
        return;

    int cur_mid = mid[index];

    if(cut == LHALF)
        st[index] = get_mid_index_deap(index) + 1;

    if(cut == GHALF)
        end[index] = get_mid_index_deap(index) - 1;

    if(cut == GHALF2)
        end[index] = get_mid_index_deap(index);

    mid[index] = (end[index] - st[index] + 1) / 2;
    if(mid[index] == 0 && st[index] == end[index])
        mid[index] = 1;

    sum_deap = sum_deap - cur_mid + mid[index];

    if(st[index] > end[index])
        not_closed--;

    if(check)
        printf("a: (%d, %d) %d\n", st[index], end[index], mid[index]);
}

void init_deap(){
    clock_deap = 0;
    deap_size = 0;
    level_deap[0] = MAX_LEVEL;
}

void verify_deap(int index){

    int check = 0;
    if(check){
        printf("before:\n");
        printf("index = %d\n", index);
        printf("level = %d\n", level_deap[index]);
        printf("self: %lld\n", deap[index]);
        printf("parent: %lld\n", deap[PARENT(index)]);
        printf("GP: %lld\n", deap[GP(index)]);
    }

    if(PARENT(index) < TOP)
        return;

    // May not be necessary
    if(cmp_deap(index, PARENT(index)) == !level_deap[index]){
        swap_deap(index, PARENT(index));
        verify_deap(PARENT(index));
    }
    if(GP(index) >= TOP)
        if(cmp_deap(index, GP(index)) == level_deap[index]){
            swap_deap(index, GP(index));
            verify_deap(GP(index));
        }

    if(check){
        printf("after:\n");
        printf("index = %d\n", index);
        printf("level = %d\n", level_deap[index]);
        printf("self: %lld\n", deap[index]);
        printf("parent: %lld\n", deap[PARENT(index)]);
        printf("GP: %lld\n", deap[GP(index)]);
    }
}

void insert_deap(int s, int index){

    if(st[index] > end[index])
        return;

    deap_size++;
    level_deap[deap_size] = !(level_deap[PARENT(deap_size)]);
    deap[deap_size] = get_price_deap(s, index, get_mid_index_deap(index)),
    order_deap[deap_size] = ++clock_deap,
    index_deap[deap_size] = index;


    if(PARENT(deap_size) < TOP)
        return;

    if(cmp_deap(deap_size, PARENT(deap_size)) == level_deap[PARENT(deap_size)]){

        swap_deap(deap_size, PARENT(deap_size));
        verify_deap(PARENT(deap_size));
    }
    else
        verify_deap(deap_size);
}

int pop_min_deap(){

    int res = index_deap[TOP], cur = TOP;

    if(0){
        printf("value = %lld\n", deap[TOP]);
        printf("%d: (%d, %d) %d\n", res, st[res], end[res], mid[res]);
    }

    swap_deap(TOP, deap_size--);

    while(cur <= deap_size){
        if(LEFT(cur) > deap_size){
            break;
        }
        if(LGC(cur) > deap_size){
            int min_index = cur;
            if(cmp_deap(LEFT(cur), min_index) == LESS)
                min_index = LEFT(cur);
            if(RIGHT(cur) <= deap_size)
                if(cmp_deap(RIGHT(cur), min_index) == LESS)
                    min_index = RIGHT(cur);
            swap_deap(min_index, cur);
            break;
        }
        else{
            int min_gc = LGC(cur);
            for(int index = LGC(cur); index <= GGC(cur); index++)
                if(cmp_deap(index, min_gc) == LESS && index <= deap_size)
                    min_gc = index;
            if(LEFT(RIGHT(cur)) > deap_size)
                if(cmp_deap(RIGHT(cur), min_gc) == LESS){
                    swap_deap(cur, RIGHT(cur));
                    break;
                }
            if(cmp_deap(cur, min_gc) == GREATER)
                swap_deap(cur, min_gc);
            if(cmp_deap(min_gc, PARENT(min_gc)) == GREATER)
                swap_deap(min_gc, PARENT(min_gc));
            cur = min_gc;
        }
    }

    return res;
}

int pop_max_deap(){

    if(deap_size == 1 || deap_size == 2)
        return index_deap[deap_size--];

    int max_index = LEFT(TOP);
    if(cmp_deap(RIGHT(TOP), max_index) == GREATER)
        max_index = RIGHT(TOP);

    int res = index_deap[max_index], cur = max_index;

    int check = 0;

    if(check){
        printf("pos: %d\n", max_index);
    }

    swap_deap(max_index, deap_size--);
    while(cur <= deap_size){
        if(check)
            printf("cur = %d\n", cur);

        if(LEFT(cur) > deap_size){
            if(check)printf("break1\n");
            break;
        }
        if(LGC(cur) > deap_size){
            if(check)printf("break2\n");
            int max_index = cur;
            if(cmp_deap(LEFT(cur), max_index) == GREATER)
                max_index = LEFT(cur);
            if(RIGHT(cur) <= deap_size)
                if(cmp_deap(RIGHT(cur), max_index) == GREATER)
                    max_index = RIGHT(cur);
            swap_deap(max_index, cur);
            break;
        }
        else{
            int max_gc = LGC(cur);
            for(int index = LGC(cur); index <= GGC(cur); index++)
                if(cmp_deap(index, max_gc) == GREATER && index <= deap_size)
                    max_gc = index;
            if(LEFT(RIGHT(cur)) > deap_size)
                if(cmp_deap(RIGHT(cur), max_gc) == GREATER){
                    swap_deap(cur, RIGHT(cur));
                    break;
                }
            if(check)
                printf("max_gc = %d\n", max_gc);
            if(cmp_deap(cur, max_gc) == LESS){
                swap_deap(cur, max_gc);
                if(check)printf("max_gc > cur, swap(%d, %d)\n", cur, max_gc);
            }
            if(cmp_deap(max_gc, PARENT(max_gc)) == LESS){
                swap_deap(max_gc, PARENT(max_gc));
                if(check)printf("now max_gc < parent, swap(%d, %d)\n", PARENT(max_gc), max_gc);
            }
            cur = max_gc;
        }
    }

    return res;
}



