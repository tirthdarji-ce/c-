#include <iostream>
#include <vector>
using namespace std;

class Book {
private:
    int id;
    string title;
    string author;
    int copies;

public:
    Book(int i, string t, string a, int c) {
        id = i;
        title = t;
        author = a;
        copies = c;
    }

    int getId() {
        return id;
    }

    void issueBook() {
        if(copies > 0) {
            copies--;
            cout << "Book Issued Successfully!\n";
        } else {
            cout << "No Copies Available!\n";
        }
    }

    void returnBook() {
        copies++;
        cout << "Book Returned Successfully!\n";
    }

    void display() {
        cout << "\nID: " << id
             << "\nTitle: " << title
             << "\nAuthor: " << author
             << "\nAvailable Copies: " << copies << endl;
    }
};

int main() {
    vector<Book> library;
    int choice;

    do {
        cout << "\n1. Add Book\n2. Issue Book\n3. Return Book\n4. Display All Books\n5. Exit\n";
        cout << "Enter Choice: ";
        cin >> choice;

        if(choice == 1) {
            int id, copies;
            string title, author;
            cin.ignore();

            cout << "Enter Book ID: ";
            cin >> id;
            cin.ignore();

            cout << "Enter Title: ";
            getline(cin, title);

            cout << "Enter Author: ";
            getline(cin, author);

            cout << "Enter Copies: ";
            cin >> copies;

            library.push_back(Book(id, title, author, copies));
            cout << "Book Added Successfully!\n";
        }

        else if(choice == 2) {
            int id;
            cout << "Enter Book ID to Issue: ";
            cin >> id;

            for(auto &book : library) {
                if(book.getId() == id) {
                    book.issueBook();
                    break;
                }
            }
        }

        else if(choice == 3) {
            int id;
            cout << "Enter Book ID to Return: ";
            cin >> id;

            for(auto &book : library) {
                if(book.getId() == id) {
                    book.returnBook();
                    break;
                }
            }
        }

        else if(choice == 4) {
            for(auto &book : library) {
                book.display();
            }
        }

    } while(choice != 5);

    return 0;
}