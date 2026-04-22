#include <iostream>
#include <set>
using namespace std;

int main() {
    set<int> s;
    int n, x;

    cout << "Enter number of elements: ";
    cin >> n;

    cout << "Enter elements: ";
    for (int i = 0; i < n; i++) {
        cin >> x;
        s.insert(x);   // duplicates automatically ignored
    }

    cout << "Unique sorted elements: ";
    for (int i : s) cout << i << " ";

    return 0;
}