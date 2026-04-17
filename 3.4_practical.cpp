#include <iostream>
using namespace std;

template <typename T>
void display(T arr[], int n) {
    for (int i = 0; i < n; i++)
        cout << arr[i] << " ";
    cout << endl;
}
template <typename T>
T findMax(T arr[], int n) {
    T maxVal = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i] > maxVal)
            maxVal = arr[i];
    }
    return maxVal;
}
template <typename T>
void reverseArray(T arr[], int n) {
    for (int i = 0; i < n / 2; i++) {
        T temp = arr[i];
        arr[i] = arr[n - 1 - i];
        arr[n - 1 - i] = temp;
    }
}
template <typename T>
void findLeaders(T arr[], int n) {
    T maxFromRight = arr[n - 1];
    cout << maxFromRight << " ";
    for (int i = n - 2; i >= 0; i--) {
        if (arr[i] > maxFromRight) {
            maxFromRight = arr[i];
            cout << maxFromRight << " ";
        }
    }
    cout << endl;
}
int main() {
    int n;

    cout << "Enter number of integers: ";
    cin >> n;
    int intArr[100];
    cout << "Enter integers:\n";
    for (int i = 0; i < n; i++)
    cin >> intArr[i];

    cout << "Integer Array: ";
    display(intArr, n);
    cout << "Maximum: " << findMax(intArr, n) << endl;
    reverseArray(intArr, n);
    cout << "Reversed: ";
    display(intArr, n);
    cout << "Leader Elements: ";
    findLeaders(intArr, n);

    cout << "\nEnter number of floats: ";
    cin >> n;
    float floatArr[100];
    cout << "Enter floats:\n";
    for (int i = 0; i < n; i++)
    cin >> floatArr[i];

    cout << "Float Array: ";
    display(floatArr, n);
    cout << "Maximum: " << findMax(floatArr, n) << endl;
    reverseArray(floatArr, n);
    cout << "Reversed: ";
    display(floatArr, n);
    cout << "Leader Elements: ";
    findLeaders(floatArr, n);

    cout << "\nEnter number of characters: ";
    cin >> n;
    char charArr[100];
    cout << "Enter characters:\n";
    for (int i = 0; i < n; i++)
    cin >> charArr[i];

    cout << "Char Array: ";
    display(charArr, n);
    cout << "Maximum: " << findMax(charArr, n) << endl;
    reverseArray(charArr, n);
    cout << "Reversed: ";
    display(charArr, n);
    cout << "Leader Elements: ";
    findLeaders(charArr, n);

    return 0;
}


