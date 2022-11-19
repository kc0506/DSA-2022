#include <stdio.h>
#include <stdlib.h>

#define INF ((int) 1e9 + 100)
#define MIN(a, b) ((a < b) ?a :b)

void Swap(int *a, int *b){
    *b = *a + *b - (*a = *b);
}

typedef struct node{
    int size, key, sum, rev, ceil;
    struct node *p, *left, *right;
} node;

node *Root, *NIL;

void Preset(){
    NIL = (node *) malloc(sizeof(node));
    NIL->key = 0, NIL->size = 0, NIL->sum = 0,
    NIL->rev = 0, NIL->ceil = INF;
    NIL->p = NIL->left = NIL->right = NIL;
    Root = NIL;
}

void Update(node *x){
    x->sum = x->left->sum + x->right->sum + x->key;
    x->size = x->left->size + x->right->size + 1;
}

void LazyRev(node *Cur){
    node *tmp = Cur->left;
    Cur->left = Cur->right;
    Cur->right = tmp;
    Cur->rev ^= 1;
}

void LazyCeil(node *Cur, int k){
    Cur->key = MIN(Cur->key, k);
    Cur->ceil = k;
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

void Print(int index, node *cur){
    if(cur == NIL){
        printf("%d: NIL\n", index);
        return;
    }
    printf("%d:\n \tkey = %d\n\tsize = %d\n\tsum = %d\n", index, cur->key, cur->size, cur->sum);
    Spread(cur);
    Print(index * 2, cur->left);
    Print(index * 2 + 1, cur->right);
}

node *Merge(node *TreeL, node *TreeR){

    Spread(TreeL);
    Spread(TreeR);

    // Print(1, TreeL); Print(1, TreeR);

    if(TreeL == NIL)
        return TreeR;
    if(TreeR == NIL)
        return TreeL;

    TreeL->right = Merge(TreeL->right, TreeR);
    Update(TreeL);
    return TreeL;
}

void Inorder(node *cur){
    if(cur == NIL)
        return;
    Inorder(cur->left);
    printf("%d ", cur->key);
    Inorder(cur->right);
}

// 1
void Insert(int rank, int key){

    // Insert in the place larger than rank nodes

    node *newnode = (node *) malloc(sizeof(node));
    newnode->key = key, newnode->sum = key,
    newnode->rev = 0, newnode->ceil = INF;
    newnode->p = newnode->left = newnode->right = NIL,
    newnode->size = 1;

    if(Root == NIL)
        Root = newnode;
    else{
        int cur_rank = rank, lorr;
        node *prev = NIL, *cur = Root;
        while(cur != NIL){
            prev = cur;
            if(cur_rank <= cur->left->size)
                cur = cur->left, lorr = 0;
            else
                cur_rank -= (cur->left->size + 1),
                cur = cur->right,
                lorr = 1;
        }
        newnode->p = prev;
        if(!lorr)
            prev->left = newnode;
        else
            prev->right = newnode;
        while(prev != NIL){
            prev->sum += key;
            prev->size++;
            prev = prev->p;
        }
    }
}


// 2
node *SearchByRank(int Rank, node *Cur){
    if(Rank - 1 == Cur->left->size)
        return Cur;

    if(Rank <= Cur->left->size)
        return SearchByRank(Rank, Cur->left);
    return SearchByRank(Rank - Cur->left->size - 1, Cur->right);
}

node *Minimum(node *x){
    while(x->left != NIL)
        x = x->left;
    return x;
}

node *Maximum(node *x){
    while(x->right != NIL)
        x = x->right;
    return x;
}

void Transplant(node *u, node *v){
    if(u->p == NIL)
        Root = v;
    else if(u == u->p->left)
        u->p->left = v;
    else
        u->p->right = v;
    v->p = u->p;
    Update(v);
}

void Delete(int Rank){

    // if(Root == NIL)
    //     return;

    // node *z = SearchByRank(Rank, Root), *y = z;

    // if(z->left == NIL){
    //     Transplant(z, z->right);
    // }
    // else if(z->right == NIL){
    //     Transplant(z, z->left);
    // }
    // else{
    //     y = Minimum(z->right);

    //     if(y != z->right){
    //         Transplant(y, y->right);
    //         y->right = z->right;
    //         y->right->p = y;
    //         Update(y);
    //     }
    //     Transplant(z, y);
    //     y->left = z->left;
    //     y->left->p = y;
    //     Update(y);
    // }

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
void Ceil(int l, int r, int k){
    node *a, *b, *c, *tmp;
    Split(l - 1, Root, &a, &tmp);
    Split(r - l + 1, tmp, &b, &c);

    LazyCeil(b, k);

    Root = Merge(Merge(a, b), c);
}


// 6
int RangeSum(int l, int r, node *Cur){

    // if(Cur->size == 1 || (l == r && l == Cur->left->size + 1))
    //     return Cur->key;

    // if(l == 1 && r == Cur->size)
    //     return Cur->sum;

    // if(r <= Cur->left->size)
    //     return RangeSum(l, r, Cur->left);
    // if(r == Cur->left->size + 1)
    //     return RangeSum(l, r - 1, Cur->left) + Cur->key;
    // if(l == Cur->left->size + 1)
    //     return RangeSum(1, r - Cur->left->size - 1, Cur->right) + Cur->key;
    // if(l > Cur->left->size + 1)
    //     return RangeSum(l - Cur->left->size - 1, r - Cur->left->size - 1, Cur->right);

    // int SumL = RangeSum(l, Cur->left->size, Cur->left),
    //     SumR = RangeSum(1, r - Cur->left->size - 1, Cur->right);
    // return SumL + Cur->key + SumR;

    node *a, *b, *c, *tmp;
    Split(l - 1, Root, &a, &tmp);
    Split(r - l + 1, tmp, &b, &c);
    int res = b->sum;
    Root = Merge(Merge(a, b), c);
    return res;
}


void Solve(){

    Preset();

    int N, Q;
    scanf("%d%d", &N, &Q);

    int a;
    for(int i = 0; i < N; i++){
        scanf("%d", &a);
        Insert(i, a);
    }

    // printf("before Q:\n");
    // Print(1, Root);

    for(int i = 0; i < Q; i++){
        int t;
        scanf("%d", &t);
        switch(t){
            case 1:{
                int p, k;
                scanf("%d%d", &p, &k);
                Insert(p, k);
                break;
            }
            case 2:{
                int p;
                scanf("%d", &p);
                Delete(p);
                break;
            }
            case 3:{
                int l, r;
                scanf("%d%d", &l, &r);
                Reverse(l, r);
                break;
            }
            case 4:{
                int l, r, x, y;
                scanf("%d%d%d%d", &l, &r, &x, &y);
                SwapBlock(l, r, x, y);
                break;
            }
            case 5:{
                int l, r, k;
                scanf("%d%d%d", &l, &r, &k);
                Ceil(l, r, k);
                break;
            }
            case 6:{
                int l, r;
                scanf("%d%d", &l, &r);
                printf("%d\n", RangeSum(l, r, Root));
                break;
            }
        }
        // printf("round %d:\n", i+1);
        // Inorder(Root);printf("\n");
    }
}

#define debug 0


int main()
{
    if(debug){
        Preset();
        Insert(0, 2);
        // Print(1, Root);
        Insert(0, 1);
        // Print(1, Root);
        Insert(2, 3);
        // Print(1, Root);
        Insert(3, 5);
        // Print(1, Root);
        Insert(2, 4);
        // Print(1, Root);
        Insert(2, 9);
        Insert(0, 0);

        Print(1, Root);

        SwapBlock(2, 3, 5, 6);
        Print(1, Root);

        return 0;

        Inorder(Root); printf("\n");

        for(int l = 1; l <= 6; l++)
            for(int r = l; r <= 6; r++)
                printf("[%d, %d] = %d\n", l, r, RangeSum(l, r, Root));
    }
    else{
        Solve();
    }
}