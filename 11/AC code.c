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
    while(c < '0' || c > '9')
        c = nc();
    while(c >= '0' && c <= '9')
        num = num * 10 + c - '0', c = nc();
    return num;
}

void Swap(int *a, int *b){
    *b = *a + *b - (*a = *b);
}

typedef struct node{
    ll key, sum, max, sec, ceil;
    int size, rev, prio, count_max;
    struct node *p, *left, *right;
} node;

node *Root, *NIL;

void Preset(){
    NIL = (node *) malloc(sizeof(node));
    NIL->key = 0;
    NIL->sum = 0;
    NIL->max = 0;
    NIL->sec = 0;
    NIL->ceil = INF;
    NIL->size = 0;
    NIL->rev = 0;
    NIL->prio = -1;
    NIL->count_max = 0;
    NIL->p = NIL->left = NIL->right = NIL;
    Root = NIL;
}

node *NewNode(ll key){
    node *newnode = (node *) malloc(sizeof(node));
    newnode->key = key;
    newnode->sum = key;
    newnode->max = key;
    newnode->sec = 0;
    newnode->ceil = INF;
    newnode->size = 1;
    newnode->rev = 0;
    newnode->prio = rand();
    newnode->count_max = 1;
    newnode->p = newnode->left = newnode->right = NIL;
    return newnode;
}

void Inorder(node *cur){
    if(cur == NIL)
        return;
    Inorder(cur->left);
    printf("%d ", cur->key);
    Inorder(cur->right);
}

void Print(int index, node *cur){
    if(cur == NIL){
        printf("%d: NIL\n", index);
        return;
    }
    Print(index * 2, cur->left);
    printf("%d:\n \tkey = %d\n\tsize = %d\n\tsum = %d\n\tceil = %lld\n", index, cur->key, cur->size, cur->sum, cur->ceil);
    printf("\tmax = %d\n\tmax_count = %d\n\tsec = %d\n", cur->max, cur->count_max, cur->sec);
    Print(index * 2 + 1, cur->right);
}

void Update(node *x){
    if(x == NIL)
        return;
    x->sum = x->left->sum + x->right->sum + x->key;
    x->size = x->left->size + x->right->size + 1;
    x->max = MAX(MAX(x->left->max, x->key), x->right->max);

    // printf("%d, (%d, %d), (%d, %d)\n", x->key, x->left->max, x->left->sec, x->right->max, x->right->sec);

    if(x->key > x->left->max && x->key > x->right->max)
        x->sec = MAX(x->left->max, x->right->max);
    else if(x->key == x->left->max && x->key > x->right->max)
        x->sec = MAX(x->left->sec, x->right->max);
    else if(x->key == x->right->max && x->key > x->left->max)
        x->sec = MAX(x->left->max, x->right->sec);
    else if(x->key == x->left->max && x->key == x->right->max)
        x->sec = MAX(x->left->sec, x->right->sec);
    else if(x->left->max == x->right->max)
        x->sec = MAX(x->key, MAX(x->left->sec, x->right->sec));
    else if(x->left->max > x->right->max)
        x->sec = MAX(x->left->sec, MAX(x->key, x->right->max));
    else
        x->sec = MAX(x->right->sec, MAX(x->key, x->left->max));
    x->count_max = (x->max == x->key) + (x->max == x->left->max) * x->left->count_max + (x->max == x->right->max) * x->right->count_max;

    // printf("res: %d\n", x->sec);
}

void LazyRev(node *Cur){
    node *tmp = Cur->left;
    Cur->left = Cur->right;
    Cur->right = tmp;
    Cur->rev ^= 1;
}

void LazyCeil(node *Cur, ll k){
    if(Cur == NIL)
        return;
    if(k >= Cur->max)
        return;
    Cur->key = MIN(Cur->key, k);
    Cur->sum = Cur->sum - Cur->count_max * (Cur->max - k);
    Cur->max = k;
    Cur->ceil = k;
    return;
}

