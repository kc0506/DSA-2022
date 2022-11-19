#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct disjointSet{
    // TODO: Determine fields to use by your method
    int parent, count;
} DisjointSet;

DisjointSet ds[1 << 24];
bool set[1 << 24] = {};

int c2i(char c) {
    if ('0' <= c && c <= '9') return c - '0';
    else if ('a' <= c && c <= 'z') return c - 'a' + 10;
    else if ('A' <= c && c <= 'Z') return c - 'A' + 36;
    return -1;
}

int hash(const char* s) {
    int ret = 0;
    int mask = (1<<24)-1;
    int len = strlen(s);
    for (int i = 0; i < len; i++)
        ret = mask & (ret << 4) | c2i(s[i]);
    return ret;
}

void makeset(const char* s){
    // TODO: Initialize a set with hash value
    int i = hash(s);
    ds[i].parent = -1;
    ds[i].count = 0;
    return;
}

inline void static init(const char* s) {
    int i = hash(s);
    if (!set[i]) {
        makeset(s);
        set[i] = 1;
    }
}

int find_set_by_index(int index){
    if(ds[index].parent == -1)
        return index;
    return ds[index].parent = find_set_by_index(ds[index].parent);
}

int find_set(const char* s) {
    init(s);
    int i = hash(s);
    // TODO: Implement your find algorithm here

    return find_set_by_index(i);
}

void group(const char *ra, const char *rb) {
    int a = find_set(ra), b = find_set(rb);
    // TODO: Implement your union algorithm here

    if(a == b)
        return;

    if(ds[a].count < ds[b].count)
        ds[a].parent = b;
    else if(ds[a].count > ds[b].count)
        ds[b].parent = a;
    else
        ds[a].parent = b, ds[a].count++;
    return;
}

bool same_set(const char*a, const char* b) {
    // TODO: Implement your algorithm here
    int ra = find_set(a), rb = find_set(b);
    return ra == rb;
}

int main() {
    // TODO: Implement your algorithm here

    int N;
    scanf("%d", &N);

    char input[20];
    for(int i = 0; i < N; i++){
        scanf("%s", input);

        char A[20], B[20];
        scanf("%s%s", A, B);

        if(input[0] == 'g'){
            group(A, B);
        }
        else{
            bool positive = same_set(A, B);
            if(positive)
                printf("Positive\n");
            else
                printf("Negative\n");
        }
    }

}