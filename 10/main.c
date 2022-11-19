#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#define MAX_N ((int) 1e6 + 100)
#define MAX_M ((int) 1e6 + 100)

char nc() {
  static char buf[1000000], *p = buf, *q = buf;
  return p == q && (q = (p = buf) + fread(buf, 1, 1000000, stdin), p == q)
             ? EOF
             : *p++;
}
int get_int(){
    int num = 0, c = nc();
    while(!(c >= '0' && c <= '9'))
        c = nc();
    while((c >= '0' && c <= '9'))
        num = num * 10 + c - '0', c = nc();
    return num;
}

char nextchar(){
    int c = nc();
    while(!islower(c)) c = nc();
    return c;
}

int N, M;

int Ans[MAX_M] = {0};

typedef struct merge_node{
    int day, x, y;
    struct merge_node *next;
} MergeNode;


typedef struct query_node{
    int day;
    struct query_node *next;
} QueryNode;

struct day_set{
    int p, rank, shop_count;
    MergeNode *merge_top;
    QueryNode *query_top;
} DaySet[MAX_M];

MergeNode *MakeNode(int day, int x, int y){
    MergeNode *newnode = (MergeNode *) malloc(sizeof(MergeNode));
    newnode->day = day, newnode->x = x, newnode->y = y, newnode->next = NULL;
    return newnode;
}

int FindDay(int day){
    if(DaySet[day].p == -1)
        return day;
    return DaySet[day].p = FindDay(DaySet[day].p);
}

void PrintDay(){
    printf("\nDay: / Set:\n");
    for(int i = 0; i <= M; i++)
        printf("%d  / %d\n", i, FindDay(i));
}


void MergeDay(int a, int b){
    int Pa = FindDay(a), Pb = FindDay(b);
    if(Pa == Pb)
        return;
    if(DaySet[Pa].rank < DaySet[Pb].rank)
        DaySet[Pa].p = Pb;
    else if(DaySet[Pa].rank > DaySet[Pb].rank)
        DaySet[Pb].p = Pa;
    else
        DaySet[Pb].p = Pa, DaySet[Pa].rank++;
}

struct shop_set{
    int p, rank;
} ShopSet[MAX_N];

void MakeShopSet(){
    for(int i = 1; i <= N; i++)
        ShopSet[i].p = 0, ShopSet->rank = 1;
}

int FindShop(int shop){
    while(ShopSet[shop].p != 0)
        shop = ShopSet[shop].p;
    return shop;
}

void MergeShop(int x, int y){
    int Px = FindShop(x), Py = FindShop(y);
    if(Px == Py)
        return;
    if(ShopSet[Px].rank < ShopSet[Py].rank)
        ShopSet[Px].p = Py;
    else if(ShopSet[Px].rank > ShopSet[Py].rank)
        ShopSet[Py].p = Px;
    else
        ShopSet[Py].p = Px, ShopSet[Px].rank++;
}

void Recur(int Parent){
    QueryNode *qcur = DaySet[Parent].query_top;
    while(qcur != NULL){
        // printf("Parent = %d\n", Parent);
        Ans[qcur->day] = DaySet[Parent].shop_count;
        qcur = qcur->next;
    }

    MergeNode *mcur = DaySet[Parent].merge_top;
    while(mcur != NULL){
        int Px = FindShop(mcur->x), Py = FindShop(mcur->y), MergeCase = 0;
        if(Px != Py){
            DaySet[mcur->day].shop_count = DaySet[Parent].shop_count - 1;
            if(ShopSet[Px].rank < ShopSet[Py].rank)
                ShopSet[Px].p = Py, MergeCase = 0;
            else if(ShopSet[Px].rank > ShopSet[Py].rank)
                ShopSet[Py].p = Px, MergeCase = 1;
            else
                ShopSet[Py].p = Px, ShopSet[Px].rank++, MergeCase = 2;
        }
        else
            DaySet[mcur->day].shop_count = DaySet[Parent].shop_count;

        Recur(mcur->day);

        switch (MergeCase){
            case 0:
                ShopSet[Px].p = 0;
                break;
            case 1:
                ShopSet[Py].p = 0;
                break;
            case 2:
                ShopSet[Py].p = 0, ShopSet[Px].rank--;
        }

        mcur = mcur->next;
    }
}

int main()
{
    int N = get_int(), M = get_int();


    DaySet[0].shop_count = N;
    for(int i = 0; i <= M; i++)
        DaySet[i].p = -1, DaySet[i].rank = 1,
        DaySet[i].merge_top = NULL, DaySet[i].query_top = NULL;

    MakeShopSet();

    for(int day = 0; day < M; day++){
        int x, y, k;
        char buf[20];
        scanf("%s", buf);
        switch(buf[0]){
            case 'm':{
                int Pday = FindDay(day);
                x = get_int(), y = get_int();
                if(DaySet[Pday].merge_top == NULL)
                    DaySet[Pday].merge_top = MakeNode(day + 1, x, y);
                else{
                    MergeNode *newnode = MakeNode(day + 1, x, y);
                    newnode->next = DaySet[Pday].merge_top;
                    DaySet[Pday].merge_top = newnode;
                }
                break;
            }
            case 'q':{
                int Pday = FindDay(day);
                MergeDay(Pday, day + 1);

                QueryNode *newnode = (QueryNode *) malloc(sizeof(QueryNode));
                newnode->day = day + 1, newnode->next = NULL;

                if(DaySet[Pday].query_top == NULL)
                    DaySet[Pday].query_top = newnode;
                else{
                    newnode->next = DaySet[Pday].query_top;
                    DaySet[Pday].query_top = newnode;
                }
                break;
            }
            case 'b':
                k = get_int();
                MergeDay(k, day + 1);
                break;
        }
    }


    // PrintDay();

    Recur(0);

    for(int i = 1; i <= M; i++)
        if(Ans[i])
            printf("%d\n", Ans[i]);
}
