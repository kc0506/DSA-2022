#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#define INF ((int) 1e9 + 100)
#define MIN(a, b) ((a < b) ?a :b)
#define MAX(a, b) ((a > b) ?a :b)
#define MAX_N ((int) 1e6 + 100)

typedef long long int ll;

char nc() {
  static char buf[1000000], *p = buf, *q = buf;
  return p == q && (q = (p = buf) + fread(buf, 1, 1000000, stdin), p == q)
             ? EOF
             : *p++;
}
int NextInt(){
    int c = nc(), num = 0;
    while(!isdigit(c))
        c = nc();
    while(isdigit(c))
        num = num * 10 + c - '0', c = nc();
    return num;
}

void Swap(int *a, int *b){
    *b = *a + *b - (*a = *b);
}

typedef struct node{
    ll key, sum, max, sec, ceil;
    int size, rev, prio, count_max;
    int p, left, right;
} node;
node Tree[MAX_N << 2];
int Root, NIL, TreeTop;

void Preset(){
    NIL = 0;
    Tree[0].key = 0;
    Tree[0].sum = 0;
    Tree[0].max = 0;
    Tree[0].sec = 0;
    Tree[0].ceil = INF;
    Tree[0].size = 0;
    Tree[0].rev = 0;
    Tree[0].prio = -1;
    Tree[0].count_max = 0;
    Tree[0].p = Tree[0].left = Tree[0].right = NIL;
    Root = NIL;
    TreeTop = 1;
}

int NewNode(ll key){
    int newnode = TreeTop++;
    Tree[newnode].key = key;
    Tree[newnode].sum = key;
    Tree[newnode].max = key;
    Tree[newnode].sec = 0;
    Tree[newnode].ceil = INF;
    Tree[newnode].size = 1;
    Tree[newnode].rev = 0;
    Tree[newnode].prio = rand();
    Tree[newnode].count_max = 1;
    Tree[newnode].p = Tree[newnode].left = Tree[newnode].right = NIL;
    return newnode;
}

void Inorder(int cur){
    if(cur == NIL)
        return;
    Inorder(Tree[cur].left);
    printf("%d ", Tree[cur].key);
    Inorder(Tree[cur].right);
}

void Print(int index, int cur){
    if(cur == NIL){
        printf("%d: NIL\n", index);
        return;
    }
    Print(index * 2, Tree[cur].left);
    printf("%d:\n \tkey = %d\n\tsize = %d\n\tsum = %d\n\tceil = %lld\n", index, Tree[cur].key, Tree[cur].size, Tree[cur].sum, Tree[cur].ceil);
    printf("\tmax = %d\n\tmax_count = %d\n\tsec = %d\n", Tree[cur].max, Tree[cur].count_max, Tree[cur].sec);
    Print(index * 2 + 1, Tree[cur].right);
}

void Update(int x){
    if(x == NIL)
        return;
    Tree[x].sum = Tree[Tree[x].left].sum + Tree[Tree[x].right].sum + Tree[x].key;
    Tree[x].size = Tree[Tree[x].left].size + Tree[Tree[x].right].size + 1;
    Tree[x].max = MAX(MAX(Tree[Tree[x].left].max, Tree[x].key), Tree[Tree[x].right].max);

    // printf("%d, (%d, %d), (%d, %d)\n", Tree[x].key, Tree[Tree[x].left].max, Tree[Tree[x].left].sec, Tree[Tree[x].right].max, Tree[Tree[x].right].sec);

    if(Tree[x].key > Tree[Tree[x].left].max && Tree[x].key > Tree[Tree[x].right].max)
        Tree[x].sec = MAX(Tree[Tree[x].left].max, Tree[Tree[x].right].max);
    else if(Tree[x].key == Tree[Tree[x].left].max && Tree[x].key > Tree[Tree[x].right].max)
        Tree[x].sec = MAX(Tree[Tree[x].left].sec, Tree[Tree[x].right].max);
    else if(Tree[x].key == Tree[Tree[x].right].max && Tree[x].key > Tree[Tree[x].left].max)
        Tree[x].sec = MAX(Tree[Tree[x].left].max, Tree[Tree[x].right].sec);
    else if(Tree[x].key == Tree[Tree[x].left].max && Tree[x].key == Tree[Tree[x].right].max)
        Tree[x].sec = MAX(Tree[Tree[x].left].sec, Tree[Tree[x].right].sec);
    else if(Tree[Tree[x].left].max == Tree[Tree[x].right].max)
        Tree[x].sec = MAX(Tree[x].key, MAX(Tree[Tree[x].left].sec, Tree[Tree[x].right].sec));
    else if(Tree[Tree[x].left].max > Tree[Tree[x].right].max)
        Tree[x].sec = MAX(Tree[Tree[x].left].sec, MAX(Tree[x].key, Tree[Tree[x].right].max));
    else
        Tree[x].sec = MAX(Tree[Tree[x].right].sec, MAX(Tree[x].key, Tree[Tree[x].left].max));
    Tree[x].count_max = (Tree[x].max == Tree[x].key) + (Tree[x].max == Tree[Tree[x].left].max) * Tree[Tree[x].left].count_max + (Tree[x].max == Tree[Tree[x].right].max) * Tree[Tree[x].right].count_max;

    // printf("res: %d\n", Tree[x].sec);
}

