#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>

#define MAX_LEN 2000000
#define OPERATOR "+-*/\%()"
#define MAX_LL 9223372036854775807
#define MIN_LL -9223372036854775807

typedef long long int ll;

char oper[MAX_LEN];
int top_oper = -1;

void push_oper(char c){
    // printf("push: %c\n", c);
    top_oper++;
    oper[top_oper] = c;
}

int is_empty_oper(){
    return top_oper == -1;
}

int pop_oper(){
    assert(!is_empty_oper());
    top_oper--;
    return oper[top_oper + 1];
}

ll postfix[MAX_LEN][2];  // 0 for num; 1 for oper
int top_post = -1;

void print(){
    for(int i = 0; i <= top_post; i++)
        if(postfix[i][1])
            printf("%c", postfix[i][0]);
        else
            printf("%d", postfix[i][0]);
    printf("\n");
}

void push_post(ll token, int type){
    top_post++;
    postfix[top_post][0] = token;
    postfix[top_post][1] = type;
}

void process_oper(char cur_oper){

    int prior[256];
    prior[0] = -1;
    prior['+'] = prior['-'] = 0;
    prior['*'] = prior['/'] = prior['%'] = 1;
    prior['('] = prior[')'] = 2;

    // printf("cur_oper = %c\n", cur_oper);
    // print();
    // printf("\n");

    switch(prior[cur_oper]){
        case -1:
        case 0:
        case 1: {
            int order = prior[cur_oper];
            while(!is_empty_oper()){
                if(prior[oper[top_oper]] < order)
                    break;
                if(oper[top_oper] == '(')
                    break;
                push_post(pop_oper(), 1);
            }
            if(cur_oper != '\0')
                push_oper(cur_oper);
            break;
        }
        case 2: {
            while(!is_empty_oper() && cur_oper == ')'){
                if(oper[top_oper] == '('){
                    pop_oper();
                    break;
                }
                push_post(pop_oper(), 1);
            }
            if(cur_oper == '(')
                push_oper('(');
            break;
        }
    }
    return;
}

void to_postifx(char *input){
    // printf("input = %s\n", input);
    int total_len = strlen(input);

    ll sign0 = 1;
    if(input[0] == '-'){
        sign0 = -1;
        input++;
        total_len--;
    }
    // printf("(new) input = %s\n", input);
    char *start = input;
    while(start <= input + total_len){
        int dis = strcspn(start, OPERATOR);
        // 將 [start, start + dis] 轉為 數字，為此先將start + dis + 1設為'\0'
        ll cur_num;
        char *cur_pos = start, cur_oper = start[dis];
        start += dis;
        *start = '\0';
        cur_num = atoll(cur_pos);
        cur_num *= sign0;
        sign0 = 1;
        // printf("dis = %d\n", dis);
        // printf("%d: %d, %c\n\n", start - input, cur_num, cur_oper);

        // 將 cur_num 推入 postfix ， 處理 oper
        if(dis != 0)
            push_post(cur_num, 0);
        // if(cur_oper != '\0')
        process_oper(cur_oper);

        // 設定下一輪
        start++;
    }
}

ll floor_div(ll a, ll b){
    ll ans = 0;

}

ll compute(ll a, ll b, char op){
    switch(op){
        case '+':
            assert(a + b - b == a);
            return a + b;
        case '-':
            assert(a - b + b == a);
            return a - b;
        case '*':
            assert((a * b) / b == a);
            return a * b;
        case '/':{
            ll x = a / b;
            if(x * b != a && ((a < 0 && b > 0) || (a > 0 && b < 0))){
                return x - 1;
                // assert(0);
            }
            return x;
        }
        case '%':
            assert(a > 0 && b > 0);
            return a % b;
    }
    return 0;
}

ll eval[MAX_LEN];
int top_eval = -1;

int is_empty_eval(){
    return top_eval == -1;
}

ll pop_eval(){
    assert(!is_empty_eval());
    top_eval--;
    return eval[top_eval + 1];
}

void push_eval(ll x){
    top_eval++;
    eval[top_eval] = x;
}

void print_eval(){
    for(int i=0; i<=top_eval; i++)
        printf("%d ", eval[i]);
    printf("\n");
}

void reset(){
    top_oper = -1;
    top_post = -1;
    top_eval = -1;
}

ll solve(char *input){
    // printf("input = %s\n", input);
    // printf("oper: %d, post: %d, eval: %d\n", top_oper, top_post, top_eval);
    // Step 1. convert to postfix
    if(strlen(input) == 0)
        return 0;

    to_postifx(input);
    // print();
    // Step 2. conduct postfix
    for(int i = 0; i <= top_post; i++){
        if(postfix[i][1] == 0)
            push_eval(postfix[i][0]);
        else{
            ll b = pop_eval(), a = pop_eval();
            // printf("a = %d, b = %d, op = %c\n", a, b, postfix[i][0]);
            push_eval(compute(a, b, postfix[i][0]));
        }
        // print_eval();
    }
    assert(top_oper == -1);
    top_post = -1;
    assert(top_eval == 0);
    ll ans = pop_eval();
    return ans;
}

int main()
{
    char c, input[MAX_LEN] = {0}, tmp[20];
    int len = 0;
    while((c = getchar()) != EOF){
        if(c == '='){
            input[len] = '\0';
            sprintf(input, "%lld", solve(input));
            len = strlen(input);
            printf("Print: %s\n", input);
        }
        else if(isprint(c)){
            input[len] = c;
            len++;
        }
    }
}