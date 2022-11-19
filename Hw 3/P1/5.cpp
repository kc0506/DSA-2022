#include <bits/stdc++.h>

using namespace std;

int F(int D[], int n)
{
    stack<int> S;
    S.push(D[0]);
    for(int i = 1; i < n; i++)
        if(!S.empty())
            if(D[i] >  S.top())
                S.push(D[i]);
            else
                S.pop();
        else
            S.push(D[i]);

    stack<int> newS;
    while(!S.empty()){
        newS.push(S.top());
        S.pop();
    }

    int index = 0;
    while(!newS.empty()){
        D[index++] = newS.top();
        newS.pop();
    }
    return index;
}

int main()
{
    int test[] = {8, 9, 3, 2, 1, 4, 5};
    int n = F(test, 7);
    for(int i = 0; i < n; i++)
        cout << test[i] << " \n"[i < n - 1];
}