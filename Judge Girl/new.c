#include <stdio.h>
#include <string.h>

typedef struct employee {
  int id;
  char first_name[32];
  char last_name[32];
  int boss_id;
} E;

int N;
E Employ[50];

int find_by_id(int id){
    for(int i=0; i<N; i++)
        if(Employ[i].id == id)
            return i;
}

int find_by_name(char *first, char *last){
    for(int i=0; i<N; i++)
        if(strcmp(Employ[i].first_name, first) == 0 && strcmp(Employ[i].last_name, last) == 0)
            return i;
}

int find_boss(int index, int object){
    if(index == object)
        return index;
    if(Employ[index].boss_id == Employ[index].id)
        return index;
    return find_by_id(Employ[index].boss_id);
}

int main()
{
    scanf("%d\n", N);
    for(int i=0; i<N; i++){
        scanf("%d ", Employ[i].id);
        scanf("%s ", Employ[i].first_name);
        scanf("%s ", Employ[i].last_name);
        scanf("%s ", Employ[i].boss_id);
    }

    int M;
    scanf("%d\n", M);
    for(int i=0; i<M; i++){
        char nameA[2][20], nameB[2][20];
        scanf("%s %s %s %s", nameA[0], nameA[1], nameB[0], nameB[1]);

        int A = find_by_name(nameA[0], nameA[1]), B = find_by_name(nameB[0], nameB[1]);
        int bossA = find_boss(A, B), bossB = find_boss(B, A);
        if(bossA == B)
            printf("subordinate\n");
        else if(bossB == A)
            printf("supervisor\n");
        else if(bossA == bossB)
            printf("colleague\n");
        else
            printf("unrelated\n");
    }
}