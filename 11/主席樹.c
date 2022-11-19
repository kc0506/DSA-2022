#include <stdio.h>
#include <stdlib.h>

#define MAX_N (((int) 1e5 + 100) << 2)
#define MAX_Q ((int) 1e5 + 100)

typedef long long int ll;

struct node{
    ll sum;
    int count, left, right;
} Tree[MAX_N << 5];

struct query{
    int t, l, r, k;
} Query[MAX_Q];

int Root[MAX_N << 5];
ll Arr[MAX_N], H[MAX_N];

int N, M, Q, top = 0;

int cmp(const void *a, const void *b){ if(*(ll *) a > *(ll *) b) return 1; return -1;}
void init(){
    int unique = 1;
    qsort(H, N, sizeof(ll), cmp);
    for(int i = 1; i < N; i++)
        if(H[i] != H[i - 1])
            unique++;
    M = unique;
}

int lower_bound(ll key, int l, int r){
    if(l == r)
        if(key <= H[l])
            return l;
        else
            return -1;

    int m = (l + r) >> 1;
    if(key <= H[m])
        return lower_bound(key, l, m);
    return lower_bound(key, m + 1, r);
}
int hash(ll key){
    return lower_bound(key, 0, N);
}

int build(int l, int r){
    // 建一棵空樹
    int cur = top++;
    Tree[cur].count = 0;
    Tree[cur].sum = 0;
    if(l == r)
        return cur;

    int m = (l + r) >> 1;
    Tree[cur].left = build(l, m);
    Tree[cur].right = build(m + 1, r);
    return cur;
}

int insert(int parent, int value, ll key, int l, int r){
    // insert value 個 key 到 parent中
    // 僅複製有被更動的節點

    // 複製一個parent
    int cur = top++;
    Tree[cur].count = Tree[parent].count + value;
    Tree[cur].sum = Tree[parent].sum + value * key;

    if(l == r)
        return cur;

    int m = (l + r) >> 1, hash_value = hash(key);
    if(key <= m){
        // 插入在左樹
        Tree[cur].left = insert(Tree[parent].left, value, key, l, m);
        Tree[cur].right = Tree[parent].right;
        return cur;
    }

    Tree[cur].left = Tree[parent].left;
    Tree[cur].right = insert(Tree[parent].right, value, key, l, m);
    return cur;
}

void input(){
    scanf("%d%d", &N, &Q);
    for(int i = 1; i <= N; i++){
        scanf("%d", Arr + i);
        H[i - 1] = Arr[i];
    }

    M = N;
    for(int i = 0; i < Q; i++){
        scanf("%d%d%d", &(Query[i].t), &(Query[i].l), &(Query[i].r));
        if(Query[i].t == 5){
            scanf("%d", &(Query[i].k));
            H[M++] = Query[i].k;
        }
        else
            Query[i].k = -1;
    }
}

ll RSQ(int l, int r){
    return Tree[Root[r]].sum - Tree[Root[l - 1]].sum;
}

int main(){
    input();

    init();

    Root[0] = build(0, M - 1);
    for(int i = 1; i <= N; i++){
        Root[i] = insert(Root[i - 1], 1, Arr[i], 0, M - 1);
    }


    for(int i = 0; i < Q; i++){
        if(Query[i].t == 5){

        }
        else{
            printf("%lld\n", RSQ(Query[i].l, Query[i].r));
        }
    }

}




