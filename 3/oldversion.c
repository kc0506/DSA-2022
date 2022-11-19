// ORIGINAL
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAX_M 1000000
#define MAX_N 1000000
#define MAX_K 1000000
#define MAX_MK 1000000

#define NEXT_TOILET 0
#define PREV_TOILET 1
#define THIS_SHOULD_NOT_HAPPEN 0
#define PRINT_GROUP 0
#define CHECK 0

int M, N, K;

typedef struct Usb{
    int id, group;
    struct Usb *neighbor_a, *neighbor_b;
} Usb;

typedef struct Group{
    int number;
    struct Group *prev, *next;
} Group;

int **toilet;
Group **toilet_head, **toilet_tail;
Usb ***group_head, ***group_tail;

void initialization(){
    // toilet
    toilet = (int **) malloc(M * sizeof(int *));
    for(int i=0; i<M; i++){
        toilet[i] = (int *) malloc(2 * sizeof(int));
        toilet[i][NEXT_TOILET] = (i - 1 + M) % M;
        toilet[i][PREV_TOILET] = (i + 1) % M;
    }

    // group
    toilet_head = (Group **) malloc(M * sizeof(Group *));
    toilet_tail = (Group **) malloc(M * sizeof(Group *));
    for(int i=0; i<M; i++)
        toilet_head[i] = toilet_tail[i] = NULL;

    // usb
    group_head = (Usb ***) malloc(M * sizeof(Usb **));
    group_tail = (Usb ***) malloc(M * sizeof(Usb **));
    for(int i=0; i<M; i++){
        group_head[i] = (Usb **) malloc(K * sizeof(Usb *));
        group_tail[i] = (Usb **) malloc(K * sizeof(Usb *));
        for(int j=0; j<K; j++)
            group_head[i][j] = group_tail[i][j] = NULL;
    }
    return;
}

void enter(int group, int id, int line){
    // Create a new usb
    Usb *new_usb = (Usb *) malloc(sizeof(Usb));
    new_usb->id = id;
    new_usb->group = group;
    new_usb->neighbor_a = new_usb->neighbor_b = NULL;

    // Check if the line is empty
    int empty_line = (toilet_tail[line] == NULL && toilet_tail[line] == NULL);
    if(empty_line){
        // Set both head and tail = new usb
        group_head[line][group] = group_tail[line][group] = new_usb;

        // Create a new group
        Group *new_group = (Group *) malloc(sizeof(Group));
        new_group->number = group;
        new_group->next = new_group->prev = NULL;

        // Set both head and tail = new group
        toilet_head[line] = toilet_tail[line] = new_group;
    }
    else{
        // Check if the group is empty
        int empty_group = (group_head[line][group] == NULL && group_tail[line][group] == NULL);
        if(empty_group){
            // Set both head and tail = new usb
            group_head[line][group] = group_tail[line][group] = new_usb;

            // Create a new group
            Group *new_group = (Group *) malloc(sizeof(Group));
            new_group->number = group;
            new_group->next = new_group->prev = NULL;

            // Concat the new group to the tail
            toilet_tail[line]->next = new_group;
            new_group->prev = toilet_tail[line];
            toilet_tail[line] = new_group;
        }
        else{
            // Concat to the group tail
            Usb *n_a = group_tail[line][group]->neighbor_a, *n_b = group_tail[line][group]->neighbor_b;
            if(n_a == NULL){  // n_a is empty, put to a
                group_tail[line][group]->neighbor_a = new_usb;
                new_usb->neighbor_b = group_tail[line][group];
            }
            else if(n_b == NULL){  // n_b is empty
                group_tail[line][group]->neighbor_b = new_usb;
                new_usb->neighbor_a = group_tail[line][group];
            }
            else
                assert(THIS_SHOULD_NOT_HAPPEN);
            group_tail[line][group] = new_usb;
        }
    }
}

