#include <stdio.h>
#include <ctype.h>
#include "price.h"

#define MAX_A 1025
#define INF 10000

typedef long long int ll;

int A, Q, N, S[MAX_A];

int next_int(){
    int c = getchar(), num = 0;
    while(!isdigit(c))
        c = getchar();
    while(isdigit(c))
        num += num * 10 + c - '0', c = getchar();
    return num;
}

// ll p(int s, int t){return t; }

ll solve_subtask1(int k){
    /*
    Since N = 1, p(t) are strictly increasing
    Hence k-th smallest is p(t = k)
    */
    return price(S[0], k);
}

ll solve_subtask2(int k){
    int min = -INF, count = 0, index, cur_t[MAX_A];
    for(int i=0; i<A; i++)
        cur_t[i] = 1;

    while(count < k){
        min = INF, index = -1;
        for(int i = 0; i < A; i++)
            if(cur_t[i] <= 1e9)
                if(price(S[i], cur_t[i]) < min)
                    min = price(S[i], cur_t[i]), index = i;
        cur_t[index]++, count++;
    }

    for(int i=0; i<A; i++)
        printf("%d ", cur_t[i]);
    printf("\n");

    return min;
}

ll solve(int new_stock, int k){
    if(A == 1)
        return solve_subtask1(k);
    else if(N == 1 && new_stock == 0)
        return solve_subtask2(k);
    else
        return 0;
}

int main()
{
    A = next_int(), Q = next_int(), N = next_int();

    for(int i=0; i<A; i++)
        S[i] = next_int();

    for(int i=0; i<Q; i++){
        int new_stock = next_int(), k = next_int();
        printf("%lld\n", solve(new_stock, k));
    }
}