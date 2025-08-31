#include <iostream>
#include <string>
#include <fstream>
using namespace std;

class Book {
public:
    string title, author;
    int isbn;
    string availability;
};

// Library user public variables
int userID;
string userName;

// Function to add a book to Library.txt
void addBook() {
    Book bookobj;
    bookobj.availability = "Available";

    ofstream LibraryBooks("Library.txt", ios::out | ios::app);
    if (!LibraryBooks) {
        cout << "Error opening file.\n";
        return;
    }

    cout << "Current Books Stored in Library:\n";
    ifstream readFile("Library.txt");
    string line;
    while (getline(readFile, line)) {
        cout << line << endl;
    }
    readFile.close();

    cout << "\n--- Creating Book ---\n";
    cout << "Title: ";
    getline(cin, bookobj.title);
    cout << "Author: ";
    getline(cin, bookobj.author);
    cout << "ISBN: ";
    cin >> bookobj.isbn;
	system("cls");
	
	cout << "[New book index added: \"" << bookobj.title << "\" by \"" << bookobj.author << "\", ISBN: " << bookobj.isbn << " | Now Available!" << "]\n\n";
    LibraryBooks << bookobj.title << ", " << bookobj.author << ", " << bookobj.isbn << ", " << bookobj.availability << "\n";
    LibraryBooks.close();
}
// Function to delete a book from Library.txt
void deleteBook() {
    cout << "Current Books Stored in Library:\n";
    ifstream readFile("Library.txt");
    if (!readFile) {
        cout << "Error opening file.\n";
        return;
    }

    string line;
    while (getline(readFile, line)) {
        cout << line << endl;
    }
    readFile.close();

    cout << "\n--- Deleting Book ---\n";
    cout << "Enter title or keyword to delete: ";
    string search;
    getline(cin, search);
    system("cls");

    ifstream inFile("Library.txt");
    ofstream outFile("temp.txt");
    bool found = false;

    while (getline(inFile, line)) {
        if (line.find(search) != string::npos) {
            cout << "[Deleted Book: " << line << "]\n\n";
            found = true;
        } else {
            outFile << line << endl;
        }
    }

    inFile.close();
    outFile.close();

    remove("Library.txt");
    rename("temp.txt", "Library.txt");

    if (!found) {
        cout << "No matching book found.\n\n";
    }
}
// Function to register user by Librarian
void addUser() {
	string line;
    cout << "--- Creating User ---\n";
    cout << "Username: ";
    getline(cin, userName);
    cout << "User ID: ";
    cin >> userID;
    system("cls");

    ofstream ListOfUsers("Users.txt", ios::out | ios::app);
    if (!ListOfUsers) {
        cout << "Error opening file.\n";
        return;
    }
    
	ListOfUsers << "Username: " << userName << " User ID: " << userID <<" | Borrowed Books:" << endl;
    ListOfUsers.close();
    cout << "Library User Added.\n\n";
}
// Function to delete a user by Librarian
void deleteUser() {
    cout << "List of all Users:\n";
    ifstream readUsers("Users.txt");
    if (!readUsers) {
        cout << "Error opening file.\n";
        return;
    }

    string line;
    while (getline(readUsers, line)) {
        cout << line << endl;
    }
    readUsers.close();

    cout << "\n--- Removing User ---\n";
    cout << "Enter username or user ID to remove: ";
    string search;
    getline(cin, search);
    system("cls");

    ifstream inFile("Users.txt");
    ofstream outFile("temp.txt");
    bool found = false;

    while (getline(inFile, line)) {
        if (line.find(search) != string::npos) {
            cout << "[Deleted User: " << line << "]\n\n";
            found = true;
        } else {
            outFile << line << endl;
        }
    }

    inFile.close();
    outFile.close();

    remove("Users.txt");
    rename("temp.txt", "Users.txt");

    if (!found) {
        cout << "No matching user found.\n\n";
    }
}
// Librarian access menu
void Librarian() {
    int choice;
    string userName;
    
	cout << "Enter Librarian User Name: ";
    cin >> userName;
	system("cls");
    
    do {
		cout << "--- Hello, " << userName << "! ---\n";
		cout << "--- You are logged in as a Librarian ---\n";
	    cout << "1. Add Book\n";
	    cout << "2. Delete Book\n";
	    cout << "3. Add a Library User\n";
	    cout << "4. Remove a Library User\n";
	    cout << "5. Log Out\n";
	    cout << "Enter Action: ";
	    cin >> choice;
	    system("cls");
	    
	    cin.ignore();
	
	    switch (choice) {
	    case 1:
	        addBook();
	        break;
	    case 2: 
	        deleteBook();
	        break;
	    case 3: 
	        addUser();
	        break;
	    case 4:
	        deleteUser();
	        break;
	    case 5:
	        return;
	    default:
	        cout << "Invalid Choice.\n";
		}
    } while (choice != 5);
}

