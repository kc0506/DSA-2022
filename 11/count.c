#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#define INF ((int) 1e9 + 100)
#define MIN(a, b) ((a < b) ?a :b)
#define MAX(a, b) ((a > b) ?a :b)
#define MAX_N ((int) 1e6 + 100)

int cur_step;

typedef long long int ll;

void SuicideSE(){ time(NULL);}

int NextInt(){
    int c = getchar(), num = 0;
    while(!isdigit(c))
        c = getchar();
    while(isdigit(c))
        num = num * 10 + c - '0', c = getchar();
    return num;
}

void Swap(int *a, int *b){
    *b = *a + *b - (*a = *b);
}

typedef struct node{
    ll sum, key, max, min;
    int size, rev, prio, count;
    struct node *p, *left, *right;
} node;

node *Root, *NIL;

node *NewNode(int key){
    node *newnode = (node *) malloc(sizeof(node));
    newnode->key = key, newnode->sum = key, newnode->max = key, newnode->min = key;
    newnode->size = 1, newnode->rev = 0, newnode->prio = rand() % INF, newnode->count = 1;
    newnode->p = newnode->left = newnode->right = NIL;
    return newnode;
}

void Preset(){
    NIL = (node *) malloc(sizeof(node));
    NIL->key = 0, NIL->sum = 0, NIL->max = 0, NIL->min = INF;
    NIL->size = 0, NIL->rev = 0, NIL->prio = -1, NIL->count = 0;
    NIL->p = NIL->left = NIL->right = NIL;
    Root = NIL;
}

void Update(node *x, int comb){
    if(x == NIL)
        return;
    x->sum = x->left->sum + x->right->sum + x->key * x->count;
    x->size = x->left->size + x->right->size + x->count;
    x->max = MAX(MAX(x->left->max, x->key), x->right->max);
    x->min = MIN(MIN(x->left->min, x->key), x->right->min);


    if(!comb)
        return;

    if(x->key == x->left->key){
        if(x->left->right == NIL)
            x->count = x->count + x->left->count, x->left = x->left->left;
    }
    if(x->key == x->right->key){
        if(x->right->left == NIL)
            x->count = x->count + x->right->count, x->right = x->right->right;
    }
}

void Print(int index, node *cur){
    if(cur == NIL){
        printf("%d: NIL\n", index);
        return;
    }
    Print(index * 2, cur->left);
    printf("%d:\n \tkey = %d\n\tsize = %d\n\tcount = %d\n\tsum = %lld\n\tmax = %lld\n\tmin = %d\n", index, cur->key, cur->size, cur->count, cur->sum, cur->max, cur->min);
    Print(index * 2 + 1, cur->right);
}


void LazyRev(node *Cur){
    node *tmp = Cur->left;
    Cur->left = Cur->right;
    Cur->right = tmp;
    Cur->rev ^= 1;
}

void Spread(node *Cur){
    if(Cur->rev){
        Cur->rev = 0;
        LazyRev(Cur->left);
        LazyRev(Cur->right);
    }
}


void Split(int Rank, node *Cur, node **a, node **b){

    // 將樹分為 <= Rank, > Rank
    Spread(Cur);
    if(Cur == NIL){
        *a = *b = NIL;
        return;
    }
    if(Cur->left->size + Cur->count <= Rank){
        *a = Cur;
        Split(Rank - Cur->left->size - Cur->count, Cur->right, &((*a)->right), b);
        Update(Cur, 1);
        return;
    }
    if(Cur->left->size >= Rank){
        *b = Cur;
        Split(Rank, Cur->left, a, &((*b)->left));
        Update(Cur, 1);
        return;
    }
    node *CurL = NewNode(Cur->key);
    CurL->count = Rank - Cur->left->size;
    CurL->left = Cur->left;
    Cur->left = CurL;
    Cur->count -= CurL->count;
    Update(Cur->left, 0);
    Update(Cur, 0);

    *b = Cur;
    Split(Rank, Cur->left, a, &((*b)->left));
    Update(Cur, 1);
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
        Update(TreeL, 1);
        return TreeL;
    }
    TreeR->left = Merge(TreeL, TreeR->left);
    Update(TreeR, 1);
    return TreeR;
}

void Inorder(node *cur){
    if(cur == NIL)
        return;
    Inorder(cur->left);
    for(int i=0; i<cur->count; i++)
        printf("%d ", cur->key);
    Inorder(cur->right);
}

typedef struct stack{
    node *mynode;
    struct stack *next;
} stack;

int cmp(node *a, node *b){
    return a->prio > b->prio;
}

node *Build(ll Arr[], int N){

    if(N == 0)
        return NIL;

    stack *S = NULL;

    for(int i = 0; i < N; i++){
        node *newnode = NewNode(Arr[i]);

        stack *newS = (stack *) malloc(sizeof(stack));
        newS->next = NULL, newS->mynode = newnode;

        node *Last = NIL;
        while(S != NULL && cmp(newnode, S->mynode)){
            Last = S->mynode;
            Update(Last, 1);
            S = S->next;
        }
        if(S != NULL)
            S->mynode->right = newnode;
        newnode->left = Last;
        Update(newnode, 1);
        newS->next = S;
        S = newS;
    }
    while(S->next != NULL){
        Update(S->mynode, 1);
        S = S->next;
    }
    Update(S->mynode, 1);
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

void SpreadCeil(node *Cur, int ceil){
    if(Cur == NIL)
        return;

    if(Cur->max <= ceil)
        return;

    Cur->key = MIN(Cur->key, ceil);

    SpreadCeil(Cur->left, ceil);
    SpreadCeil(Cur->right, ceil);

    Update(Cur, 1);
}

void CeilRecur(node *Cur, int k){
    if(Cur == NIL || Cur->max <= k)
        return;

    if(Cur->min > k){
        Cur->key = k;
        Cur->count = Cur->size;
        Cur->left = Cur->right = NIL;
        Update(Cur, 0);
        return;
    }

    Cur->key = MIN(Cur->key, k);
    CeilRecur(Cur->left, k);
    CeilRecur(Cur->right, k);
    Update(Cur, 1);
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
    // Inorder(Root);printf("\n");
    for(int i = 0; i < Q; i++){
        int t;
        cur_step = i;
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
        // Inorder(Root);printf("\n");
        // Inorder(Root);printf("\n");
    }
}


int main()
{
    // clock_t start = clock();
    srand(741448763);
    Solve();
    // printf("%lf\n", (double) (clock() - start) / CLOCKS_PER_SEC);
}