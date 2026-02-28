#include <iostream>
#include <vector>
using namespace std;

class Item {
private:
    int itemID;
    string itemName;
    double price;
    int quantity;

public:
    // Default Constructor
    Item() {
        itemID = 0;
        itemName = "Unknown";
        price = 0.0;
        quantity = 0;
    }

    // Parameterized Constructor
    Item(int id, string name, double p, int qty) {
        itemID = id;
        itemName = name;
        price = p;
        quantity = qty;
    }

    int getID() const {
        return itemID;
    }

    void addStock(int qty) {
        if(qty > 0) {
            quantity += qty;
            cout << "Stock Updated Successfully!\n";
        } else {
            cout << "Invalid Quantity!\n";
        }
    }

    void sellItem(int qty) {
        if(qty <= 0) {
            cout << "Invalid Quantity!\n";
        }
        else if(qty > quantity) {
            cout << "Sale Failed! Not enough stock.\n";
        }
        else {
            quantity -= qty;
            cout << "Sale Successful!\n";
        }
    }

    void display() const {
        cout << "\n----- Item Details -----\n";
        cout << "Item ID   : " << itemID << endl;
        cout << "Item Name : " << itemName << endl;
        cout << "Price     : ₹" << price << endl;
        cout << "Stock     : " << quantity << endl;
    }
};

int main() {
    vector<Item> inventory;
    int choice;

    do {
        cout << "\n1. Add Item (Default)";
        cout << "\n2. Add Item (Custom)";
        cout << "\n3. Increase Stock";
        cout << "\n4. Sell Item";
        cout << "\n5. Display Item";
        cout << "\n6. Exit";
        cout << "\nEnter Choice: ";
        cin >> choice;

        if(choice == 1) {
            Item temp;
            inventory.push_back(temp);
            cout << "Default Item Added!\n";
        }

        else if(choice == 2) {
            int id, qty;
            string name;
            double price;

            cout << "Enter Item ID: ";
            cin >> id;
            cin.ignore();

            cout << "Enter Item Name: ";
            getline(cin, name);

            cout << "Enter Price: ";
            cin >> price;

            cout << "Enter Quantity: ";
            cin >> qty;

            inventory.push_back(Item(id, name, price, qty));
            cout << "Item Added Successfully!\n";
        }

        else if(choice >= 3 && choice <= 5) {
            int id;
            cout << "Enter Item ID: ";
            cin >> id;

            bool found = false;

            for(auto &item : inventory) {
                if(item.getID() == id) {
                    found = true;

                    if(choice == 3) {
                        int qty;
                        cout << "Enter Quantity to Add: ";
                        cin >> qty;
                        item.addStock(qty);
                    }

                    else if(choice == 4) {
                        int qty;
                        cout << "Enter Quantity to Sell: ";
                        cin >> qty;
                        item.sellItem(qty);
                    }

                    else if(choice == 5) {
                        item.display();
                    }

                    break;
                }
            }

            if(!found)
                cout << "Item Not Found!\n";
        }

    } while(choice != 6);

    return 0;
}