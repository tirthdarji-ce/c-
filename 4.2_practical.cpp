#include <iostream>
using namespace std;

class Point
{
private:
    int x, y;

public:
    // Constructor
    Point(int xVal = 0, int yVal = 0)
    {
        x = xVal;
        y = yVal;
    }

    // Chainable move function using this pointer
    Point& move(int dx, int dy)
    {
        x += dx;
        y += dy;
        return *this;
    }

    void display() const
    {
        cout << "Point (" << x << ", " << y << ")" << endl;
    }
};

// Pass-by-reference using pointer
void shiftPoint(Point *p, int dx, int dy)
{
    p->move(dx, dy);
}

int main()
{
    int x, y;
    cout << "Enter initial x and y: ";
    cin >> x >> y;

    Point p(x, y);

    cout << "Initial Position: ";
    p.display();

    int dx1, dy1, dx2, dy2;

    cout << "Enter first move (dx dy): ";
    cin >> dx1 >> dy1;

    cout << "Enter second move (dx dy): ";
    cin >> dx2 >> dy2;

    // Chainable movement
    p.move(dx1, dy1).move(dx2, dy2);

    cout << "After Chain Move: ";
    p.display();

    int dx3, dy3;
    cout << "Enter shift using pointer (dx dy): ";
    cin >> dx3 >> dy3;

    // Pass by reference using pointer
    shiftPoint(&p, dx3, dy3);

    cout << "After Pointer Shift: ";
    p.display();

    return 0;
}