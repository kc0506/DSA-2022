#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX(a, b) ((a > b) ?a :b)
#define MIN(a, b) ((a < b) ?a :b)
#define MAX_L ((int) 1e6 + 100)
#define MAX_K ((int) 1e6 + 100)
#define MIN_ASCII (0x21)
#define MAX_ASCII (0x7E)
#define PRIME 12582917

int next_int(){
    int c = getchar(), num = 0;
    while(!isdigit(c))
        c = getchar();
    while(isdigit(c))
        num = num * 10 + c - '0', c = getchar();
    return num;
}

int k, l, flag;

int *Index, *Index_new;

char **sort(char **S){
    int C[MAX_ASCII + 10];
    char **S_new = (char **) malloc(sizeof(char *) * k);
    for(int i = 0; i < k; i++)
        S_new[i] = (char *) malloc(sizeof(char) * l);

    Index = (int *) malloc(sizeof(int) * k), Index_new = (int *) malloc(sizeof(int) * k);
    for(int i = 0; i < k; i++)
        Index[i] = i;

    char **S_prev = (char **) malloc(sizeof(char *) * k);
    for(int i = 0; i < k; i++)
        S_prev[i] = (char *) malloc(sizeof(char) * l);

    for(int i = l - 1; i >= 0; i--){

        for(int j = MIN_ASCII; j <= MAX_ASCII; j++)
            C[j] = 0;

        for(int j = 0; j < k; j++)
            C[S[j][i]]++;

        for(int j = MIN_ASCII + 1; j <= MAX_ASCII; j++)
            C[j] += C[j - 1];

        // for(int j = MIN_ASCII; j <= MAX_ASCII; j++)
        //     printf("%c: %d\n", j, C[j]);
        // printf("\n");

        for(int j = k - 1; j >= 0; j--){
            strcpy(S_new[--C[S[j][i]]], S[j]);
            Index_new[C[S[j][i]]] = Index[j];
        }

        for(int j = 0; j < k; j++){
            strcpy(S[j], S_new[j]);
            Index[j] = Index_new[j];
        }

    }

    // for(int i =0; i<k;i++)
    //     printf("%s\n", S_new[i]);

    return S_new;
}

void solve(){
    k = next_int(), l = next_int(), flag = next_int();
    char **S = (char **) malloc(sizeof(char *) * k);
    for(int i = 0; i < k; i++){
        S[i] = (char *) malloc(sizeof(char *) * l);
        scanf("%s", S[i]);
    }

    sort(S);

    int ans1 = -1, ans2 = -1;
    for(int i = 0; i < k - 1; i++){
        if(strcmp(S[i], S[i + 1]) == 0)
            ans1 = Index_new[i], ans2 = Index_new[i + 1];
    }
    if(ans1 == -1)
        printf("No\n");
    else
        printf("Yes\n%d %d\n", MIN(ans1, ans2), MAX(ans1, ans2));
}

int main()
{
    solve();
}
