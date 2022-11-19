#include <stdio.h>

#define MAX 100

int MergeSort(int Arr[], int p, int q){
    if(p >= q)
        return 0;

    int Count = 0, Mid = p + (q - p) / 2 + 1;


    // printf("Mid = %d\n", Mid);

    Count += MergeSort(Arr, p, Mid - 1);
    Count += MergeSort(Arr, Mid, q);

    // printf("(%d, %d)\n", p, q);
    // return 0;

    int LeftArr[MAX], RightArr[MAX];
    for(int i = p; i < Mid; i++)
        LeftArr[i - p] = Arr[i];
    for(int i = Mid; i <= q; i++)
        RightArr[i - Mid] = Arr[i];

    // return 0;

    int Index = p, LeftCur = 0, LeftRemain = (Mid - 1) - p + 1, RightCur = 0, RightRemain = q - Mid + 1;
    while(LeftRemain > 0 && RightRemain > 0){
        if(LeftArr[LeftCur] > RightArr[RightCur])
            Arr[Index++] = LeftArr[LeftCur++], LeftRemain--;
        else{
            Arr[Index++] = RightArr[RightCur++], Count += LeftRemain, RightRemain--;
        }
    }


    for(int i = 0; i < LeftRemain; i++)
        Arr[Index++] = LeftArr[LeftCur++];
    for(int i = 0; i < RightRemain; i++)
        Arr[Index++] = RightArr[RightCur++];
    return Count;
}


int main()
{
    int Arr[MAX] = {0, 7, 1, 3, 9, 5, 2};
    printf("Count = %d\n", MergeSort(Arr, 1, 6));
}