void go(int line){
    // return;
    // Check if this is the last usb in the group
    int num = toilet_head[line]->number;
    int last_usb = (group_head[line][num] == group_tail[line][num]);

    if(last_usb){
        // Set both head and tail = NULL
        free(group_head[line][num]);
        group_head[line][num] = group_tail[line][num] = NULL;

        // Check if this is the last group in the line
        int last_group = (toilet_head[line] == toilet_tail[line]);
        if(last_group){
            // Set both head and tail = NULL
            free(toilet_head[line]);
            toilet_head[line] = toilet_tail[line] = NULL;
        }
        else{
            // Only need to maintain tail
            Group *new_toilet_head = toilet_head[line]->next;
            free(toilet_head[line]);
            toilet_head[line] = new_toilet_head;
            toilet_head[line]->prev = NULL;
        }
    }
    else{
        // Only need to maintain tail
        // First find the new tail
        Usb *n_a = group_head[line][num]->neighbor_a,
            *n_b = group_head[line][num]->neighbor_b,
            *tmp = group_head[line][num];
        if(n_a != NULL)  // n_a is the new tail
            group_head[line][num] = n_a;
        else if(n_b != NULL)
            group_head[line][num] = n_b;
        else
            assert(THIS_SHOULD_NOT_HAPPEN);  // becuse ! last_usb
        n_a = group_head[line][num]->neighbor_a;
        n_b = group_head[line][num]->neighbor_b;

        // Then cut down the path conneted to old tail
        if(n_a == tmp)
            group_head[line][num]->neighbor_a = NULL;
        else if(n_b == tmp)
            group_head[line][num]->neighbor_b = NULL;
        else
            assert(THIS_SHOULD_NOT_HAPPEN);
        free(tmp);
    }
    return;
}

void leave(int line){
    // return;
    // Check if this is the last usb in the group
    int num = toilet_tail[line]->number;
    int last_usb = (group_head[line][num] == group_tail[line][num]);

    if(last_usb){
        // Set both head and tail = NULL
        free(group_head[line][num]);
        group_head[line][num] = group_tail[line][num] = NULL;

        // Check if this is the last group in the line
        int last_group = (toilet_head[line] == toilet_tail[line]);
        if(last_group){
            // Set both head and tail = NULL
            free(toilet_head[line]);
            toilet_head[line] = toilet_tail[line] = NULL;
        }
        else{
            // Only need to maintain tail
            Group *new_toilet_tail = toilet_tail[line]->prev;
            free(toilet_tail[line]);
            toilet_tail[line] = new_toilet_tail;
            toilet_tail[line]->next = NULL;
        }
    }
    else{
        // Only need to maintain tail
        // First find the new tail
        Usb *n_a = group_tail[line][num]->neighbor_a,
            *n_b = group_tail[line][num]->neighbor_b,
            *tmp = group_tail[line][num];
        if(n_a != NULL)  // n_a is the new tail
            group_tail[line][num] = n_a;
        else if(n_b != NULL)
            group_tail[line][num] = n_b;
        else
            assert(THIS_SHOULD_NOT_HAPPEN);  // becuse ! last_usb
        n_a = group_tail[line][num]->neighbor_a;
        n_b = group_tail[line][num]->neighbor_b;

        // Then cut down the path conneted to old tail
        if(n_a == tmp)
            group_tail[line][num]->neighbor_a = NULL;
        else if(n_b == tmp)
            group_tail[line][num]->neighbor_b = NULL;
        else
            assert(THIS_SHOULD_NOT_HAPPEN);
        free(tmp);
    }
    return;
}

