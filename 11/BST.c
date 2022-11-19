#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <assert.h>

#define INF ((int) 1e9 + 100)
#define MIN(a, b) ((a < b) ?a :b)
#define MAX(a, b) ((a > b) ?a :b)
#define MAX_N ((int) 1e6 + 100)

#define DEBUG (0)

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
void Swap(int *a, int *b){ *b = *a + *b - (*a = *b);}
int CeilDiv(int x){ return x/2 + x%2;}


int CurStep;

typedef struct node{
    ll sum, key, ceil, max, ass;
    int size, rev, prio;
    struct node *p, *left, *right, *BST;
} node;

node *Root, *NIL;

// Debug系列
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
    printf("%d:\n\tprio = %x\n\tkey = %d\n\tsize = %d\n\tsum = %d\n\tmax = %lld\n", index, cur->prio, cur->key, cur->size, cur->sum, cur->max);
    printf("\tceil = %d\n", cur->ceil);
    printf("\tass = %d\n", cur->ass);
    printf("\tBST: "); Inorder(cur->BST); printf("\n");
    Print(index * 2 + 1, cur->right);
}

// 初始化
void Preset(){
    NIL = (node *) malloc(sizeof(node));
    NIL->sum = NIL->key = NIL->max = 0;
    NIL->ceil = NIL->ass = INF;
    NIL->size = 0;
    NIL->rev = 0;
    NIL->prio = -1;
    NIL->BST = NIL->p = NIL->left = NIL->right = NIL;
    Root = NIL;
}

node *NewNode(ll key){
    node *newnode = (node *) malloc(sizeof(node));
    newnode->sum = newnode->key = newnode->max = key;
    newnode->ceil = newnode->ass = INF;
    newnode->size = 1;
    newnode->rev = 0;
    newnode->prio = rand();
    newnode->BST = newnode->p = newnode->left = newnode->right = NIL;
    return newnode;
}

// 各種功能
void UpdateValue(node *x){
    // 當x的左右都維護好後來維護x
    if(x == NIL)
        return;
    x->sum = x->left->sum + x->right->sum + x->key;
    x->size = x->left->size + x->right->size + 1;
    x->max = MAX(MAX(x->left->max, x->key), x->right->max);
}

void LazyAss(node *Cur, ll k){
    if(Cur == NIL)
        return;
    Cur->key = k;
    Cur->sum = k * Cur->size;
    Cur->ass = k;
}

void SpreadBST(node *Cur){
    if(Cur->ass != INF){
        LazyAss(Cur->left, Cur->ass);
        LazyAss(Cur->right, Cur->ass);
        Cur->ass = INF;
    }
}

void SplitByKey(ll key, node *Cur, node **L, node **R){
    // For BST
    // 分成兩棵樹：一棵全部 <= key，一棵全部 > key
    if(Cur == NIL){
        *L = *R = NIL;
        return;
    }

    // 因為我們接著要分割Cur，所以要先spread
    SpreadBST(Cur);
    if(Cur->key <= key){
        // Cur在左樹，即a
        *L = Cur;
        SplitByKey(key, Cur->right, &(Cur->right), R);
        // 因為Cur.right可能被改變，所以要維護Cur
        UpdateValue(Cur);
    }
    else{
        // Cur在右樹
        *R = Cur;
        SplitByKey(key, Cur->left, L, &(Cur->left));
        UpdateValue(Cur);
    }
}

node *MergeBST(node *L, node *R){
    // For BST
    // L的key <= R的key

    SpreadBST(L);
    SpreadBST(R);

    if(DEBUG && 0){
        printf("L:\n");
        Print(1, L);
        printf("R:\n");
        Print(1, R);
    }

    if(L == NIL)
        return R;
    if(R == NIL)
        return L;

    // 因為接著會分割，所以先spread


    // 決定誰是根
    if(L->prio > R->prio){
        L->right = MergeBST(L->right, R);
        UpdateValue(L);
        return L;
    }
    R->left = MergeBST(L, R->left);
    UpdateValue(R);
    return R;
}

node *MergeBSTGen(node *L, node *R){
    // For BST
    // L 不一定皆 < R
    if(L == NIL)
        return R;
    if(R == NIL)
        return L;

    SpreadBST(L);
    SpreadBST(R);
    if(L->prio > R->prio){
        // L當根
        node *a, *b;
        SplitByKey(L->key, R, &a, &b);

        // 因為L, a, b 依舊沒有必然的<，所以需要繼續遞迴
        L->left = MergeBSTGen(L->left, a);
        L->right = MergeBSTGen(L->right, b);
        UpdateValue(L);
        return L;
    }
    node *a, *b;
    SplitByKey(R->key, L, &a, &b);
    R->left = MergeBSTGen(a, R->left);
    R->right = MergeBSTGen(b, R->right);
    UpdateValue(R);
    return R;
}

node *CeilBST(node *Cur, ll k){
    // For BST
    // 將Cur 中大於k的值改為k後回傳

    // 首先以k分割
    node *L, *R;
    SplitByKey(k, Cur, &L, &R);

    // 接著將R改為全k的樹
    LazyAss(R, k);


    // 接著要合併兩棵樹
    return MergeBST(L, R);
}

node *CopyBST(node *Cur){
    // For BST
    if(Cur == NIL)
        return NIL;

    node *newnode = NewNode(Cur->key);

    newnode->left = CopyBST(Cur->left);
    newnode->right = CopyBST(Cur->right);

    UpdateValue(newnode);
    newnode->ass = Cur->ass;
    newnode->prio = Cur->prio;
    return newnode;
}

