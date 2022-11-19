#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>
// #include "price.h"

#include <time.h>

#define PRINT_TIME 1

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
#define PREV 0
#define NEXT 1
#define MIN_LEVEL 0
#define MAX_LEVEL 1
#define LESS 0
#define GREATER 1
#define THIS_SHOULD_NOT_HAPPEN 0

typedef long long int ll;
typedef struct node{int index; struct node *next, *prev;} node;

clock_t StartTime;

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

    if(PRINT_TIME) printf("Before build_heap: %lf\n", (double) (clock() - StartTime) / CLOCKS_PER_SEC);

    for(int i = 0; i < A * N; i++){
        order[i + 1] = ++order_clock,
        index_table[i + 1] = i,
        heap[i + 1] = get_price(i), next_table[i + 1] = get_next_price(i), day_table[i]++;
    }

    if(PRINT_TIME) printf("After loop 1: %lf\n", (double) (clock() - StartTime) / CLOCKS_PER_SEC);

    heap_size = A * N;

    if(PRINT_TIME) printf("Before heapify: %lf\n", (double) (clock() - StartTime) / CLOCKS_PER_SEC);


    for(int i = heap_size / 2; i >= TOP; i--)
        heapify(i);

    if(PRINT_TIME) printf("After build_heap: %lf\n", (double) (clock() - StartTime) / CLOCKS_PER_SEC);
}

