#include <iostream>
using namespace std;
void inputArray(int arr[], int n) {
    cout << "Enter elements:\n";
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }
}
int recursiveSum(int arr[], int n) {
    if (n == 0)
        return 0;
    return arr[n - 1] + recursiveSum(arr, n - 1);
}
int iterativeSum(int arr[], int n) {
    int sum = 0;
    for (int i = 0; i < n; i++)
        sum += arr[i];
    return sum;
}
void displaySums(int recSum, int itrSum) {
    cout << "\nRecursive Sum: " << recSum << endl;
    cout << "Iterative Sum: " << itrSum << endl;
}

int main() {
    int n;
    cout << "Enter size of array: ";
    cin >> n;

    int arr[1000];

    inputArray(arr, n);

    int recSum = recursiveSum(arr, n);
    int itrSum = iterativeSum(arr, n);

    displaySums(recSum, itrSum);
    return 0;
}