void UpdateBST(node *x){
    // For Treap
    // 假設小孩的BST及key皆為正確
    x->BST = MergeBSTGen(MergeBSTGen(CopyBST(x->left->BST), NewNode(x->key)), CopyBST(x->right->BST));
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

    // For Treap
    // 讓Cur的key, sum符合ceil後的結果
    // 為此需要維護Cur->BST


    Cur->BST = CeilBST(Cur->BST, k);
    Cur->key = MIN(Cur->key, k);
    Cur->sum = Cur->BST->sum;
    if(DEBUG && 0){
        printf("HEreeeeeeeeeeeeee:\n");
        Inorder(Cur); printf("\n");
        Print(1, Cur->BST);
    }

    // 需要留下一個懶標記，因為子樹還沒做
    Cur->ceil = MIN(Cur->ceil, k);
}

void SpreadTreap(node *Cur){
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

void SplitBySize(ll Rank, node *Cur, node **L, node **R){
    // 分為 <= Rank 和 > Rank 的兩棵樹
    if(Cur == NIL){
        *L = *R = NIL;
        return;
    }

    SpreadTreap(Cur);
    if(Cur->left->size + 1 <= Rank){
        // Cur在L
        *L = Cur;
        SplitBySize(Rank - Cur->left->size - 1, Cur->right, &(Cur->right), R);

        // Cur的右邊可能被改，所以除了value以外也要合併子樹
        // 由於有經過spread，所以左右的BST應皆為正確
        UpdateValue(Cur);
        UpdateBST(Cur);
    }
    else{
        // Cur在R
        *R = Cur;
        SplitBySize(Rank, Cur->left, L, &(Cur->left));
        UpdateValue(Cur);
        UpdateBST(Cur);
    }
}

node *MergeTreap(node *L, node *R){
    if(L == NIL)
        return R;
    if(R == NIL)
        return L;

    // L < R (order)
    SpreadTreap(L);
    SpreadTreap(R);

    if(L->prio > R->prio){
        // L為根
        L->right = MergeTreap(L->right, R);

        // 因為L的right會變，所以要維護，包含BST
        UpdateValue(L);
        // 由於有spread過，可以保證子樹的BST是正確的
        UpdateBST(L);
        return L;
    }
    // R為根
    R->left = MergeTreap(L, R->left);
    UpdateValue(R);
    UpdateBST(R);
    return R;
}


// 5
void RangeCeil(int l, int r, ll k){
    node *L, *M, *R, *tmp;
    SplitBySize(l - 1, Root, &L, &tmp);
    SplitBySize(r - l + 1, tmp, &M, &R);

    // M是我們要改的目標
    LazyCeil(M, k);

    Root = MergeTreap(MergeTreap(L, M), R);
}

// 6
ll RangeSum(int l, int r){
    node *L, *M, *R, *tmp;
    SplitBySize(l - 1, Root, &L, &tmp);
    SplitBySize(r - l + 1, tmp, &M, &R);

    ll res = M->BST->sum;

    assert(res == M->sum);

    Root = MergeTreap(MergeTreap(L, M), R);
    return res;
}


// Build
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

    // 建一個stack裝最右側一排的點
    stack *S = NULL;
    for(int i = 0; i < N; i++){
        node *newnode = NewNode(Arr[i]);
        newnode->BST = NewNode(Arr[i]);

        stack *newS = (stack *) malloc(sizeof(stack));
        newS->next = NULL, newS->mynode = newnode;

        node *Last = NIL;
        while(S != NULL && cmp(newnode, S->mynode)){
            Last = S->mynode;
            UpdateValue(Last);
            UpdateBST(Last);
            S = S->next;  // 相當於pop
        }

        // 最後一個pop出來的就是newnode的left
        newnode->left = Last;
        UpdateValue(newnode);
        UpdateBST(newnode);
        // printf("i = %d\n", i);
        // Print(1, newnode);
        if(S != NULL){
            S->mynode->right = newnode;
            UpdateValue(S->mynode);
            UpdateBST(S->mynode);
            // Print(1, S->mynode);
        }

        newS->next = S;
        S = newS;
    }
    while(S->next != NULL){
        UpdateValue(S->mynode);
        UpdateBST(S->mynode);
        S = S->next;
    }
    UpdateValue(S->mynode);
    UpdateBST(S->mynode);
    return S->mynode;
}


void Solve(){

    Preset();

    int N = NextInt(), Q = NextInt();

    ll Arr[MAX_N];
    for(int i = 0; i < N; i++){
        Arr[i] = NextInt();
    }
    Root = Build(Arr, N);
    // Root = MergeTreap(Build(Arr, CeilDiv(N)), Build(Arr + CeilDiv(N), N - CeilDiv(N)));
    // Print(1, Root);
    // Inorder(Root); printf("\n");
    for(int i = 0; i < Q; i++){
        CurStep = i;
        int t;
        // printf("%d:\n", i);
        t = NextInt();
        switch(t){
            case 5:{

                int l = NextInt(), r = NextInt(), k = NextInt();
                RangeCeil(l, r, k);
                break;
            }
            case 6:{
                int l = NextInt(), r = NextInt();
                printf("%lld\n", RangeSum(l, r));
                break;
            }
        }
        // Inorder(Root); printf("\n");
        // return;
        // Print(1, Root);
    }
}


int main()
{
    srand(741448763);
    Solve();
}