void insert(int index, ll p){
    if(CHECK)
        printf("insert (%d, %d) = %lld\n", get_s(index), get_day(index), p);

    day_inc(index);
    heap_size++;
    heap[heap_size] = p, order[heap_size] = ++order_clock,
    index_table[heap_size] = index, next_table[heap_size] = get_next_price(index);

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
    // wont be used twice if Q == 1

    if(PRINT_TIME) printf("Before find new: %lf\n", (double) (clock() - StartTime) / CLOCKS_PER_SEC);

    clock_t now = clock();

    for(int i=0; i<k; i++){

        result[++max_k_index] = pop();

        // if(CHECK){
        //     printf("Round %d:\n", max_k_index);
        //     print_heap();
        // }

        int day = get_day(poped_index);
        if(day <= MAX_DAY && day <= N + k){
            ll p = get_price(poped_index);
            insert(poped_index, p);
        }

        // if(CHECK){
        //     printf("After insertion:\n");
        //     print_heap();
        // }

    }

    if(PRINT_TIME) printf("After find new: %lf\n", (double) (clock() - StartTime) / CLOCKS_PER_SEC);

    clock_t end = clock();
    if(PRINT_TIME) printf("Average: %lf sec per loop.\n", (double)(end - now) / CLOCKS_PER_SEC / k);
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


// From here is sub5
int cur_find_k;
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
int ReStart, ReEnd, StStart, StEnd, ReSize, StSize, ReMid, StMid;

ll PriceHash[MAX_K + MAX_N];
ll GetPriceSub6(int s, int Day){
    // if(PriceHash[Day] == -1)
        return (PriceHash[Day] = price(s, Day));
    return PriceHash[Day];
}

int FloorDiv(int x){
    return x / 2 + x % 2;
}

int LPR(int Day){
    return Day - N - StStart + 2;
}

int GPR(int Day){
    return Day + N - StStart;
}

void UpdateValue(){
    ReSize = ReEnd - ReStart + 1,
    StSize = StEnd - StStart + 1,
    ReMid = ReStart + FloorDiv(ReSize) - 1,
    StMid = StStart + FloorDiv(StSize) - 1;
}

ll MinCase(int s){
    ll min = result[ReStart];
    for(int i = 0; i < N; i++)
        min = MIN(min, price(s, StStart + i));
    return min;
}

int LinearSelect(int s, ll Arr[], int k, int p, int q){
    int Index = p - 1, Count;
    for(int i = p; i < q; i++)
        if(price(s, Arr[i]) < price(s, Arr[q]))
            swap(&(Arr[++Index]), &(Arr[i]));
    swap(&(Arr[++Index]), &(Arr[q]));
    Count = Index - p + 1;
    if(Count == k)
        return Arr[Index];
    else if(Count < k)
        return LinearSelect(s, Arr, k - Count, Index + 1, q);
    else
        return LinearSelect(s, Arr, k, p, Index - 1);
}

ll EdgeCase(int s, int k){
    // Possible range: [st, st + k + N]
    // Use linear selection
    ll Arr[MAX_K], KthDay;
    for(int i = 0; i <= k + N; i++){
        Arr[i] = StStart + i;
    }
    KthDay = LinearSelect(s, Arr, k, MAX(StStart, StStart + k - N), StStart + k + N);
    return price(s, KthDay);
}

void QuickSort(int s, ll Arr[], int p, int q){
    if(p >= q) return;
    int Index = p - 1;
    for(int i = p; i < q; i++)
        if(price(s, Arr[i]) < price(s, Arr[q]))
            swap(&(Arr[++Index]), &(Arr[i]));
    swap(&(Arr[++Index]), &(Arr[q]));
    QuickSort(s, Arr, p, Index - 1);
    QuickSort(s, Arr, Index + 1, q);
}

void SelectionSort(int s, ll Arr[], int p, int q){
    for(int i = p; i < q; i++){
        ll min_index = i;
        for(int j = i + 1; j <= q; j++){
            if(price(s, Arr[min_index]) > price(s, Arr[j]))
                min_index = j;
        }
        swap(&(Arr[min_index]), &(Arr[i]));
    }
    return;
}

ll MergeKth(int s, int k, ll Arr[], int St1, int St2){

    int debug = 0;

    if(debug){
        printf("\nFinding k = %d\n", k);
        printf("St1 = %d, St2 = %d\n", St1, St2);
    }

    if(k == 1){
        if(debug) printf("result[%d] = %lld\nprice(s, %d) = %lld\n", St1, result[St1], Arr[St2], price(s, Arr[St2]));
        return (price(s, Arr[St2]) < result[St1]) ?price(s, Arr[St2]) :result[St1];
    }

    // Now we want to find which is smaller:
    // k/2 th of arr1 or arr2?
    // If arr1[k/2] is smaller, then there are less than 2 * (k/2) elements smaller;
    // i.e. it can't be kth element. Vice versa.
    int cur = k / 2, Size1 = ReEnd - St1 + 1, Size2 = StSize - St2 + 1;

    if(debug) printf("cur = %d, Size1 = %d, Size2 = %d\n", cur, Size1, Size2);

    // if any of arr (in this case, only result) has < k/2 size, then directly use last one.
    if(Size1 < cur){
        if(debug) printf("Size1 is less than cur.\n");

        ll Val1 = result[ReEnd], Val2 = price(s, Arr[St2 + cur - 1]);

        if(debug) printf("Val1 = %lld\nVal2 = %lld\n", Val1, Val2);

        if(Val1 > Val2)
            return MergeKth(s, k - cur, Arr, St1, St2 + cur);
        else  // There are k - size left to be found
            return price(s, Arr[St2 + k - Size1 - 1]);
    }
    else if(Size2 < cur){
        ll Val1 = result[St1 + cur - 1], Val2 = price(s, Arr[StSize]);

        if(debug) printf("Size2 is less than cur.\n");
        if(debug) printf("Val1 = %lld\nVal2 = %lld\n", Val1, Val2);

        if(Val1 < Val2)
            return MergeKth(s, k - cur, Arr, St1 + cur, St2);
        else  // There are k - size left to be found
            return result[St1 + k - Size2 - 1];
    }

    ll Half1 = result[St1 + cur - 1], Half2 = price(s, Arr[St2 + cur - 1]);

    if(debug) printf("Half1 = result[%d] = %lld\nHalf2 = price(s, Arr[%d]) = price(s, %d) = %lld\n", St1 + cur - 1, Half1, St2 + cur - 1, Arr[St2 + cur - 1], Half2);

    if(Half1 > Half2)
        return MergeKth(s, k - cur, Arr, St1, St2 + cur);
    else
        return MergeKth(s, k - cur, Arr, St1 + cur, St2);
}

int SortS;
ll SortArr[10];
int ReducCmp(const void *a, const void *b){
    if(price(SortS, *(int *) a) > price(SortS, *(int *) b)) return 1;
    else return -1;
}

int GetMedianIndex(ll Arr[], ll p, ll mid, ll q){
    if(Arr[p] > Arr[mid]) swap(&(p), &(mid));
    if(Arr[mid] > Arr[q]) swap(&(mid), &(q));
    if(Arr[p] > Arr[mid]) swap(&(p), &(mid));
    return mid;
}

ll QuickSelect(ll Arr[], int p, int q, int k){

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

    int Index = p - 1, Count, Mid = GetMedianIndex(Arr, p, (p + q) / 2, q);
    swap(&(Arr[q]), &(Arr[Mid]));
    for(int i = p; i < q; i++)
        if(Arr[i] <= Arr[q])
            swap(&(Arr[++Index]), &(Arr[i]));
    swap(&(Arr[++Index]), &(Arr[q]));
    Count = Index - p + 1;
    if(Count == k)
        return Arr[Index];
    else if(Count < k)
        return QuickSelect(Arr, Index + 1, q, k - Count);
    else
        return QuickSelect(Arr, p, Index - 1, k);
}

ll ReducArr[MAX_N * 100];
ll ReducCase(int s, int k){

    if(PRINT_TIME) printf("After recur: %lf\n", (double) (clock() - StartTime) / CLOCKS_PER_SEC);

    int debug = 0;
    // if(debug) printf("It's ReducCase.\n", StSize);
    if(debug){
    //     printf("\nFinding k = %d\n", k);
         printf("Re: size = %d\n", ReSize);
    //     printf("[%d, %d]\n", ReStart, ReEnd);
        printf("St: size = %d\n", StSize);
    //     printf("[%d, %d]\n", StStart, StEnd);
    }

    int ArrTop = -1;
    for(int i = ReStart; i <= ReEnd; i++)
        ReducArr[++ArrTop] = result[i];
    for(int i = StStart; i <= StEnd; i++)
        ReducArr[++ArrTop] = GetPriceSub6(s, i);

    if(PRINT_TIME) printf("Before select: %lf\n", (double) (clock() - StartTime) / CLOCKS_PER_SEC);

    return QuickSelect(ReducArr, 0, ArrTop, k);

    /*
    if(PRINT_TIME) printf("Before sorted: %lf\n", (double) (clock() - StartTime) / CLOCKS_PER_SEC);

    ll Days[3 * MAX_N];
    for(int i = 1; i <= StSize; i++)
        Days[i] = StStart + i - 1;


    SortS = s;

    qsort(Days + 1, StSize, sizeof(ll), ReducCmp);

    if(PRINT_TIME) printf("After sorted: %lf\n", (double) (clock() - StartTime) / CLOCKS_PER_SEC);

    if(debug && 0){
        printf("After sorted:\n");
        for(int i = 1; i <= StSize; i++)
            printf("day %d: %lld\n", Days[i], price(s, Days[i]));
    }

    return MergeKth(s, k, Days, ReStart, 1);*/
}

int ccc=0;
ll RecurSub6(int s, int k){
    int debug = 0;

    // ccc++; if(ccc > 1000) return 0;

    if(k == 1)
        return MinCase(s);
    if(ReSize == 0)
        return EdgeCase(s, k);
    if(StSize <= MAX(3 * N, 1000))
        return ReducCase(s, k);

    ll ReMidVal = result[ReMid], StMidVal = GetPriceSub6(s, StMid);
    int ReLPR = ReMid - ReStart + 1, ReGPR = ReLPR,
        StLPR = LPR(StMid),
        StGPR = GPR(StMid);

    // if(debug){
    //     printf("\nFinding k = %d\n", k);
    //     printf("Re: size = %d\n", ReSize);
    //     printf("[%d, %d], mid = %d, LPR = %d, GPR = %d\n", ReStart, ReEnd, ReMid, ReLPR, ReGPR);
    //     printf("St: size = %d\n", StSize);
    //     printf("[%d, %d], mid = %d, LPR = %d, GPR = %d\n", StStart, StEnd, StMid, StLPR, StGPR);
    //     printf("SumLPR = %d, SumGPR = %d\n", ReLPR + StLPR, ReGPR + StGPR);
    //     printf("\nReMidVal = %lld\nStMidVal = %lld\nRe > St = %d\n", ReMidVal, StMidVal, ReMidVal > StMidVal);
    // }


    if(ReLPR + StLPR >= k)
        if(ReMidVal > StMidVal){
            ReEnd = ReMid - (ReLPR + StLPR > k);
            if(ReSize == 1){
                ll EdgeVal = EdgeCase(s, k);
                return MIN(result[ReStart], EdgeVal);
            }
        }
        else
            StEnd = StMid + N - 1;
    else if(ReGPR + StGPR <= k)
        if(ReMidVal < StMidVal){
            int CurStart = ReStart;
            ReStart = ReMid + (ReGPR + StGPR < k), k -= ReStart - CurStart;
            if(ReSize == 1){
                ll EdgeVal = EdgeCase(s, k);
                return MAX(result[ReStart], EdgeVal);
            }
        }
        else{
            int CurStart = StStart;
            StStart = StMid - N + 1, k -= StStart - CurStart;
        }
    else{
        int LPRDiff = k - (ReLPR + StLPR), GPRDiff = (ReGPR + StGPR) - k;

        // if(debug) printf("LPRDiff = %d, GPRDiff = %d\n", LPRDiff, GPRDiff);

        if(ReMidVal > StMidVal){
            int CurStart = StStart, CurEnd = ReEnd;
            ReEnd = MIN(ReEnd, ReMid + LPRDiff);
            StStart = MAX(StStart, StMid - (GPRDiff / N + 1) * N);

            // if(debug) printf("Re: [%d, %d] is impossible.\n", ReEnd + 1, CurEnd);
            // if(debug) printf("St: [%d, %d] is impossible.\n", CurStart, StStart - 1);

            k -= StStart - CurStart;
        }
        else{
            int CurStart = ReStart, CurEnd = StEnd;
            ReStart = MAX(ReStart, ReMid - GPRDiff);
            StEnd = MIN(StEnd, StMid + (LPRDiff / N + 1) * N);
            k -= ReStart - CurStart;

            // if(debug) printf("Re: [%d, %d] is impossible.\n", CurStart, ReStart - 1);
            // if(debug) printf("St: [%d, %d] is impossible.\n", StEnd + 1, CurEnd);
        }
    }

    UpdateValue();
    return RecurSub6(s, k);
}

ll QuerySub6(int s, int k){
    // for(int i = 1; i <= k + N; i++)
        // PriceHash[i] = -1;
    ReStart = 1, ReEnd = k, StStart = 1, StEnd = k + N;
    UpdateValue();
    return RecurSub6(s, k);
}

ll query(){
    int new_stock = next_int(), k = next_int();

    if(max_k_index == 0)
        find_new_res(1000000);

    // system("pause");

    if(PRINT_TIME) printf("After find result: %lf\n", (double) (clock() - StartTime) / CLOCKS_PER_SEC);

    if(new_stock == 0 || hash_large[new_stock % MOD] || hash_small[new_stock % MOD])
        return result[k];
    if(N == 1)
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

    // if(PRINT_TIME) printf("Before init: %lf\n", (double) (clock() - StartTime) / CLOCKS_PER_SEC);

    if(A == 1 && N == 1){
        solve_subtask1();
        return;
    }
    init();

    // if(PRINT_TIME) printf("After init: %lf\n", (double) (clock() - StartTime) / CLOCKS_PER_SEC);

    for(int i=0; i<Q; i++){
        printf("%lld\n", query());
        // if(PRINT_TIME) printf("After query: %lf\n", (double) (clock() - StartTime) / CLOCKS_PER_SEC);
    }
}

int main()
{
    StartTime = clock();

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