void close(int line){
    // Check if this is the last toilet
    int last_toilet = (toilet[line][NEXT_TOILET] == line);

    if(last_toilet){
        // Check is empty toilet
        int empty_toilet = (toilet_head[line] == NULL && toilet_tail[line] == NULL);
        assert(empty_toilet);
        toilet[line][NEXT_TOILET] = toilet[line][PREV_TOILET] = -1;
        return;
    }
    else{
        int new_line = toilet[line][NEXT_TOILET];

        // Start from toilet tail
        Group *group = toilet_tail[line];
        while(group != NULL){
            int num = group->number;

            // Check if group(num) is empty in the new_line
            int empty_group_in_new_line = (
                group_head[new_line][num] == NULL &&
                group_tail[new_line][num] == NULL
            );
            if(empty_group_in_new_line){
                // no need to change usb, but need to add new group

                // Create new group
                Group *new_group = (Group *) malloc(sizeof(Group));
                new_group->number = num;
                new_group->next = new_group->prev = NULL;

                // Check if the new line is empty
                int new_line_is_empty = (toilet_head[new_line] == NULL && toilet_tail[new_line] == NULL);
                if(new_line_is_empty){
                    toilet_head[new_line] = toilet_tail[new_line] = new_group;
                }
                else{
                    // Change the tail
                    toilet_tail[new_line]->next = new_group;
                    new_group->prev = toilet_tail[new_line];
                    toilet_tail[new_line] = new_group;
                }

                // Set the new head = cur tail
                group_head[new_line][num] = group_tail[line][num];
                // Set the new tail = cur head
                group_tail[new_line][num] = group_head[line][num];
                // Clear the cur head & tail
                group_head[line][num] = group_tail[line][num] = NULL;
            }
            else{
                // Concat cur tail to new tail
                // First find which of a & b (from new tail) is empty
                Usb *new_tail = group_tail[new_line][num], *cur_tail = group_tail[line][num],
                    *n_a = new_tail->neighbor_a, *n_b = new_tail->neighbor_b;
                if(n_a == NULL){
                    new_tail->neighbor_a = cur_tail;
                }
                else if(n_b == NULL){
                    new_tail->neighbor_b = cur_tail;
                }
                else
                    assert(THIS_SHOULD_NOT_HAPPEN);

                // Then find which of a & b (from cur tail) is empty
                n_a = cur_tail->neighbor_a; n_b = cur_tail->neighbor_b;
                if(n_a == NULL)
                    cur_tail->neighbor_a = new_tail;
                else if(n_b == NULL)
                    cur_tail->neighbor_b = new_tail;
                else
                    assert(THIS_SHOULD_NOT_HAPPEN);

                // Set new tail = cur head
                group_tail[new_line][num] = group_head[line][num];

                // Clear the cur head & tail
                group_head[line][num] = group_tail[line][num] = NULL;
            }
            // After reverse and concat usb to new line, clear cur group
            Group *tmp = group;
            group = group->prev;
            free(tmp);
        }
    }
    // Clear this toilet
    toilet_head[line] = toilet_tail[line] = NULL;

    // Maintain other toilets
    int next = toilet[line][NEXT_TOILET], prev = toilet[line][PREV_TOILET];
    toilet[next][PREV_TOILET] = prev;
    toilet[prev][NEXT_TOILET] = next;
}

void print(){
    int print_group = PRINT_GROUP;
    for(int line = 0; line < M; line++){
        // Start from toilet head
        Group *group = toilet_head[line];
        int first_print = 1;
        while(group != NULL){
            // Print usb
            int group_number = group->number;
            Usb *usb = group_head[line][group_number], *prev = NULL;
            while(usb != NULL){
                // For spacing
                if(first_print)
                    first_print = 0;
                else
                    printf(" ");
                printf("%d", usb->id);
                if(print_group)
                    printf(" (group %d)", usb->group);

                // Find where to go: the one that is not prev
                Usb *n_a = usb->neighbor_a, *n_b = usb->neighbor_b, *tmp = usb;
                if(n_b == prev)
                    usb = n_a;
                else if(n_a == prev)
                    usb = n_b;
                else
                    assert(THIS_SHOULD_NOT_HAPPEN);
                prev = tmp;
            }
            group = group->next;
        }
        printf("\n");
    }
}

void destroy(){
    // toilet
    for(int i=0; i<M; i++)
        free(toilet[i]);
    free(toilet);

    // group
    free(toilet_head);
    free(toilet_tail);

    // usb
    for(int i=0; i<M; i++){
        free(group_head[i]);
        free(group_tail[i]);
    }
    free(group_head);
    free(group_tail);

    return;
}

int main()
{
    int check = CHECK;

    scanf("%d%d%d", &M, &N, &K);
    initialization();
    char input[20];
    for(int i=0; i<N; i++){
        scanf("%s", input);
        if(strcmp(input, "enter") == 0){
            int group, id, line;
            scanf("%d%d%d", &group, &id, &line);
            enter(group, id, line);
        }
        else if(strcmp(input, "go") == 0){
            int line;
            scanf("%d", &line);
            go(line);
        }
        else if(strcmp(input, "leave") == 0){
            int line;
            scanf("%d", &line);
            leave(line);
        }
        else if(strcmp(input, "close") == 0){
            int line;
            scanf("%d", &line);
            close(line);
        }
        if(check){
            printf("-\nround %d: \n", i+1);
            print();
        }
    }
    if(check) printf("-\nresult:\n");
    print();
    // destroy();
}