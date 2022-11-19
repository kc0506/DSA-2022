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
int shift_arr[MAX_L];

typedef struct node{int hash_value; struct node *next;} node;
typedef struct element{int index, count, ignore; struct element *next;} Element;

int to_travel[PRIME + 100] = {0};
node *changed_value_head = 0, *changed_value_tail = 0;
Element *hash_table[PRIME + 100] = {0};

int hash(const char *s){
    int len = strlen(s), res = 0;
    for(int i = 0; i < len; i++){
        res = res * BASE + (s[i] - NULLCHR);
        res %= PRIME;
    }
    return res;
}

int hash1(const char *s){
    int res = 0;
    for(int i=0; i<l; i++){
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
    string_arr = (char **) malloc(sizeof(char *) * k);
    for(int i = 0; i < k; i++)
        string_arr[i] = (char *) malloc(sizeof(char) * l);

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

void solve(){

    k = next_int(), l = next_int(), flag = next_int();

    set_up();

    /// for(int i=0; i<l; i++)
        // printf("%d\n", shift_arr[i]);

    // char s[20] = {'q', 'w', 'e', 'r'}, s1[20] = {NULLCHR, 'w', 'e', 'r'}, s2[20] = {'q', NULLCHR, 'e', 'r'}, s3[20] = {'q', 'w', NULLCHR, 'r'}, s4[20] = {'q', 'w', 'e', NULLCHR};
    // printf("hash(qwer) = %d\n", hash1(s));
    // printf("hash(*wer) = %d\n", hash1(s1));
    // printf("hash(q*er) = %d\n", hash1(s2));
    // printf("hash(qw*r) = %d\n", hash1(s3));
    // printf("hash(qwe*) = %d\n", hash1(s4));

    int true_res = 0, subtract = 0;
    int index1 = -1, index2 = -1;

    for(int i = 0; i < k; i++){
        scanf("%s", string_arr[i]);

        // O(l)
        int hash_value = hash(string_arr[i]);
        if(hash_table[hash_value] == NULL){
            Element *new_ele = (Element *) malloc(sizeof(Element));
            new_ele->count = 1, new_ele->ignore = -1, new_ele->index = i, new_ele->next = NULL;
            hash_table[hash_value] = new_ele;
        }
        else{
            Element *cur = hash_table[hash_value], *prev = NULL;
            while(cur != NULL){
                if(compare(cur, i, -1)){
                    index1 = cur->index, index2 = i;
                    subtract += cur->count++;
                    if(to_travel[hash_value] == 0){
                        to_travel[hash_value] = 1;
                        node *new_node = (node *) malloc(sizeof(node));
                        new_node->hash_value = hash_value, new_node->next = NULL;
                        if(changed_value_head == 0 && changed_value_tail == 0){
                            changed_value_head = changed_value_tail = new_node;
                        }
                        else{
                            changed_value_tail->next = new_node;
                            changed_value_tail = new_node;
                        }
                    }
                    break;
                }
                prev = cur, cur = cur->next;
            }
            if(cur == NULL){
                Element *new_ele = (Element *) malloc(sizeof(Element));
                new_ele->count = 1, new_ele->ignore = -1, new_ele->index = i, new_ele->next = NULL;
                prev->next = new_ele;
            }
        }

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
                new_ele->count = 1, new_ele->ignore = j, new_ele->index = i, new_ele->next = NULL;
                hash_table[hash_value] = new_ele;
            }
            else{
                Element *cur = hash_table[hash_value], *prev = NULL;
                while(cur != NULL){
                    if(compare(cur, i, j)){
                        index1 = cur->index, index2 = i;
                        true_res += cur->count++;
                        if(to_travel[hash_value] == 0){
                            to_travel[hash_value] = 1;
                            node *new_node = (node *) malloc(sizeof(node));
                            new_node->hash_value = hash_value, new_node->next = NULL;
                            if(changed_value_head == 0 && changed_value_tail == 0){
                                changed_value_head = changed_value_tail = new_node;
                            }
                            else{
                                changed_value_tail->next = new_node;
                                changed_value_tail = new_node;
                            }
                        }
                        break;
                    }
                    prev = cur, cur = cur->next;
                }
                if(cur == NULL){
                    Element *new_ele = (Element *) malloc(sizeof(Element));
                    new_ele->count = 1, new_ele->ignore = j, new_ele->index = i, new_ele->next = NULL;
                    prev->next = new_ele;
                }
            }
        }
    }

    // print_hash_table();

    if(index1 == -1 && index2 == -1){
        printf("No\n");
    }
    else if(flag == 0){
        printf("Yes\n%d %d\n", index1, index2);
    }
    else{

        // suicide_re();

        printf("Yes\n");

        // node *cur = changed_value_head;
        // while(cur != NULL){
        //     Element *iter = hash_table[cur->hash_value];
        //     while(iter != NULL){
        //         // printf("count = %d\n", iter->count);

        //         true_res += iter->count * (iter->count - 1) * (iter->ignore != -1) / 2;
        //         subtract += iter->count * (iter->count - 1) * (iter->ignore == -1) / 2;
        //         iter = iter->next;
        //     }
        //     cur = cur->next;
        // }
        // printf("subtract = %d\n", subtract);

        true_res -= subtract * (l - 1);
        printf("%d\n", true_res);
    }
}

int main()
{
    solve();
}
