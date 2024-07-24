#include <iostream>
#include <mysql/mysql.h>
#include <sstream>

using namespace std;

const char* hostname = "127.0.0.1";
const char* user = "root";
const char* pass = "123";
const char* dbname = "cpp_crud_library";
unsigned int port = 31235;
const char* unixsocket = NULL;
unsigned long clientflag = 0;

MYSQL* connect_db() {
    MYSQL* conn = mysql_init(0);
    if (conn) {
        conn = mysql_real_connect(conn, hostname, user, pass, dbname, port, unixsocket, clientflag);
        if (conn) {
            cout << "Connected to the database successfully." << endl;
        } else {
            cerr << "Connection failed: " << mysql_error(conn) << endl;
        }
    } else {
        cerr << "mysql_init failed" << endl;
    }
    return conn;
}

void add_book(const string& title, const string& author, int year, int copies) {
    MYSQL* conn = connect_db();
    if (conn) {
        stringstream query;
        query << "INSERT INTO books (title, author, year, copies) VALUES ('" << title << "', '" << author << "', " << year << ", " << copies << ")";
        if (mysql_query(conn, query.str().c_str())) {
            cerr << "INSERT failed: " << mysql_error(conn) << endl;
        } else {
            cout << "Book successfully added." << endl;
        }
        mysql_close(conn);
    }
}

void show_books() {
    MYSQL* conn = connect_db();
    if (conn) {
        if (mysql_query(conn, "SELECT * FROM books")) {
            cerr << "SELECT failed: " << mysql_error(conn) << endl;
            mysql_close(conn);
            return;
        }

        MYSQL_RES* res = mysql_store_result(conn);
        if (res == nullptr) {
            cerr << "mysql_store_result failed: " << mysql_error(conn) << endl;
            mysql_close(conn);
            return;
        }

        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res))) {
            cout << "ID: " << row[0] << ", Title: " << row[1] << ", Author: " << row[2] << ", Year: " << row[3] << ", Copies: " << row[4] << endl;
        }

        mysql_free_result(res);
        mysql_close(conn);
    }
}

void update_book(int book_id, const string& title, const string& author, int year, int copies) {
    MYSQL* conn = connect_db();
    if (conn) {
        stringstream query;
        query << "UPDATE books SET title = '" << title << "', author = '" << author << "', year = " << year << ", copies = " << copies << " WHERE id = " << book_id;
        if (mysql_query(conn, query.str().c_str())) {
            cerr << "UPDATE failed: " << mysql_error(conn) << endl;
        } else {
            cout << "Book successfully updated." << endl;
        }
        mysql_close(conn);
    }
}

void delete_book(int book_id) {
    MYSQL* conn = connect_db();
    if (conn) {
        stringstream query;
        query << "DELETE FROM books WHERE id = " << book_id;
        if (mysql_query(conn, query.str().c_str())) {
            cerr << "DELETE failed: " << mysql_error(conn) << endl;
        } else {
            cout << "Book successfully deleted." << endl;
        }
        mysql_close(conn);
    }
}

void borrow_book(int book_id) {
    MYSQL* conn = connect_db();
    if (conn) {
        stringstream query;
        query << "UPDATE books SET copies = copies - 1 WHERE id = " << book_id << " AND copies > 0";
        if (mysql_query(conn, query.str().c_str())) {
            cerr << "Borrow failed: " << mysql_error(conn) << endl;
        } else {
            if (mysql_affected_rows(conn) > 0) {
                cout << "Book borrowed successfully." << endl;
            } else {
                cout << "Book not available for borrowing." << endl;
            }
        }
        mysql_close(conn);
    }
}

void return_book(int book_id) {
    MYSQL* conn = connect_db();
    if (conn) {
        stringstream query;
        query << "UPDATE books SET copies = copies + 1 WHERE id = " << book_id;
        if (mysql_query(conn, query.str().c_str())) {
            cerr << "Return failed: " << mysql_error(conn) << endl;
        } else {
            cout << "Book returned successfully." << endl;
        }
        mysql_close(conn);
    }
}

int main() {
    int role_choice;
    cout << "Select Role:\n";
    cout << "1. Admin\n";
    cout << "2. User\n";
    cout << "Enter choice: ";
    cin >> role_choice;

    int choice;
    while (true) {
        if (role_choice == 1) {
            cout << "\nAdmin Menu:\n";
            cout << "1. Add Book\n";
            cout << "2. Show All Books\n";
            cout << "3. Update Book\n";
            cout << "4. Delete Book\n";
            cout << "5. Exit\n";
            cout << "Enter choice: ";
            cin >> choice;

            if (choice == 1) {
                string title, author;
                int year, copies;
                cout << "Enter book title: ";
                cin.ignore();
                getline(cin, title);
                cout << "Enter author: ";
                getline(cin, author);
                cout << "Enter year of publication: ";
                cin >> year;
                cout << "Enter number of copies: ";
                cin >> copies;
                add_book(title, author, year, copies);
            } else if (choice == 2) {
                show_books();
            } else if (choice == 3) {
                int book_id;
                string title, author;
                int year, copies;
                cout << "Enter book ID to update: ";
                cin >> book_id;
                cin.ignore();
                cout << "Enter new title: ";
                getline(cin, title);
                cout << "Enter new author: ";
                getline(cin, author);
                cout << "Enter new year of publication: ";
                cin >> year;
                cout << "Enter new number of copies: ";
                cin >> copies;
                update_book(book_id, title, author, year, copies);
            } else if (choice == 4) {
                int book_id;
                cout << "Enter book ID to delete: ";
                cin >> book_id;
                delete_book(book_id);
            } else if (choice == 5) {
                break;
            } else {
                cout << "Invalid choice. Please try again." << endl;
            }
        } else if (role_choice == 2) {
            cout << "\nUser Menu:\n";
            cout << "1. Show All Books\n";
            cout << "2. Borrow Book\n";
            cout << "3. Return Book\n";
            cout << "4. Exit\n";
            cout << "Enter choice: ";
            cin >> choice;

            if (choice == 1) {
                show_books();
            } else if (choice == 2) {
                int book_id;
                cout << "Enter book ID to borrow: ";
                cin >> book_id;
                borrow_book(book_id);
            } else if (choice == 3) {
                int book_id;
                cout << "Enter book ID to return: ";
                cin >> book_id;
                return_book(book_id);
            } else if (choice == 4) {
                break;
            } else {
                cout << "Invalid choice. Please try again." << endl;
            }
        } else {
            cout << "Invalid role choice. Exiting..." << endl;
            break;
        }
    }
    return 0;
}