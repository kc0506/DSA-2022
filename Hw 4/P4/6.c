#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_N ((int) 1e5 + 100)
#define MAX_Q ((int) 1e5 + 100)
#define MIN(a, b) ((a < b) ?a :b)
#define MAX(a, b) ((a > b) ?a :b)

void Swap(int *a, int *b){
    *b = *a + *b - (*a = *b);
}

int GetInt(){
    int num = 0, c = getchar();
    while(!isdigit(c))
        c = getchar();
    while(isdigit(c))
        num = num * 10 + c - '0', c = getchar();
    return num;
}

char NextChar(){
    int c = getchar();
    while(!islower(c)) c = getchar();
    return c;
}

int N, Q, Cost[MAX_N];

int main()
{
    scanf("%d%d", &N, &Q);

    for(int i = 1; i <= N; i++)
        scanf("%d", &(Cost[i]));

    int PreSum[MAX_N] = {0};
    for(int i = 1; i <= N; i++)
        PreSum[i] = PreSum[i - 1] + Cost[i];

    for(int i = 0; i < Q; i++){
        int l, r;
        scanf("%d%d", &l, &r);
        printf("%d\n", PreSum[r] - PreSum[l - 1]);
    }
}


