#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>
// #include "price.h"

#define MIN(a, b) ((a < b) ?a :b)
#define MAX(a, b) ((a > b) ?a :b)
#define MOD ((int) 1e7 + 9)
#define MAX_A 1224
#define MAX_N 1224
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


typedef long long int ll;
typedef struct node{int index; struct node *next, *prev;} node;

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
    if(get_day(index) <= MAX_DAY){
        return price(get_s(index), get_day(index));
    }
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
        // printf("result[%d] = %lld\n", max_k_index, result[max_k_index]);

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

void init(){
    max_k_index = 0;

    for(int index = 0; index < N * A; index++){
        day_table[index] = 1;
    }
    build_heap();

    if(0){
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
int NotClosed, TotalSize, HalfSum, DLLStart, StartDay[MAX_N], EndDay[MAX_N], MidDay[MAX_N], HalfSize[MAX_N], Closed[MAX_N], DoubleLL[MAX_N][2];

int RealDay(int Index, int Day){
    return (Day - 1) * N + (Index - 1) % N + 1;
}

ll GetFirstVal(int s, int Index){
    if(Index == 0) return result[StartDay[Index]];
    return price(s, RealDay(Index, StartDay[Index]));
}

ll GetMidVal(int s, int Index){
    if(Index == 0) return result[MidDay[Index]];
    return price(s, RealDay(Index, MidDay[Index]));
}

ll GetValByDay(int s, int Index, int Day){
    if(Index == 0) return result[Day];
    return price(s, RealDay(Index, Day));
}

ll GetKthVal(int s, int Index, int k){
    if(Index == 0) return result[StartDay[Index] + k - 1];
    return price(s, RealDay(Index, StartDay[Index] + k - 1));
}

int GetMidDay(int Index){
    return StartDay[Index] + HalfSize[Index] - 1;
}

int GetHalfSize(int Index){
    if(StartDay[Index] == EndDay[Index])
        return 1;
    return (EndDay[Index] - StartDay[Index] + 1) / 2;
}

// Deap part
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

void Preproc(int k){
    DLLStart = N;
    for(int i = 0; i <= N; i++)
        StartDay[i] = 1, EndDay[i] = k / N + 3, HalfSize[i] = GetHalfSize(i),
        MidDay[i] = GetMidDay(i), DoubleLL[i][PREV] = (i + N) % (N + 1), DoubleLL[i][NEXT] = (i + 1) % (N + 1);
    EndDay[0] = k;

    HalfSum = 0;
    for(int i = 0; i <= N; i++)
        HalfSum += HalfSize[i];

    for(int i = 0; i <= N; i++)
        Closed[i] = 0;
    NotClosed = N + 1;
    TotalSize = N * (k / N + 3) + k;

    // for(int i=1; i<=k; i++)
        // result[i] = 10000000000000000;
}

int FindIndex(int MaxOrMin, int s){
    int FindMin = 0, FindMax = 1, First = 1, Index, debug = 0;
    ll ExtrVal;
    if(MaxOrMin == FindMax){
        for(int i = DLLStart; ;){
            if(First)
                Index = i, ExtrVal = GetMidVal(s, i), First = 0;
            else if(GetMidVal(s, i) > ExtrVal)
                Index = i, ExtrVal = GetMidVal(s, i);
            if((i = DoubleLL[i][NEXT]) == DLLStart)
                break;
        }
    }
    else if(MaxOrMin == FindMin){
        for(int i = DLLStart; ;){
            if(First)
                Index = i, ExtrVal = GetMidVal(s, i), First = 0;
            else if(GetMidVal(s, i) < ExtrVal)
                Index = i, ExtrVal = GetMidVal(s, i);
            if((i = DoubleLL[i][NEXT]) == DLLStart)
                break;
        }
    }

    // if(debug){
    //     if(MaxOrMin) printf("Max is: "); else printf("Min is: ");
    //     printf("[%d][%d] = %lld\n", Index, MidDay[Index], GetMidVal(s, Index));
    // }

    return Index;
}

void UpdateStart(int Index, int NewVal){
    TotalSize += StartDay[Index] - (StartDay[Index] = MAX(StartDay[Index], NewVal));
    HalfSum = HalfSum - HalfSize[Index] + (HalfSize[Index] = GetHalfSize(Index));
    MidDay[Index] = GetMidDay(Index);
    if(!Closed[Index] && HalfSize[Index] == 0){
        if(Index == DLLStart)
            DLLStart = DoubleLL[Index][NEXT];
        DoubleLL[DoubleLL[Index][PREV]][NEXT] = DoubleLL[Index][NEXT],
        DoubleLL[DoubleLL[Index][NEXT]][PREV] = DoubleLL[Index][PREV];
    }
    NotClosed -= (!Closed[Index]) && (Closed[Index] = HalfSize[Index] == 0);
}

void UpdateEnd(int Index, int NewVal){
    TotalSize -= EndDay[Index] - (EndDay[Index] = MIN(EndDay[Index], NewVal));
    HalfSum = HalfSum - HalfSize[Index] + (HalfSize[Index] = GetHalfSize(Index));
    MidDay[Index] = GetMidDay(Index);
    if(!Closed[Index] && HalfSize[Index] == 0){
        if(Index == DLLStart)
            DLLStart = DoubleLL[Index][NEXT];
        DoubleLL[DoubleLL[Index][PREV]][NEXT] = DoubleLL[Index][NEXT],
        DoubleLL[DoubleLL[Index][NEXT]][PREV] = DoubleLL[Index][PREV];
    }
    NotClosed -= (!Closed[Index]) && (Closed[Index] = HalfSize[Index] == 0);
}

void Print(){
    printf("NotClosed = %d, TotalSize = %d, HalfSum = %d\n", NotClosed, TotalSize, HalfSum);
    // return;

    for(int i = 0; i <= N; i++){
        printf("%d:\nStart = %d, End = %d, Mid = %d\n", i, StartDay[i], EndDay[i], MidDay[i]);
        printf("Size = %d, HalfSize = %d\n", (EndDay[i] - StartDay[i] + 1), HalfSize[i]);
        printf("It's ");
        if(Closed[i]) printf("closed.\n"); else printf("not close.\n");
    }
}

ll TrivialCase(int s){
    int First = 1;
    ll Min;
    for(int i = DLLStart; ;){
        if(First)
            Min = GetFirstVal(s, i), First = 0;
        else if(GetFirstVal(s, i) < Min)
            Min = GetFirstVal(s, i);
        if((i = DoubleLL[i][NEXT]) == DLLStart)
            break;
    }
    return Min;
}

void QuickSort(ll arr[], int p, int q){
    if(p >= q) return;
    int index = p - 1;
    for(int i = p; i < q; i++)
    if(arr[i] < arr[q])
        swap(&(arr[++index]), &(arr[i]));
    swap(&(arr[++index]), &(arr[q]));
    QuickSort(arr, index + 1, q);
    QuickSort(arr, p, index - 1);
}

void QSortAndPrint(ll arr[], int p, int q, int k){
    printf("Before sorted.\n");
    QuickSort(arr, p, q);
    printf("After sorted.\n");
    for(int i=p; i<=q; i++)
        printf("%lld%c", arr[i], " \n"[i == q]);
    printf("and the k-th element is %lld\n", arr[k - 1]);
}

int GetMedian(ll arr[], ll head, ll mid, ll tail){

    if(arr[head] > arr[mid]) swap(&(head), &(mid));
    if(arr[mid] > arr[tail]) swap(&(mid), &(tail));
    if(arr[head] > arr[mid]) swap(&(head), &(mid));

    return mid;
}

ll LinearRanking(ll SearchBox[], int p, int q, int k){

    int debug = 0;
    if(debug) printf("\nFinding %d: [%d, %d]\n", k, p, q);

    int Index = p - 1, Count, Median = GetMedian(SearchBox, p, (p + q) / 2, q);
    ll Pivot = SearchBox[Median];
    swap(&(SearchBox[Median]), &(SearchBox[q]));

    if(debug) printf("Pivot = %lld\n", Pivot);

    for(int i = p; i < q; i++)
    if(SearchBox[i] <= Pivot)
        swap(&(SearchBox[++Index]), &(SearchBox[i]));
    swap(&(SearchBox[++Index]), &(SearchBox[q]));

    if(debug) printf("Index = %d\n", Index);

    Count = Index - p + 1;
    if(Count == k)
        return SearchBox[Index];
    else if(Count < k)
        return LinearRanking(SearchBox, Index + 1, q, k - Count);
    else
        return LinearRanking(SearchBox, p, Index - 1, k);
}

ll LinearSearch(int s, int k){
    int SearchTop = -1;
    ll SearchBox[MAX_N * 4];
    for(int i = DLLStart; ;){
        for(int day = StartDay[i]; day <= EndDay[i]; day++){
            SearchBox[++SearchTop] = GetValByDay(s, i, day);
        }
        if((i = DoubleLL[i][NEXT]) == DLLStart) break;
    }

    int debug = 0;
    if(debug) printf("SearchTop = %d\n", SearchTop);
    // QSortAndPrint(SearchBox, 0, SearchTop, k);

    return LinearRanking(SearchBox, 0, SearchTop, k);
}

ll RecurSub6(int s, int k){
    int debug = 0;

    if(debug)
    printf("\nk = %d:\nNotClosed = %d, TotalSize = %d, HalfSum = %d\n", k, NotClosed, TotalSize, HalfSum);

    if(k == 1){
        if(debug) printf("This is trivail case.\n");
        return TrivialCase(s);
    }
    if(NotClosed == 1){
        int i = DLLStart;
        if(debug) printf("Since only %d is not closed, directly return[%d][%d] = %lld\n", i, i, StartDay[i] + k - 1, GetKthVal(s, i, k));
        return GetKthVal(s, i, k);
    }
     if(TotalSize < 4 * N)
         return LinearSearch(s, k);

    int SumCase, Index, CurHalfSum = HalfSum, NewVal, FindMin = 0, FindMax = 1, CurSize = TotalSize;

    if(HalfSum < k)
        SumCase = 0;
    else if(HalfSum == k)
        SumCase = 1;
    else
        SumCase = 2;

    switch(SumCase){
        case 0:
            // if(debug) printf("HalfSum < k\n");

            Index = FindIndex(FindMin, s), NewVal = MidDay[Index];
            // Reject StartDay[index] ~ mid[index]
            // If Mid[index] = aN + b, then we are rejecting all n s.t. price(s, n) < price(s, aN + b)
            // (1) aN + b > a' * N + b for all a' < a, i.e. update shrink [index]
            // (2) aN + b > a' * N + b' for all a' < a and b' < b
            // When Mid[index] is aN + b day, then RealMid is a + 1
            // Hence StartDay[[1: index]] = RealMid + 1

            // if(debug) printf("NewVal = %d\n", NewVal);

            UpdateStart(Index, NewVal + 1);
            if(Index)
            for(int i = DLLStart; ;){
                if(i < Index && i)
                    UpdateStart(Index, NewVal);
                else if(i > Index && i)
                    UpdateStart(Index, NewVal - 1);
                if((i = DoubleLL[i][NEXT]) == DLLStart) break;
            }
            // printf("k = %d - (%d - %d) = ", k, CurSize, TotalSize);
            k -= CurSize - TotalSize;
            // printf("%d\n", k);
            break;
        case 1:
            // if(debug) printf("HalfSum = k\n");

            Index = FindIndex(FindMax, s), NewVal = MidDay[Index];
            UpdateEnd(Index, NewVal);

            // if(debug) printf("NewVal = %d\n", NewVal);

            UpdateEnd(Index, NewVal);
            if(Index)
            for(int i = DLLStart; ;){
                if(i < Index && i)
                    UpdateEnd(i, NewVal + 1);
                else if(i > Index && i)
                    UpdateEnd(i, NewVal);
                if((i = DoubleLL[i][NEXT]) == DLLStart) break;
            }
            if(CurSize == TotalSize){
                // if(debug) printf("CurSize = %d, TotalSize = %d; There is no change.\n", CurSize, TotalSize);
                ll CurMidVal = GetMidVal(s, Index);
                if(TotalSize == HalfSum){
                    if(debug) printf("Since Size = %d and HalfSum = %d are the same, directly return.\n", TotalSize, HalfSum);
                    return CurMidVal;
                }
                else{
                    if(debug) printf("Preserve Mid[%d] (%d) = %lld as candidate.\n", Index, MidDay[Index], CurMidVal);
                    UpdateEnd(Index, StartDay[Index] - 1);
                    ll RecurVal = RecurSub6(s, k);
                    return MIN(CurMidVal, RecurVal);
                }
            }
            break;
        case 2:
            // if(debug) printf("HalfSum > k\n");
            Index = FindIndex(FindMax, s), NewVal = MidDay[Index];
            NewVal = MidDay[Index];
            // if(debug) printf("NewVal = %d\n", NewVal);
            UpdateEnd(Index, NewVal - 1);
            if(Index)
            for(int i = DLLStart; ;){
                if(i < Index && i)
                    UpdateEnd(i, NewVal + 1);
                else if(i > Index && i)
                    UpdateEnd(i, NewVal);
                if((i = DoubleLL[i][NEXT]) == DLLStart) break;
            }
            break;
    }
    return RecurSub6(s, k);
}

ll QuerySub6(int s, int k){
    Preproc(k);
    return RecurSub6(s, k);
}

ll query(){
    int new_stock = next_int(), k = next_int();

    if(k > max_k_index)
        find_new_res(k - max_k_index);

    if(new_stock == 0 || hash_large[new_stock % MOD] || hash_small[new_stock % MOD])
        return result[k];
    if(N == 1)
        return QuerySub6(new_stock, k);
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

// void update(int index, int cut){

//     int check = 0;

//     if(check)
//         printf("b: (%d, %d) %d\n", st[index], end[index], mid[index]);

//     if(st[index] > end[index])
//         return;

//     int cur_mid = mid[index];

//     if(cut == LHALF)
//         st[index] = get_mid_index_deap(index) + 1;

//     if(cut == GHALF)
//         end[index] = get_mid_index_deap(index) - 1;

//     if(cut == GHALF2)
//         end[index] = get_mid_index_deap(index);

//     mid[index] = (end[index] - st[index] + 1) / 2;
//     if(mid[index] == 0 && st[index] == end[index])
//         mid[index] = 1;

//     sum_deap = sum_deap - cur_mid + mid[index];

//     if(st[index] > end[index])
//         not_closed--;

//     if(check)
//         printf("a: (%d, %d) %d\n", st[index], end[index], mid[index]);
// }

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

    if(Closed[index])
        return;

    deap_size++;
    level_deap[deap_size] = !(level_deap[PARENT(deap_size)]);
    deap[deap_size] = GetMidVal(s, index);
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

    // if(0){
    //     printf("value = %lld\n", deap[TOP]);
    //     printf("%d: (%d, %d) %d\n", res, st[res], end[res], mid[res]);
    // }

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



