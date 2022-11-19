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
#define MOD ((int) 1e7 + 9)
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
        if(day <= MAX_DAY && day <= N + k){
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
int ReStart, ReEnd, StStart, StEnd, ReSize, StSize, ReMid, StMid;

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
    ll min = result[ReStart], cur_p;
    for(int i = 0; i < N; i++)
        cur_p = price(s, StStart + i), min = MIN(min, cur_p);
    return min;
}

int LinearSelect(int s, ll Arr[], int k, int p, int q){
    int Index = p - 1, Count;
    for(int i = p; i < q; i++)
        if(Arr[i] <= Arr[q])
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
    ll Arr[MAX_K];
    for(int i = 0; i <= k + N; i++)
        Arr[i] = price(s, StStart + i);

    return LinearSelect(s, Arr, k, StStart, StStart + k + N);
}

ll MergeKth(int s, int k, ll Arr[], int St1, int St2){

    // int debug = 0;

    // if(debug){
    //     printf("\nFinding k = %d\n", k);
    //     printf("St1 = %d, St2 = %d\n", St1, St2);
    // }

    if(k == 1){
        // if(debug) printf("result[%d] = %lld\nprice(s, %d) = %lld\n", St1, result[St1], Arr[St2], price(s, Arr[St2]));
        return (Arr[St2] < result[St1]) ?Arr[St2] :result[St1];
    }

    int cur = k / 2, Size1 = ReEnd - St1 + 1, Size2 = StSize - St2 + 1;

    // if(debug) printf("cur = %d, Size1 = %d, Size2 = %d\n", cur, Size1, Size2);

    // if any of arr (in this case, only result) has < k/2 size, then directly use last one.
    if(Size1 < cur){
        // if(debug) printf("Size1 is less than cur.\n");

        ll Val1 = result[ReEnd], Val2 = Arr[St2 + cur - 1];

        // if(debug) printf("Val1 = %lld\nVal2 = %lld\n", Val1, Val2);

        if(Val1 > Val2)
            return MergeKth(s, k - cur, Arr, St1, St2 + cur);
        else  // There are k - size left to be found
            return Arr[St2 + k - Size1 - 1];
    }
    else if(Size2 < cur){
        ll Val1 = result[St1 + cur - 1], Val2 = Arr[StSize];

        // if(debug) printf("Size2 is less than cur.\n");
        // if(debug) printf("Val1 = %lld\nVal2 = %lld\n", Val1, Val2);

        if(Val1 < Val2)
            return MergeKth(s, k - cur, Arr, St1 + cur, St2);
        else  // There are k - size left to be found
            return result[St1 + k - Size2 - 1];
    }

    ll Half1 = result[St1 + cur - 1], Half2 = Arr[St2 + cur - 1];

    // if(debug) printf("Half1 = result[%d] = %lld\nHalf2 = price(s, Arr[%d]) = price(s, %d) = %lld\n", St1 + cur - 1, Half1, St2 + cur - 1, Arr[St2 + cur - 1], Half2);

    if(Half1 > Half2)
        return MergeKth(s, k - cur, Arr, St1, St2 + cur);
    else
        return MergeKth(s, k - cur, Arr, St1 + cur, St2);
}

int ReducCmp(const void *a, const void *b){
    if(*(ll *) a > *(ll *) b) return 1;
    return -1;
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

ll ReducCase(int s, int k){

    // int debug = 0;
    // if(debug) printf("It's ReducCase.\n", StSize);
    // if(debug){
    // //     printf("\nFinding k = %d\n", k);
    //      printf("Re: size = %d\n", ReSize);
    // //     printf("[%d, %d]\n", ReStart, ReEnd);
    //     printf("St: size = %d\n", StSize);
    // //     printf("[%d, %d]\n", StStart, StEnd);
    // }

    int ArrTop = -1;
    ll ReducArr[MAX_K];
    for(int i = ReStart; i <= ReEnd; i++)
        ReducArr[++ArrTop] = result[i];
    for(int i = StStart; i <= StEnd; i++)
        ReducArr[++ArrTop] = price(s, i);

    return QuickSelect(ReducArr, 0, ArrTop, k);

    // ll Prices[100 * MAX_N];
    // for(int i = 1; i <= StSize; i++)
    //     Prices[i] = price(s, StStart + i - 1);

    // qsort(Prices + 1, StSize, sizeof(ll), ReducCmp);
    // return MergeKth(s, k, Prices, ReStart, 1);
}

int ccc=0, Qcount=0, HasChanged;
ll RecurSub6(int s, int k){
    int debug = 0;

    if(k == 1)
        return MinCase(s);
    if(ReSize == 0)
        return EdgeCase(s, k);
    if(StSize <= MAX(3 * N, 1000) || HasChanged == 0)
        return ReducCase(s, k);

    ll ReMidVal = result[ReMid], StMidVal = price(s, StMid);
    int ReLPR = ReMid - ReStart + 1, ReGPR = ReLPR,
        StLPR = LPR(StMid),
        StGPR = GPR(StMid);

    // if(debug){
    //      printf("\nFinding k = %d\n", k);
    //      printf("Re: size = %d\n", ReSize);
    //      printf("[%d, %d], mid = %d, LPR = %d, GPR = %d\n", ReStart, ReEnd, ReMid, ReLPR, ReGPR);
    //      printf("St: size = %d\n", StSize);
    //      printf("[%d, %d], mid = %d, LPR = %d, GPR = %d\n", StStart, StEnd, StMid, StLPR, StGPR);
    //      printf("SumLPR = %d, SumGPR = %d\n", ReLPR + StLPR, ReGPR + StGPR);
    //      printf("\nReMidVal = %lld\nStMidVal = %lld\nRe > St = %d\n", ReMidVal, StMidVal, ReMidVal > StMidVal);
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

    int OriSizeSum = ReSize + StSize;
    UpdateValue();
    HasChanged = OriSizeSum != ReSize + StSize;
    return RecurSub6(s, k);
}

ll QuerySub6(int s, int k){
    HasChanged = 1;
    ReStart = 1, ReEnd = k, StStart = 1, StEnd = k + N;
    UpdateValue();

    return RecurSub6(s, k);
}

ll query(){
    int new_stock = next_int(), k = next_int();

    if(max_k_index == 0)
        find_new_res(1000000);

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


