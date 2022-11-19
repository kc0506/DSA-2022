#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_N ((int) 1e5 + 100)
#define MAX_Q ((int) 1e5 + 100)
#define MIN(a, b) ((a < b) ?a :b)
#define MAX(a, b) ((a > b) ?a :b)
#define Red 0
#define Black 1


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

typedef struct node{
    int key, color;
    struct node *p, *left, *right;
} Node;

Node *Root, *NIL;

void printc(int x){
    printf("%c\n", "RB"[x == Black]);
}

void Recur(int index, Node *x){
    if(x != NIL)
        printf("%d: %d (%c)\n", index, x->key, "RB"[x->color == Black]);
    else
        printf("%d: NIL (B)\n", index);
    if(x != NIL){
        Recur(index * 2, x->left);
        Recur(index * 2 + 1, x->right);
    }
}

void PrintTree(){
    Recur(1, Root);
}

void TreePreset(){
    NIL = (Node *) malloc(sizeof(Node));
    NIL->p = NIL->left = NIL->right = NIL, NIL->color = Black, NIL->key = 0;
    Root = NIL;
}

Node *FindKey(int key){
    Node *cur = Root;
    while(cur != NIL){
        if(key == cur->key)
            return cur;
        if(key > cur->key)
            cur = cur->right;
        else
            cur = cur->left;
    }
    return NIL;
}

Node *Minimum(Node *x){
    while(x->left != NIL)
        x = x->left;
    return x;
}

Node *Maximum(Node *x){
    while(x->right != NIL)
        x = x->right;
    return x;
}

Node *Predecessor(Node *x){
    if(x->left != NIL)
        return Maximum(x->left);
    Node *y = x->p;
    while(y != NIL && x == y->left)
        x = y, y = y->p;
    return y;
}

Node *Sucessor(Node *x){
    if(x->right != NIL)
        return Minimum(x->right);
    Node *y = x->p;
    while(y != NIL && x == y->right)
        x = y, y = y->p;
    return y;
}

void Transplant(Node *u, Node *v){
    if(u->p == NIL)
        Root = v;
    else if(u == u->p->left)
        u->p->left = v;
    else
        u->p->right = v;
    v->p = u->p;
}

void RotateL(Node *x){
    if(x->right == NIL)
        return;

    Node *y = x->right;

    x->right = y->left;
    if(y->left != NIL)
        y->left->p = x;

    y->p = x->p;
    if(y->p == NIL)
        Root = y;
    else if(x == x->p->left)
        x->p->left = y;
    else
        x->p->right = y;

    y->left = x;
    x->p = y;

    return;
}

void RotateR(Node *x){

    if(x->left == NIL)
        return;

    Node *y = x->left;

    x->left = y->right;
    if(y->right != NIL)
        y->right->p = x;

    y->p = x->p;
    if(y->p == NIL)
        Root = y;
    else if(x == x->p->left)
        x->p->left = y;
    else
        x->p->right = y;

    y->right = x;
    x->p = y;
    return;
}

void InsertFix(Node *z){

    while(z->p->color == Red){
        if(z->p == z->p->p->left){
            Node *y = z->p->p->right;
            if(y->color == Red){
                z->p->color = Black;
                y->color = Black;
                z->p->p->color = Red;
                z = z->p->p;
            }
            else{
                if(z == z->p->right){
                    z = z->p;
                    RotateL(z);
                }
                z->p->color = Black;
                z->p->p->color = Red;
                RotateR(z->p->p);
                break;
            }
        }
        else{
            Node *y = z->p->p->left;
            if(y->color == Red){
                z->p->color = Black;
                y->color = Black;
                z->p->p->color = Red;
                z = z->p->p;
            }
            else{
                if(z == z->p->left){
                    z = z->p;
                    RotateR(z);
                }
                z->p->color = Black;
                z->p->p->color = Red;
                RotateL(z->p->p);
                break;
            }
        }
    }
    Root->color = Black;
}

void Insert(int key){
    Node *z = (Node *) malloc(sizeof(Node));
    z->color = Red, z->key = key, z->left = z->right = z->p = NIL;


    Node *prev = NIL, *cur = Root;
    while(cur != NIL){
        prev = cur;
        if(z->key < cur->key)
            cur = cur->left;
        else
            cur = cur->right;
    }


    z->p = prev;
    if(prev == NIL)
        Root = z;
    else if(z->key < prev->key)
        prev->left = z;
    else
        prev->right = z;

    InsertFix(z);
    return;
}

void DeleteFIx(Node *x){
    // x: 缺了一個黑色的位置

    while(x != Root && x->color == Black){
        if(x == x->p->left){
            Node *w = x->p->right;
            assert(w != NIL);

            if(w->color == Red){
                // case 1
                w->color = Black;
                x->p->color = Red;
                RotateL(x->p);
                w = x->p->right;
            }
            if(w->left->color == Black && w->right->color == Black){
                w->color = Red;
                x = x->p;
            }
            else{
                if(w->right->color == Black){
                    w->color = Red;
                    w->left->color = Black;
                    RotateR(w);
                    w = x->p->right;
                }
                w->color = x->p->color;
                x->p->color = Black;
                w->right->color = Black;
            }
        }
        else{
            Node *w = x->p->left;
            assert(w != NIL);

            if(w->color == Red){
                // case 1
                w->color = Black;
                x->p->color = Red;
                RotateR(x->p);
                w = x->p->right;
            }
            if(w->right->color == Black && w->left->color == Black){
                w->color = Red;
                x = x->p;
            }
            else{
                if(w->left->color == Black){
                    w->color = Red;
                    w->right->color = Black;
                    RotateL(w);
                    w = x->p->left;
                }
                w->color = x->p->color;
                x->p->color = Black;
                w->left->color = Black;
            }
        }
    }
    x->color = Black;
}

void DeleteKey(int key){

    Node *z = FindKey(key);
    if(z == NIL)
        return;

    Node *x, *y = z;
    int yColor = y->color;

    if(z->left == NIL){
        x = z->right;
        Transplant(z, z->right);
    }
    else if(z->right == NIL){
        x = z->left;
        Transplant(z, z->left);
    }
    else{
        y = Minimum(z->right);
        yColor = y->color;
        x = y->right;

        if(y == z->right)
            x->p = y;
        else{
            Transplant(y, y->right);
            y->right = z->right;
            y->right->p = y;
        }
        Transplant(z, y);
        y->left = z->left;
        y->left->p = y;
        y->color = z->color;
    }

    if(yColor == Black)
        DeleteFIx(x);
}


int N, Q;

int main()
{
    TreePreset();

    scanf("%d%d", &N, &Q);

    for(int i = 1; i <= N; i++)
        scanf("%d", &(Cost[i]));

    int PreSum[MAX_N] = {0};
    for(int i = 1; i <= N; i++)
        PreSum[i] = PreSum[i - 1] + Cost[i];

    for(int i = 0; i < Q; i++){
        int op, p, k, l, r;
        scanf("%d%d%d", &op);

        switch(op){
            case 1:
                scanf("%d%d", &p, &k);
                break;
            case 6:
                break;
        }

        printf("%d\n", PreSum[r] - PreSum[l - 1]);
    }
}


