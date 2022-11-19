#include <stdio.h>
#include <string.h>
#include <assert.h>

typedef struct BigInt
{
    int len;
    int digits[300];
} BigInt;

void print(BigInt a)
{
    for(int i = a.len - 1; i >= 0; i--)
        printf("%d", a.digits[i]);
    printf("\n");
}

void swap(BigInt *a, BigInt *b)
{
    BigInt tmp = *a;
    *a = *b;
    *b = tmp;
    return;
}

BigInt set(const char *num)
{
    BigInt tmp;
    int l = strlen(num);

    for(int i = 0; i < 300; i++)
        tmp.digits[i] = 0;

    for(int i = l - 1; i >= 0; i--){
        tmp.digits[l - i - 1] = num[i] - '0';
    }
    tmp.len = l;
    return tmp;
}

int even(BigInt a)
{
    return a.digits[0] % 2 == 0;
}

int zero(BigInt a)
{
    return a.len == 1 && a.digits[0] == 0;
}

int cmp(BigInt a, BigInt b)
{
    if(a.len > b.len)
        return 1;
    if(a.len < b.len)
        return -1;
    for(int i = a.len - 1; i >= 0; i--)
        if(a.digits[i] > b.digits[i])
            return 1;
        else if(a.digits[i] < b.digits[i])
            return -1;
    return 0;
}

BigInt div(BigInt *a, int x)
{
    int carrier = 0;
    for(int i = a->len - 1; i >= 0; i--){
        a->digits[i] += carrier;
        carrier = (a->digits[i] % x) * 10;
        a->digits[i] /= x;
    }
    if(a->digits[a->len - 1] == 0)
        a->len = a->len - 1;
    return *a;
}

BigInt mul(BigInt *a, int x)
{
    int carrier = 0;
    for(int i = 0; i < a->len; i++){
        a->digits[i] *= x;
        a->digits[i] += carrier;
        carrier = a->digits[i] / 10;
        a->digits[i] %= 10;
    }
    if(carrier > 0){
        a->digits[a->len] = carrier;
        a->len = a->len + 1;
    }
    return *a;
}

BigInt sub(BigInt *a, BigInt *b)
{
    assert(cmp(*a, *b) >= 0);
    for(int i = 0; i < b->len; i++){
        a->digits[i] -= b->digits[i];
        if(a->digits[i] < 0){
            a->digits[i] += 10;
            a->digits[i + 1] = a->digits[i + 1] - 1;
        }
    }
    for(int i = b->len; i < a->len; i++)
        if(a->digits[i] < 0){
            a->digits[i] += 10;
            a->digits[i + 1] = a->digits[i + 1] - 1;
        }
    while(a->digits[a->len - 1] == 0 && a->len > 1)
        a->len -= 1;
    return *a;
}

int round = 0;

BigInt gcd(BigInt a, BigInt b)
{
    int ans = 0;
    if(cmp(a, b) < 0)
        swap(&a, &b);
    while(!zero(a) && !zero(b)){
        if(even(a) && even(b)){
            ans++;
            div(&a, 2);
            div(&b, 2);
        }
        else if(even(a))
            div(&a, 2);
        else if(even(b))
            div(&b, 2);
        else;
        if(cmp(a, b) < 0)
            swap(&a, &b);
        sub(&a, &b);
    }
    for(int i = 0; i < ans; i++)
        mul(&b, 2);
    return b;
}

int main()
{
    char a[300], b[300];
    scanf("%s%s", a, b);
    print(gcd(set(a), set(b)));
}