void LazyRev(int Cur){
    int tmp = Tree[Cur].left;
    Tree[Cur].left = Tree[Cur].right;
    Tree[Cur].right = tmp;
    Tree[Cur].rev ^= 1;
}

void LazyCeil(int Cur, ll k){
    if(Cur == NIL)
        return;
    if(k >= Tree[Cur].max)
        return;
    Tree[Cur].key = MIN(Tree[Cur].key, k);
    Tree[Cur].sum = Tree[Cur].sum - Tree[Cur].count_max * (Tree[Cur].max - k);
    Tree[Cur].max = k;
    Tree[Cur].ceil = k;
    return;
}

void Spread(int Cur){
    if(Tree[Cur].rev){
        Tree[Cur].rev = 0;
        LazyRev(Tree[Cur].left);
        LazyRev(Tree[Cur].right);
    }
    if(Tree[Cur].ceil != INF){
        LazyCeil(Tree[Cur].left, Tree[Cur].ceil);
        LazyCeil(Tree[Cur].right, Tree[Cur].ceil);
        Tree[Cur].ceil = INF;
    }
}

void Split(int Rank, int Cur, int *a, int *b){
    Spread(Cur);

    if(Cur == NIL)
        *a = *b = NIL;
    else if(Tree[Tree[Cur].left].size + 1 <= Rank){
        *a = Cur;
        Split(Rank - Tree[Tree[Cur].left].size - 1, Tree[Cur].right, &(Tree[(*a)].right), b);
        Update(Cur);
    }
    else{
        *b = Cur;
        Split(Rank, Tree[Cur].left, a, &(Tree[(*b)].left));
        Update(Cur);
    }
}

int Merge(int TreeL, int TreeR){

    Spread(TreeL);
    Spread(TreeR);

    if(TreeL == NIL)
        return TreeR;
    if(TreeR == NIL)
        return TreeL;

    if(Tree[TreeL].prio > Tree[TreeR].prio){
        Tree[TreeL].right = Merge(Tree[TreeL].right, TreeR);
        Update(TreeL);
        return TreeL;
    }
    Tree[TreeR].left = Merge(TreeL, Tree[TreeR].left);
    Update(TreeR);
    return TreeR;
}

typedef struct stack{
    int mynode;
    struct stack *next;
} stack;

int Build(ll Arr[], int N){

    if(N == 0)
        return NIL;

    stack *S = NULL;

    for(int i = 0; i < N; i++){
        int newnode = NewNode(Arr[i]);

        stack *newS = (stack *) malloc(sizeof(stack));
        newS->next = NULL, newS->mynode = newnode;

        int Last = NIL;
        while(S != NULL && Tree[newnode].prio > Tree[S->mynode].prio){
            Last = S->mynode;
            Update(Last);
            S = S->next;
        }
        if(S != NULL)
            Tree[S->mynode].right = newnode;
        Tree[newnode].left = Last;
        Update(newnode);
        newS->next = S;
        S = newS;
    }
    while(S->next != NULL){
        Update(S->mynode);
        S = S->next;
    }
    Update(S->mynode);
    return S->mynode;
}

