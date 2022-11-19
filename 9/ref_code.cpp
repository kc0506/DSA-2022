// CPP implementation of the approach
#include <bits/stdc++.h>
using namespace std;

// Function to return the count of same pairs
int pairCount(map<string, int> &d)
{
	int sum = 0;
	for (auto i : d)
		sum += (i.second * (i.second - 1)) / 2;

	return sum;
}

// Function to return total number of strings
// which satisfy required condition
int difference(vector<string> &array, int m)
{
	// Dictionary changed will store strings
	// with wild cards
	// Dictionary same will store strings
	// that are equal
	map<string, int> changed, same;

	// Iterating for all strings in the given array
	for (auto s : array)
	{
		// If we found the string then increment by 1
		// Else it will get default value 0
		same[s]++;

		// Iterating on a single string
		for (int i = 0; i < m; i++)
		{
			// Adding special symbol to the string
			string t = s.substr(0, i) + "//" + s.substr(i + 1);

			// Incrementing the string if found
			// Else it will get default value 0
			changed[t]++;
		}
	}

	// Return counted pairs - equal pairs
	return pairCount(changed) - pairCount(same) * (m - 1);
}

// Driver Code
int main()
{
	int k, l, flag;
	cin >> k >> l >> flag;
	vector<string> array(0);

	string buf;
	for(int i = 0; i < k; i++){
	    cin >> buf;
	    array.push_back(buf);
	}

    for(string s: array)
        cout << s << endl;

	cout << difference(array, l) << endl;

	return 0;
}

// This code is contributed by
// sanjeev2552
