#include <bits/stdc++.h>

using namespace std;

#define d 26

bool Check(vector<vector<char>> Arr, vector<char> P){
    int n = Arr.size(), m = Arr[0].size(), h = P.size();

    int Base = 1;
    for(int i = 0; i < h - 1; i++)
        Base *= d;

    int HashP = 0;
    for(int i = 0; i < h; i++)
        HashP = HashP * d + P[i];

    // cout << "HashP = " << HashP << endl;

    for(int i = 0; i < n; i++){
        int HashA = 0;
        for(int j = 0; j < h; j++)
            HashA = HashA * d + Arr[i][j];
        for(int j = 0; j < m; j++){
            // cout << "HashA = " << HashA << endl;

            if(HashA == HashP)
                return true;
            HashA -= Arr[i][j] * Base;
            HashA = HashA * d + Arr[i][(j + h) % m];
        }
    }
    return false;
}

int main()
{
    int n = 3, m = 5;
    vector<vector<char>> Arr(n, vector<char>(m, 0));
    vector<char> P = {'i', 's', 'q'};

    for(int i = 0; i < n; i++)
        for(int j = 0; j < m; j++)
            cin >> Arr[i][j];

    cout << "check = " << Check(Arr, P);
}