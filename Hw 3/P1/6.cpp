#include <bits/stdc++.h>

using namespace std;

int F(int D[], int n)
{
    stack<int> S;
    priority_queue<int> H;

    S.push(D[0]);
    for(int i = 1; i < n; i++)
        if(!S.empty())
            if(D[i] >  S.top())
                S.push(D[i]);
            else{
                H.push(D[i]);
                H.push(S.top());
                S.pop();
            }
        else
            S.push(D[i]);

    stack<int> newS;

    while(!S.empty() && !H.empty()){
        if(H.top() > S.top()){
            newS.push(H.top());
            H.pop();
        }
        else{
            newS.push(S.top());
            S.pop();
        }
    }

    while(!H.empty()){
        newS.push(H.top());
        H.pop();
    }
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