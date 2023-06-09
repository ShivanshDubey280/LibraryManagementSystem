#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <iomanip>

using namespace std;

struct Book {
    string name;
    string author;
    bool available;
    string issuedTo;
    string issueDate;
    string returnDate;
};

struct Student {
    string name;
    vector<string> issuedBooks;
};

void addBook(vector<Book>& books) {
    Book newBook;
    cout << "Enter book name: ";
    cin.ignore();
    getline(cin, newBook.name);
    cout << "Enter author name: ";
    getline(cin, newBook.author);
    newBook.available = true;
    newBook.issuedTo = "";
    newBook.issueDate = "";
    newBook.returnDate = "";
    books.push_back(newBook);
    cout << "Book added successfully.\n";
}

void addStudent(vector<Student>& students) {
    Student newStudent;
    cout << "Enter student name: ";
    cin.ignore();
    getline(cin, newStudent.name);
    students.push_back(newStudent);
    cout << "Student added successfully.\n";
}

void displayBooks(const vector<Book>& books) {
    cout << "Books in the library:\n";
    for (const Book& book : books) {
        cout << "Book Name: " << book.name << endl;
        cout << "Author: " << book.author << endl;
        cout << "Availability: " << (book.available ? "Available" : "Issued") << endl;
        if (!book.available) {
            cout << "Issued To: " << book.issuedTo << endl;
            cout << "Issue Date: " << book.issueDate << endl;
            cout << "Return Date: " << book.returnDate << endl;
        }
        cout << endl;
    }
}

void displayStudents(const vector<Student>& students) {
    cout << "Students in the library:\n";
    for (const Student& student : students) {
        cout << "Student Name: " << student.name << endl;
        cout << "Number of Issued Books: " << student.issuedBooks.size() << endl;
        cout << "Issued Books:\n";
        for (const string& book : student.issuedBooks) {
            cout << "- " << book << endl;
        }
        cout << endl;
    }
}

void issueBook(vector<Book>& books, vector<Student>& students) {
    string bookName;
    string studentName;
    bool bookFound = false;
    bool studentFound = false;
    cout << "Enter book name to be issued: ";
    cin.ignore();
    getline(cin, bookName);
    cout << "Enter student name: ";
    getline(cin, studentName);
    for (Book& book : books) {
        if (book.name == bookName) {
            if (book.available) {
                book.available = false;
                book.issuedTo = studentName;

                time_t now = time(0);
                tm* currentDate = localtime(&now);
                tm* returnDate = new tm(*currentDate);
                returnDate->tm_mday += 30;
                time_t returnTime = mktime(returnDate);
                char* returnDateString = asctime(returnDate);
                book.issueDate = asctime(currentDate);
                book.returnDate = returnDateString;

                bookFound = true;
            } else {
                cout << "Book is already issued.\n";
            }
            break;
        }
    }
    for (Student& student : students) {
        if (student.name == studentName) {
            student.issuedBooks.push_back(bookName);
            studentFound = true;
            break;
        }
    }
    if (bookFound && studentFound) {
        cout << "Book issued successfully.\n";
    } else {
        cout << "Book or student not found.\n";
    }
}

void returnBook(vector<Book>& books, vector<Student>& students) {
    string bookName;
    string studentName;
    bool bookFound = false;
    bool studentFound = false;
    cout << "Enter book name to be returned: ";
    cin.ignore();
    getline(cin, bookName);
    cout << "Enter student name: ";
    getline(cin, studentName);
    for (Book& book : books) {
        if (book.name == bookName && !book.available && book.issuedTo == studentName) {
            book.available = true;
            book.issuedTo = "";
            book.issueDate = "";
            book.returnDate = "";
            bookFound = true;
            break;
        }
    }
    for (Student& student : students) {
        if (student.name == studentName) {
            for (auto it = student.issuedBooks.begin(); it != student.issuedBooks.end(); ++it) {
                if (*it == bookName) {
                    student.issuedBooks.erase(it);
                    studentFound = true;
                    break;
                }
            }
            break;
        }
    }

    if (bookFound && studentFound) {
        cout << "Book returned successfully.\n";
    } else {
        cout << "Book or student not found.\n";
    }
}

void saveData(const vector<Book>& books, const vector<Student>& students) {
    ofstream bookFile("books.txt");
    ofstream studentFile("students.txt");
    for (const Book& book : books) {
        bookFile << book.name << "," << book.author << "," << book.available << ","
                 << book.issuedTo << "," << book.issueDate << "," << book.returnDate << endl;
    }
    for (const Student& student : students) {
        studentFile << student.name;
        for (const string& book : student.issuedBooks) {
            studentFile << "," << book;
        }
        studentFile << endl;
    }
    bookFile.close();
    studentFile.close();
    cout << "Data saved successfully.\n";
}

void loadData(vector<Book>& books, vector<Student>& students) {
    ifstream bookFile("books.txt");
    ifstream studentFile("students.txt");

    // Check if the data files exist
    if (!bookFile.good() || !studentFile.good()) {
        cout << "No existing data found. Starting with empty records.\n";
        return;
    }

    string line;
    while (getline(bookFile, line)) {
        Book book;
        size_t pos = 0;
        string token;
        int count = 0;
        while ((pos = line.find(",")) != string::npos) {
            token = line.substr(0, pos);
            switch (count) {
                case 0:
                    book.name = token;
                    break;
                case 1:
                    book.author = token;
                    break;
                case 2:
                    book.available = (token == "1");
                    break;
                case 3:
                    book.issuedTo = token;
                    break;
                case 4:
                    book.issueDate = token;
                    break;
                case 5:
                    book.returnDate = token;
                    break;
            }
            line.erase(0, pos + 1);
            count++;
        }
        // Handle the last token (return date) separately
        if (count == 5) {
            book.returnDate = line;
        }
        books.push_back(book);
    }

    while (getline(studentFile, line)) {
        Student student;
        size_t pos = 0;
        string token;
        int count = 0;
        while ((pos = line.find(",")) != string::npos) {
            token = line.substr(0, pos);
            if (count == 0) {
                student.name = token;
            } else {
                student.issuedBooks.push_back(token);
            }
            line.erase(0, pos + 1);
            count++;
        }
        // Handle the last token (issued book) separately
        if (!line.empty()) {
            student.issuedBooks.push_back(line);
        }
        students.push_back(student);
    }

    bookFile.close();
    studentFile.close();
    cout << "Data loaded successfully.\n";
}

int main() {
    vector<Book> books;
    vector<Student> students;

    // Load existing data
    loadData(books, students);

    int choice;
    while (true) {
        cout << "Library Management System\n";
        cout << "1. Add Book\n";
        cout << "2. Add Student\n";
        cout << "3. Display Books\n";
        cout << "4. Display Students\n";
        cout << "5. Issue Book\n";
        cout << "6. Return Book\n";
        cout << "7. Save Data\n";
        cout << "8. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                addBook(books);
                break;
            case 2:
                addStudent(students);
                break;
            case 3:
                displayBooks(books);
                break;
            case 4:
                displayStudents(students);
                break;
            case 5:
                issueBook(books, students);
                break;
            case 6:
                returnBook(books, students);
                break;
            case 7:
                saveData(books, students);
                break;
            case 8:
                cout << "Exiting...\n";
                return 0;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}
