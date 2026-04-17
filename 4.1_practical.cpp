#include <iostream>
using namespace std;

int main() {
    int *arr = nullptr;
    int size = 0;

    int value;
    for (int k=0;k<3;k++) {
        cout<<"Enter value: ";
        cin>>value;

        int *newArr=new int[size+1];

        for (int i=0;i<size;i++){
            newArr[i]=arr[i];
        }
        newArr[size]=value;

        delete[] arr;
        arr=newArr;
        size++;
    }
    cout<<"Array after insertion: "<<endl;
    for (int i = 0; i < size; i++) {
        cout<<arr[i]<<" "<<endl;
    }
    int pos;
    cout<<"Enter position to delete"<<endl;
    cin>>pos;

    if (pos>=0 && pos<size) {
        int *newArr = new int[size-1];

        for (int i = 0, j = 0; i < size; i++){
            if (i != pos) {
                newArr[j++] = arr[i];
            }
        }
        delete[] arr;
        arr = newArr;
        size--;

        cout <<"Element deleted"<<endl;
    } else {
        cout<<"Invalid position"<<endl;
    }
    cout<<"Array after deletion: "<<endl;
    for (int i = 0; i < size; i++) {
        cout<<arr[i] << " "<<endl;
    }
    delete[] arr;

    return 0;
}
