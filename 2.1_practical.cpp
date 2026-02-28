#include <iostream>
using namespace std;

const int MAX = 100;

struct Rectangle {
    double length;
    double width;
};

Rectangle rects[MAX];
int countRect = 0;

// Add rectangle
void addRectangle() {
    cout << "Enter Length: ";
    cin >> rects[countRect].length;
    cout << "Enter Width: ";
    cin >> rects[countRect].width;

    countRect++;
    cout << "Rectangle Added Successfully!\n";
}

// Update rectangle
void updateRectangle() {
    int index;
    cout << "Enter Rectangle Number (1 to " << countRect << "): ";
    cin >> index;

    if(index >= 1 && index <= countRect) {
        cout << "Enter New Length: ";
        cin >> rects[index-1].length;
        cout << "Enter New Width: ";
        cin >> rects[index-1].width;
        cout << "Updated Successfully!\n";
    } else {
        cout << "Invalid Rectangle Number!\n";
    }
}

// Display rectangles with calculations
void displayRectangles() {
    for(int i = 0; i < countRect; i++) {
        double area = rects[i].length * rects[i].width;
        double perimeter = 2 * (rects[i].length + rects[i].width);

        cout << "\nRectangle " << i+1 << endl;
        cout << "Length: " << rects[i].length << endl;
        cout << "Width: " << rects[i].width << endl;
        cout << "Area: " << area << endl;
        cout << "Perimeter: " << perimeter << endl;
    }
}

int main() {
    int choice;

    do {
        cout << "\n1. Add Rectangle\n2. Update Rectangle\n3. Display All\n4. Exit\n";
        cout << "Enter Choice: ";
        cin >> choice;

        switch(choice) {
            case 1: addRectangle(); break;
            case 2: updateRectangle(); break;
            case 3: displayRectangles(); break;
            case 4: cout << "Exiting...\n"; break;
            default: cout << "Invalid Choice!\n";
        }

    } while(choice != 4);

    return 0;
}