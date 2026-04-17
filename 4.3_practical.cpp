#include<iostream>
using namespace std;

int main()
{
    int n,m;
    cout<<"Enter size of first array: ";
    cin>>n;
    cout<<"Enter size of second array: ";
    cin>>m;

    int* a=new int[n];
    int* b=new int[m];

    cout<<"Enter elements of first sorted array: "<<endl;
    for(int i=0;i<n;i++){
        cin>>a[i];
    }
    cout<<"Enter elements of second sorted array: "<<endl;
    for(int i=0;i<m;i++){
        cin>>b[i];
    }
    int* c=new int[n+m];

    int i=0,j=0,k=0;

    while(i<n&&j<m){
        if(a[i]<b[j]){
            c[k]=a[i];
            i++;
        }
        else{
            c[k]=b[j];
            j++;
        }
        k++;
    }

    while(i<n){
        c[k]=a[i];
        i++;
        k++;
    }

    while(j<m){
        c[k]=b[j];
        j++;
        k++;
    }

    cout<<"Merged sorted array: "<<endl;
    for(int i=0;i<n+m;i++)
        cout<<c[i]<<" "<<endl;

    delete[] a;
    delete[] b;
    delete[] c;
    return 0;
}