// Function for user to borrow a book
void BorrowBook() {
    cout << "Current Books Stored in Library:\n";
    ifstream readFile("Library.txt");
    string line;
    while (getline(readFile, line)) {
        cout << line << endl;
    }
    readFile.close();

	string selectBook;
    cout << "\n--- Borrowing Book ---\n";
    cout << "Username: " << userName << endl;
    cout << "Enter FULL Book Title to borrow: ";
    getline(cin, selectBook);
    system("cls");
    
    ifstream inFile("Library.txt");
    ofstream outFile("temp.txt");
    bool found = false;

    while (getline(inFile, line)) {
        // Match full title (safer than partial)
        if (line.find(selectBook) != string::npos && line.find("Available") != string::npos) {
            size_t pos = line.find("Available");
            if (pos != string::npos) {
                line.replace(pos, 9, "Borrowed");
            }
            found = true;
            cout << "[Borrowed: " << selectBook << "]\n";
        }
        outFile << line << endl;
    }

    inFile.close();
    outFile.close();

    if (!found) {
        cout << "Book not found or already borrowed.\n\n";
        remove("temp.txt");
        return;
    }

    remove("Library.txt");
    rename("temp.txt", "Library.txt");

   
    ifstream userIn("Users.txt");
    ofstream userOut("tempUsers.txt");
    string userLine;
    bool userFound = false;

    while (getline(userIn, userLine)) {
        if (userLine.find("Username: " + userName) != string::npos) {
            userFound = true;

            // Only add book if not already borrowed by this user
            if (userLine.find(selectBook) == string::npos) {
                size_t pos = userLine.find("| Borrowed Books:");
                if (pos != string::npos) {
                    if (userLine.back() != ':' && userLine.back() != ' ')
                        userLine += ",";
                    userLine += " " + selectBook;
                } else {
                    userLine += " | Borrowed Books: " + selectBook;
                }
            }
        }
        userOut << userLine << endl;
    }

    userIn.close();
    userOut.close();

    remove("Users.txt");
    rename("tempUsers.txt", "Users.txt");

    if (userFound) {
        cout << "[Book borrowed successfully and added to your borrowed list.]\n\n";
    } else {
        cout << "[User not found. Book borrowing updated in library only.]\n\n";
    }
}
// Function for user to return a book
void ReturnBooks() {
    cout << "Books you borrowed:\n";
    string line, userLine;
    bool hasBorrowed = false;

    //Show borrowed books
    ifstream userIn("Users.txt");
    while (getline(userIn, userLine)) {
        if (userLine.find("Username: " + userName) != string::npos) {
            cout << userLine << endl;
            hasBorrowed = true;
        }
    }
    userIn.close();

    if (!hasBorrowed) {
        cout << "You have no borrowed books.\n\n";
        return;
    }

    //Ask user which book to return
    string bookSelect;
    cout << "\nWhich title would you like to return (Enter full title): ";
    getline(cin, bookSelect);
    system("cls");

    //Update Users.txt (remove book from borrowed list)
    ifstream userIn2("Users.txt");
    ofstream userOut("tempUsers.txt");
    bool returned = false;

    while (getline(userIn2, userLine)) {
        if (userLine.find("Username: " + userName) != string::npos) {
            size_t pos = userLine.find(bookSelect);
            if (pos != string::npos) {
                // Remove the book and any extra ", " before/after
                userLine.erase(pos, bookSelect.length());

                // Clean up leftover commas and spaces
                while (userLine.find(" ,") != string::npos)
                    userLine.replace(userLine.find(" ,"), 2, "");
                while (userLine.find(", ,") != string::npos)
                    userLine.replace(userLine.find(", ,"), 3, ",");
                if (userLine.back() == ',' || userLine.back() == ' ')
                    userLine.pop_back();

                returned = true;
                cout << "[Book returned successfully!]\n\n";
            }
        }
        userOut << userLine << endl;
    }

    userIn2.close();
    userOut.close();

    remove("Users.txt");
    rename("tempUsers.txt", "Users.txt");

    if (!returned) {
        cout << "Book not found in your borrowed list.\n\n";
        return;
    }

    //Update Library.txt (set book as Available)
    ifstream libIn("Library.txt");
    ofstream libOut("tempLibrary.txt");

    while (getline(libIn, line)) {
        if (line.find(bookSelect) != string::npos && line.find("Borrowed") != string::npos) {
            size_t pos = line.find("Borrowed");
            line.replace(pos, 8, "Available");
        }
        libOut << line << endl;
    }

    libIn.close();
    libOut.close();

    remove("Library.txt");
    rename("tempLibrary.txt", "Library.txt");
}
// Library User access menu
void UserAccess() {
    int choice;
    string userLine;
    
	cout << "Enter Library User Name: ";
    cin >> userName;
	system("cls");
	
	ifstream userIn("Users.txt");
	while (getline(userIn, userLine)) {
	    if (userLine.find("Username: " + userName) != string::npos) {
			userIn.close();
			system("cls");
			do {
				cout << "--- Welcome, " << userName << "! ---\n";
				cout << "--- You are logged in as a Library User. ---\n";
				cout << "1. Borrow a Book\n";
				cout << "2. Return Books\n";
				cout << "3. Log Out\n";
				cout << "Choose an Action: ";
				cin>>choice;
				system("cls");
					
				cin.ignore();
					
				switch (choice) {
				case 1:
					BorrowBook();
					break;
				case 2:
					ReturnBooks();
					break;
				case 3:
					return;
				default:
					cout << "Invalid Option.\n";
				}
			} while (choice != 3);
		} else {
			system("cls");
		    cout << "User does not exist.\n\n";
		}
	}
}

// Main function
int main() {
    int choice;
    do {
    	cout << "--- Choose Library Access ---\n";
    	cout << "1. Librarian Access\n";
    	cout << "2. User Access\n";
	    cout << "3. Exit\n";
	    cout << "Enter Access Type: ";
	    cin >> choice;
	    system("cls");
	
	    switch (choice) {
	    case 1:
	        Librarian();
	        break;
	    case 2:
	        UserAccess();
	        break;
	    case 3:
	        break;
	    default:
	        cout <<"Invalid Option.\n\n";
	    }
	} while (choice != 3);
    
    cout << "Thank you for using the program!";
    return 0;
}