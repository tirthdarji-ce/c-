#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    vector<int> v;
    int n, x;

    cout << "Enter number of elements: ";
    cin >> n;

    cout << "Enter elements: ";
    for (int i = 0; i < n; i++) {
        cin >> x;
        v.push_back(x);
    }

    // Method 1: std::reverse
    vector<int> v1 = v;
    reverse(v1.begin(), v1.end());
    cout << "std::reverse: ";
    for (int i : v1) cout << i << " ";

    // Method 2: manual iterator swap
    vector<int> v2 = v;
    for (auto i = v2.begin(), j = v2.end() - 1; i < j; i++, j--)
        swap(*i, *j);

    cout << "\nmanual reverse: ";
    for (int i : v2) cout << i << " ";

    return 0;
}