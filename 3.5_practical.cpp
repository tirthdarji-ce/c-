#include <iostream>
using namespace std;

int superDigit(long n){

    if(n<10)
        return n;

    long sum=0;

    while(n>0){
        sum+=n%10;
        n/=10;
    }
    return superDigit(sum);
}

int main(){
    string n;
    int k;

    cout<<"Enter the value of n and k : ";
    cin>>n>>k;

    long sum=0;
    for (int i=0;i<n.length();i++){
        sum+=n[i]-'0';
    }
    sum*=k;
    cout<<"The value of super digit is : "<<superDigit(sum)<<endl;
    return 0;
}
