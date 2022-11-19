#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>

#define MAX(a, b) ((a > b) ?a :b)
#define MIN(a, b) ((a < b) ?a :b)
#define MAX_L ((int) 1e6 + 100)
#define MAX_K ((int) 1e6 + 100)
#define MIN_ASCII (0x21)
#define MAX_ASCII (0x7E)
#define NULLCHR (MIN_ASCII - 1)
#define BASE (MAX_ASCII - NULLCHR + 1)
#define PRIME 12582917
#define SAME 1
#define DEBUG 0

int next_int(){
    int c = getchar(), num = 0;
    while(!isdigit(c))
        c = getchar();
    while(isdigit(c))
        num = num * 10 + c - '0', c = getchar();
    return num;
}

void suicide_re(){
    int *a = NULL;
    *a = 1;
}

int k, l, flag;

char **string_arr;
unsigned long long int shift_arr[MAX_L];

typedef struct node{int hash_value; struct node *next;} node;
typedef struct element{int index, count, ignore; struct element *next;} Element;

int *weight;
Element *hash_table[PRIME + 100] = {0};

int hash(const char *s){
    int len = strlen(s), res = 0;
    for(int i = 0; i < len; i++){
        res = res * BASE + (s[i] - NULLCHR);
        res %= PRIME;
    }
    return res;
}

int compare(Element *ele, int index, int ignore){
    if(ele->ignore != ignore)
        return 0;
    if(ele->index == index)
        return 1;
    for(int i = 0; i < l; i++){
        if(i == ignore)
            continue;
        if(string_arr[ele->index][i] != string_arr[index][i])
            return 0;
    }
    return 1;
}

void set_up(){
    weight = (int *) malloc(sizeof(int) * k);
    for(int i = 0; i < k; i++)
        weight[i] = 0;

    string_arr = (char **) malloc(sizeof(char *) * (k + 1));
    for(int i = 0; i < k; i++)
        string_arr[i] = (char *) malloc(sizeof(char) * (l + 1));

    shift_arr[0] = 1;
    for(int i = 1; i < l; i++)
        shift_arr[i] = shift_arr[i - 1] * BASE % PRIME;
}

void print_hash_table(){
    for(int i = 0; i < PRIME; i++){
        if(hash_table[i] == NULL)
            continue;
        printf("hash_value = %d:\n", i);
        Element *cur = hash_table[i];
        while(cur != NULL){
            printf("\tignore %d:\n", cur->ignore);
            printf("\t\t");
            for(int j=0; j<l; j++)
                if(j == cur->ignore)
                    printf("#");
                else
                    printf("%c", string_arr[cur->index][j]);
            printf("\n");
            cur = cur->next;
        }
    }
}

int true_res = 0, subtract = 0, same = 0, changed = 0;
int index1 = -1, index2 = -1;

void merge_same(){
    for(int i = 0; i < k; i++){
        int hash_value = hash(string_arr[i]);
        if(hash_table[hash_value] == NULL){
            Element *new_ele = (Element *) malloc(sizeof(Element));
            new_ele->count = 1, new_ele->ignore = -1, new_ele->index = i, new_ele->next = NULL;
            hash_table[hash_value] = new_ele;
            weight[i]++;
        }
        else{
            Element *cur = hash_table[hash_value], *prev = NULL;
            while(cur != NULL){
                if(compare(cur, i, -1)){
                    index1 = cur->index, index2 = i;
                    same += cur->count++;
                    weight[cur->index]++;
                    break;
                }
                prev = cur, cur = cur->next;
            }
            if(cur == NULL){
                Element *new_ele = (Element *) malloc(sizeof(Element));
                new_ele->count = 1, new_ele->ignore = -1, new_ele->index = i, new_ele->next = NULL;
                prev->next = new_ele;
                weight[i]++;
            }
        }
    }
}

void print_string(){
    for(int i = 0; i < k; i++){
        if(weight[i]){
            printf("i = %d\n", i);
            printf("\t%s x %d\n", string_arr[i], weight[i]);
        }
    }
}

void debug(){
    k = 100, l = 5000, flag = 1;

    set_up();

    for(int i = 0; i < 50; i++){
        for(int j = 0; j < l; j++)
            string_arr[i][j] = (j % BASE + (j % BASE == 0)) + NULLCHR;
        string_arr[i][l] = '\0';
    }
    for(int i = 50; i < k; i++){
        string_arr[i][0] = 'x';
        for(int j = 1; j < l; j++)
            string_arr[i][j] = (j % BASE + (j % BASE == 0)) + NULLCHR;
        string_arr[i][l] = '\0';
    }
}

void print_ans(){

    if(index1 == -1 && index2 == -1){
        printf("No\n");
    }
    else if(flag == 0){
        printf("Yes\n%d %d\n", index1, index2);
    }
    else{

        // suicide_re();

        printf("Yes\n");

        if(DEBUG)
            printf("changed = %d\nsame = %d\n", changed, same);
        printf("%d\n", changed + same);
    }
}

void solve(){
    if(DEBUG)
        debug();
    else{
        k = next_int(), l = next_int(), flag = next_int();
        set_up();
        for(int i = 0; i < k; i++)
            scanf("%s", string_arr[i]);
    }

    merge_same();

    // print_string();
    // return;

    for(int i = 0; i < k; i++){
        if(weight[i] == 0)
            continue;
        // // l * O(1)

        int hash_value = hash(string_arr[i]);
        for(int j = 0; j < l; j ++){
            // ignore the j-th character
            // treat j-th character as '\0'
            if(j == 0){
                hash_value -= (string_arr[i][j] - NULLCHR) * shift_arr[l - j - 1] % PRIME;
                hash_value %= PRIME;
                hash_value = (hash_value + PRIME) % PRIME;
            }
            else{
                hash_value += (string_arr[i][j - 1] - NULLCHR) * shift_arr[l - j] % PRIME;
                hash_value %= PRIME;
                hash_value -= (string_arr[i][j] - NULLCHR) * shift_arr[l - j - 1] % PRIME;
                hash_value %= PRIME;
                hash_value = (hash_value + PRIME) % PRIME;
            }

            // printf("hash(%d) = %d\n", j, hash_value);

            if(hash_table[hash_value] == NULL){
                Element *new_ele = (Element *) malloc(sizeof(Element));
                new_ele->count = weight[i], new_ele->ignore = j, new_ele->index = i, new_ele->next = NULL;
                hash_table[hash_value] = new_ele;
            }
            else{
                Element *cur = hash_table[hash_value], *prev = NULL;
                while(cur != NULL){
                    if(compare(cur, i, j)){
                        index1 = cur->index, index2 = i;
                        changed = changed + weight[i] * cur->count;
                        cur->count += weight[i];
                        break;
                    }
                    prev = cur, cur = cur->next;
                }
                if(cur == NULL){
                    Element *new_ele = (Element *) malloc(sizeof(Element));
                    new_ele->count = weight[i], new_ele->ignore = j, new_ele->index = i, new_ele->next = NULL;
                    prev->next = new_ele;
                }
            }
        }
    }
    print_ans();

    // print_hash_table();
}

int main()
{
    solve();
}
