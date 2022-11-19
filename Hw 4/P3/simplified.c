// No boom ver.

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_N ((int) 1e6 + 100)
#define MAX_M ((int) 1e6 + 100)
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

int N, M, ShopCount;

int CurDay = 0;

typedef struct set{
    int parent, count;
} Set;

typedef struct node{
    int key;
    struct node *next;
} Node;

typedef struct shopset{
    int count;
    Set *;
} ShopSet[MAX_M];

Set DS[MAX_N];

void Init(){

    Data[0] = N;

    ShopCount = N;
    for(int i = 1; i <= N; i++)
        DS[i].parent = 0;

    return;
}

int Data[MAX_M] = {0};

int FindSet(int x){
    if(DS[x].parent == 0)
        return x;
    return DS[x].parent = FindSet(DS[x].parent);
}

void Merge(int x, int y){
    // merge x into y
    // int SetX = FindSet(x), SetY = FindSet(y);
    // if(SetX != SetY){
    //     DS[SetX].parent = SetY;
    //     ShopCount--;
    // }


    Data[++CurDay] = Data[CurDay] - 1;
    return;
}

void Query(){
    printf("%d\n", Data[CurDay++]);
    return;
}

void Boom(int k){
    // back to the state after k-day
    CurDay = k;
    return;
}

int main()
{
    scanf("%d%d", &N, &M);
    Init();
    for(int i = 0; i < M; i++){
        int x, y, k;
        char buf[20];
        scanf("%s\n", buf);
        switch(buf[0]){
            case 'm':
                scanf("%d%d", &x, &y);
                Merge(x, y);
                break;
            case 'q':
                Query();
                break;
            case 'b':
                scanf("%d", &k);
                break;
        }
    }
}