#include <stdio.h>

int N, M, board[25][25] = {0};

int rows[25], cols[25], clues_r[25][25], clues_c[25][25];

void print()
{
    for(int i=0; i<N; i++)
        for(int j=0; j<=M; j++)
            if(j == M)
                printf("\n");
            else
                printf("%c", "_0o"[board[i][j] + 1]);
    return;
}

int check(int x, int y, int color)
{
    int ans = 1;
    board[x][y] = color;

    // first check row

    int seg_index = -1, cur_len = 0;

    for(int j=0; j<M; j++)
    {
        if(board[x][j] == 1)
        {
            if(cur_len == 0)
                seg_index++;
            cur_len++;
        }
        else
        {
            if(cur_len != 0)
            {
                if(seg_index >= rows[x])
                {
                    ans = 0;
                }
                else if(board[x][j] == 0)
                {
                    if(cur_len > clues_r[x][seg_index])
                        ans = 0;
                }
                else if(board[x][j] == -1)
                {
                    if(cur_len != clues_r[x][seg_index])
                        ans = 0;
                }
                cur_len = 0;
            }
        }
    }
    if(cur_len != 0)
    {
        if(seg_index >= rows[x])
            ans = 0;
        else if(cur_len != clues_r[x][seg_index])
            ans = 0;
    }
    if(seg_index < rows[x] - 1 && y == M - 1)
    {
        ans = 0;
        // print();
    }

    //rintf("ans before check col = %d\n", ans);
    // then check col

    seg_index = -1, cur_len = 0;
    for(int i=0; i<N; i++)
    {
        if(board[i][y] == 1)
        {
            if(cur_len == 0)
                seg_index++;
            cur_len++;
        }
        else
        {
            if(cur_len != 0)
            {
                if(seg_index >= cols[y])
                {
                    ans = 0;
                }
                else if(board[i][y] == 0)
                {
                    if(cur_len > clues_c[y][seg_index])
                        ans = 0;
                }
                else if(board[i][y] == -1)
                {
                    if(cur_len != clues_c[y][seg_index])
                        ans = 0;
                }
                cur_len = 0;
            }
        }
    }
    if(cur_len != 0)
    {
        if(seg_index >= cols[y])
            ans = 0;
        else if(cur_len != clues_c[y][seg_index])
            ans = 0;
        cur_len = 0;
    }
    if(seg_index < cols[y] - 1 && x == N - 1 && ans)
    {
        ans = 0;
    }

    board[x][y] = 0;
    return ans;
}

int filled()
{
    for(int i=0; i<N; i++)
        for(int j=0; j<M; j++)
            if(!board[i][j])
                return 0;
    return 1;
}

int recur(int pos, int color) // pos = x * M + y; 1for black -1 for white
{
    int x = pos / M, y = pos % M;

    if(filled())
        return 1;

    if(!check(x, y, color))
        return 0;

    board[x][y] = color;

    if(recur(pos + 1, 1))
        return 1;
    if(recur(pos + 1, -1))
        return 1;
    board[x][y] = 0;
    return 0;
}

void solve()
{
    if(recur(0, 1) || recur(0, -1))
    {
        print();
        return;
    }
    printf("no solution\n");
    return;
}

int main()
{
    scanf("%d %d", &N, &M);
    for(int i=0; i<N; i++)
    {
        int tmp;
        scanf("%d", &tmp);
        rows[i] = tmp;
        for(int j=0; j<tmp; j++)
            scanf("%d", &(clues_r[i][j]));
    }

    for(int i=0; i<M; i++)
    {
        int tmp;
        scanf("%d", &tmp);
        cols[i] = tmp;
        for(int j=0; j<tmp; j++)
            scanf("%d", &(clues_c[i][j]));
    }

    solve();
}