void Spread(node *Cur){
    if(Cur->rev){
        Cur->rev = 0;
        LazyRev(Cur->left);
        LazyRev(Cur->right);
    }
    if(Cur->ceil != INF){
        LazyCeil(Cur->left, Cur->ceil);
        LazyCeil(Cur->right, Cur->ceil);
        Cur->ceil = INF;
    }
}

void Split(int Rank, node *Cur, node **a, node **b){
    Spread(Cur);

    if(Cur == NIL)
        *a = *b = NIL;
    else if(Cur->left->size + 1 <= Rank){
        *a = Cur;
        Split(Rank - Cur->left->size - 1, Cur->right, &((*a)->right), b);
        Update(Cur);
    }
    else{
        *b = Cur;
        Split(Rank, Cur->left, a, &((*b)->left));
        Update(Cur);
    }
}

node *Merge(node *TreeL, node *TreeR){

    Spread(TreeL);
    Spread(TreeR);

    if(TreeL == NIL)
        return TreeR;
    if(TreeR == NIL)
        return TreeL;

    if(TreeL->prio > TreeR->prio){
        TreeL->right = Merge(TreeL->right, TreeR);
        Update(TreeL);
        return TreeL;
    }
    TreeR->left = Merge(TreeL, TreeR->left);
    Update(TreeR);
    return TreeR;
}

typedef struct stack{
    node *mynode;
    struct stack *next;
} stack;

node *Build(ll Arr[], int N){

    if(N == 0)
        return NIL;

    stack *S = NULL;

    for(int i = 0; i < N; i++){
        node *newnode = NewNode(Arr[i]);

        stack *newS = (stack *) malloc(sizeof(stack));
        newS->next = NULL, newS->mynode = newnode;

        node *Last = NIL;
        while(S != NULL && newnode->prio > S->mynode->prio){
            Last = S->mynode;
            Update(Last);
            S = S->next;
        }
        if(S != NULL)
            S->mynode->right = newnode;
        newnode->left = Last;
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

    node *newnode = NewNode(key);

    if(Root == NIL)
        Root = newnode;
    else{
        node *a, *b;
        Split(rank, Root, &a, &b);
        Root = Merge(Merge(a, newnode), b);
    }
}

// 2
void Delete(int Rank){
    int n = Root->size;
    node *a, *b, *c, *tmp;
    Split(Rank - 1, Root, &a, &tmp);
    Split(1, tmp, &b, &c);

    Root = Merge(a, c);
}

// 3
void Reverse(int l, int r){
    node *a, *b, *c, *tmp;
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
    node *a, *b, *c, *d, *e, *tmp1, *tmp2, *tmp3;
    Split(r, Root, &tmp1, &tmp2);
    Split(l - 1, tmp1, &a, &b);
    Split(x - r - 1, tmp2, &c, &tmp3);
    Split(y - x + 1, tmp3, &d, &e);

    Root = Merge(Merge(Merge(a, d), c), Merge(b, e));
}

// 5
int CurStep;
void CeilRecur(node *Cur, ll k){
    if(Cur == NIL)
        return;

    Spread(Cur);
    if(Cur->max <= k)
        return;
    if(Cur->sec < k){
        LazyCeil(Cur, k);
        return;
    }

    Cur->key = MIN(Cur->key, k);
    CeilRecur(Cur->left, k);
    CeilRecur(Cur->right, k);
    Update(Cur);
}

void Ceil(int l, int r, ll k){
    node *a, *b, *c, *tmp;
    Split(l - 1, Root, &a, &tmp);
    Split(r - l + 1, tmp, &b, &c);

    CeilRecur(b, k);

    Root = Merge(Merge(a, b), c);
}


// 6
ll RangeSum(int l, int r, node *Cur){
    node *a, *b, *c, *tmp;
    Split(l - 1, Root, &a, &tmp);
    Split(r - l + 1, tmp, &b, &c);
    ll res = b->sum;
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
    srand(741448763);
    Solve();
}