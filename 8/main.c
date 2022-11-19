#include <stdio.h>
#include <string.h>


#define MAX(a, b) ((a > b) ?a :b)
#define MIN(a, b) ((a < b) ?a :b)
#define MAX_L ((int) 1e7 + 200)
#define MAX_C (0x7E)

void Suicide1(){
    system("pause");
}

void Suicide2(){
    int *a = NULL;
    *a = 2;
}

void SwapC(char *a, char *b){
    *b = *a + *b - (*a = *b);
}

char InputS[MAX_L] = {0}, RevS[MAX_L];

void RevStr(char *S){
    int Len = strlen(S);
    char *p = S, *q = S + Len - 1;
    while(p < q)
        SwapC(p++, q--);
}

void PrefixCompute(const char *S, int Len, int Pi[]){
    int CurMatch = 0;
    Pi[0] = 0;
    for(int i = 1; i < Len; i++){
        while(CurMatch > 0 && S[CurMatch] != S[i])
            CurMatch = Pi[CurMatch - 1];
        if(S[CurMatch] == S[i])
            CurMatch++;
        Pi[i] = CurMatch;
    }
}

int IsPalindrome(char *S){
    int Len = strlen(S), left = 0, right = Len - 1, flag = 1;
    while(left < right){
        if(S[left++] != S[right--])
            flag = 0;
    }
    return flag;
}


int PiFront[MAX_L * 2] = {0};
char LargeSFront[MAX_L * 2];

char test1[MAX_L], test2[MAX_L];

int AddFront(){
    int Len = strlen(InputS);

    strcpy(LargeSFront, InputS);
    LargeSFront[Len] = 0x20;
    strcat(LargeSFront, RevS);
    PrefixCompute(LargeSFront, 2 * Len + 1, PiFront);

    int Bound = PiFront[2 * Len];
    strncpy(test1, InputS, Bound);

    return Len - Bound;
}

int PiBack[MAX_L * 2] = {0};
char LargeSBack[MAX_L * 2];
int AddBack(){
    int Len = strlen(InputS);

    strcpy(LargeSBack, RevS);
    LargeSBack[Len] = 0x20;
    strcat(LargeSBack, InputS);
    PrefixCompute(LargeSBack, 2 * Len + 1, PiBack);

    int Bound = PiBack[2 * Len];

    strcpy(test2, InputS + Len - Bound);

    return Len - Bound;
}

void Solve(){

    int AnsFront = AddFront(), AnsBack = AddBack(), Len = strlen(InputS);
    if(AnsFront == 0 && AnsBack == 0){
        printf("0\n");
        printf("%s\n", InputS);
    }
    else{

        printf("%d\n", MIN(AnsBack, AnsFront));

        if(AnsFront <= AnsBack){
            Suicide1();
            for(int i = 0; i < AnsFront; i++)
                printf("%c", RevS[i]);
            printf("%s\n", InputS);
        }
        if(AnsBack <= AnsFront){
            printf("%s", InputS);
            for(int i = Len - AnsBack; i < Len; i++)
                printf("%c", RevS[i]);
            printf("\n");
        }
    }
}

int main()
{
    scanf("%s", InputS);

    strcpy(RevS, InputS);
    RevStr(RevS);

    Solve();
}