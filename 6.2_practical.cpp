#include <iostream>
using namespace std;

class Person{

public:
    string name;
    int age;

    Person(){

    cout<<"Enter your name and age : ";
    cin>>name>>age;
    }
};

class Employee : public Person{

public:
    int id;

    Employee(){

    cout<<"Enter your employee ID : ";
    cin>>id;
    }
};

class Manager : public Employee{

public:
    string department;

    Manager(){

    cout<<"Enter your Department : ";
    cin>>department;
    }

    void getData(){

    cout<<"-------------------------------------------"<<endl;

    cout<<"The name of employee is : "<<name<<endl;
    cout<<"The age of employee is : "<<age<<endl;
    cout<<"The ID of employee is : "<<id<<endl;
    cout<<"The department of employee is : "<<department<<endl;
    }
};

int main(){

    Manager m1;
    m1.getData();
    return 0;
}