// 1
void Insert(int rank, ll key){

    // Insert in the place larger than rank nodes

    int newnode = NewNode(key);

    if(Root == NIL)
        Root = newnode;
    else{
        int a, b;
        Split(rank, Root, &a, &b);
        Root = Merge(Merge(a, newnode), b);
    }
}

// 2
void Delete(int Rank){
    int a, b, c, tmp;
    Split(Rank - 1, Root, &a, &tmp);
    Split(1, tmp, &b, &c);

    Root = Merge(a, c);
}

// 3
void Reverse(int l, int r){
    int a, b, c, tmp;
    Split(l - 1, Root, &a, &tmp);
    Split(r - l + 1, tmp, &b, &c);
    LazyRev(b);
    Root = Merge(Merge(a, b), c);
}

// 4
void SwapBlock(int l, int r, int x, int y){
    if(l > x){
        Swap(&l, &x);
        Swap(&r, &y);
    }
    int a, b, c, d, e, tmp1, tmp2, tmp3;
    Split(r, Root, &tmp1, &tmp2);
    Split(l - 1, tmp1, &a, &b);
    Split(x - r - 1, tmp2, &c, &tmp3);
    Split(y - x + 1, tmp3, &d, &e);

    Root = Merge(Merge(Merge(a, d), c), Merge(b, e));
}

// 5
int CurStep;
void CeilRecur(int Cur, ll k){
    if(Cur == NIL)
        return;

    Spread(Cur);
    if(Tree[Cur].max <= k)
        return;
    if(Tree[Cur].sec < k){
        LazyCeil(Cur, k);
        return;
    }

    Tree[Cur].key = MIN(Tree[Cur].key, k);
    CeilRecur(Tree[Cur].left, k);
    CeilRecur(Tree[Cur].right, k);
    Update(Cur);
}

void Ceil(int l, int r, ll k){
    int a, b, c, tmp;
    Split(l - 1, Root, &a, &tmp);
    Split(r - l + 1, tmp, &b, &c);

    CeilRecur(b, k);

    Root = Merge(Merge(a, b), c);
}


// 6
ll RangeSum(int l, int r, int Cur){
    int a, b, c, tmp;
    Split(l - 1, Root, &a, &tmp);
    Split(r - l + 1, tmp, &b, &c);
    ll res = Tree[b].sum;
    Root = Merge(Merge(a, b), c);
    return res;
}

int CeilDiv(int x){ return x/2 + x%2;}

void Solve(){

    Preset();

    int N = NextInt(), Q = NextInt();

    ll Arr[MAX_N];
    for(int i = 0; i < N; i++){
        Arr[i] = NextInt();
    }

    Root = Merge(Build(Arr, CeilDiv(N)), Build(Arr + CeilDiv(N), N - CeilDiv(N)));
    // Print(1, Root);
    for(int i = 0; i < Q; i++){
        int t;
        CurStep = i;
        // printf("i = %d:\n", i);
        t = NextInt();
        switch(t){
            case 1:{
                int p = NextInt();
                ll k = NextInt();
                Insert(p, k);
                break;
            }
            case 2:{
                int p = NextInt();
                Delete(p);
                break;
            }
            case 3:{
                int l = NextInt(), r = NextInt();
                Reverse(l, r);
                break;
            }
            case 4:{
                int l = NextInt(), r = NextInt(), x = NextInt(), y = NextInt();
                SwapBlock(l, r, x, y);
                break;
            }
            case 5:{
                int l = NextInt(), r = NextInt(), k = NextInt();
                Ceil(l, r, k);
                break;
            }
            case 6:{
                int l = NextInt(), r = NextInt();
                printf("%lld\n", RangeSum(l, r, Root));
                break;
            }
        }
        // Print(1, Root);
    }
}


int main()
{
    srand(487635200);
    Solve();
}