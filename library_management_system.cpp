#include <algorithm>
#include <cctype>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

const string DATA_FILE = "library_books.txt";

string trim(const string& text) {
    const size_t first = text.find_first_not_of(" \t\r\n");
    if (first == string::npos) {
        return "";
    }

    const size_t last = text.find_last_not_of(" \t\r\n");
    return text.substr(first, last - first + 1);
}

string lowerCase(string text) {
    transform(text.begin(), text.end(), text.begin(), [](unsigned char character) {
        return static_cast<char>(tolower(character));
    });
    return text;
}

string readText(const string& prompt, bool allowBlank = false) {
    string value;

    while (true) {
        cout << prompt;
        getline(cin, value);
        value = trim(value);

        if ((allowBlank || !value.empty()) && value.find('|') == string::npos) {
            return value;
        }

        cout << "Please enter a valid value. The '|' character is not allowed.\n";
    }
}

int readNumber(const string& prompt, int minimum, int maximum) {
    string input;
    int value;

    while (true) {
        cout << prompt;
        getline(cin, input);
        stringstream parser(input);
        char extra;

        if ((parser >> value) && !(parser >> extra) &&
            value >= minimum && value <= maximum) {
            return value;
        }

        cout << "Enter a whole number from " << minimum << " to " << maximum << ".\n";
    }
}

class Book {
private:
    int id;
    string title;
    string author;
    int year;
    bool issued;

public:
    Book(int bookId = 0, const string& bookTitle = "", const string& bookAuthor = "",
         int publicationYear = 0, bool isIssued = false)
        : id(bookId), title(bookTitle), author(bookAuthor),
          year(publicationYear), issued(isIssued) {}

    int getId() const { return id; }
    const string& getTitle() const { return title; }
    const string& getAuthor() const { return author; }
    int getYear() const { return year; }
    bool isIssued() const { return issued; }

    void updateDetails(const string& newTitle, const string& newAuthor, int newYear) {
        title = newTitle;
        author = newAuthor;
        year = newYear;
    }

    void issue() { issued = true; }
    void returnBook() { issued = false; }

    string toFileLine() const {
        return to_string(id) + "|" + title + "|" + author + "|" +
               to_string(year) + "|" + (issued ? "1" : "0");
    }

    static bool fromFileLine(const string& line, Book& book) {
        stringstream row(line);
        string idText, titleText, authorText, yearText, issuedText;

        if (!getline(row, idText, '|') || !getline(row, titleText, '|') ||
            !getline(row, authorText, '|') || !getline(row, yearText, '|') ||
            !getline(row, issuedText)) {
            return false;
        }

        try {
            const int bookId = stoi(idText);
            const int publicationYear = stoi(yearText);
            const bool isIssued = (issuedText == "1");

            if (bookId <= 0 || titleText.empty() || authorText.empty() ||
                publicationYear < 0 || (issuedText != "0" && issuedText != "1")) {
                return false;
            }

            book = Book(bookId, titleText, authorText, publicationYear, isIssued);
            return true;
        } catch (const exception&) {
            return false;
        }
    }
};

class Library {
private:
    vector<Book> books;

    int findBookIndex(int bookId) const {
        for (size_t index = 0; index < books.size(); ++index) {
            if (books[index].getId() == bookId) {
                return static_cast<int>(index);
            }
        }
        return -1;
    }

    void printBook(const Book& book) const {
        cout << left << setw(8) << book.getId()
             << setw(30) << book.getTitle().substr(0, 28)
             << setw(25) << book.getAuthor().substr(0, 23)
             << setw(8) << book.getYear()
             << (book.isIssued() ? "Issued" : "Available") << '\n';
    }

public:
    void loadFromFile() {
        ifstream input(DATA_FILE);
        string line;
        books.clear();

        while (getline(input, line)) {
            Book book;
            if (Book::fromFileLine(line, book) && findBookIndex(book.getId()) == -1) {
                books.push_back(book);
            }
        }
    }

    bool saveToFile() const {
        ofstream output(DATA_FILE);
        if (!output) {
            return false;
        }

        for (const Book& book : books) {
            output << book.toFileLine() << '\n';
        }
        return true;
    }

    bool addBook(const Book& book) {
        if (findBookIndex(book.getId()) != -1) {
            return false;
        }
        books.push_back(book);
        return true;
    }

    bool updateBook(int bookId, const string& title, const string& author, int year) {
        const int index = findBookIndex(bookId);
        if (index == -1) {
            return false;
        }

        books[index].updateDetails(title, author, year);
        return true;
    }

    bool deleteBook(int bookId) {
        const int index = findBookIndex(bookId);
        if (index == -1) {
            return false;
        }

        books.erase(books.begin() + index);
        return true;
    }

    bool issueBook(int bookId) {
        const int index = findBookIndex(bookId);
        if (index == -1 || books[index].isIssued()) {
            return false;
        }

        books[index].issue();
        return true;
    }

    bool returnBook(int bookId) {
        const int index = findBookIndex(bookId);
        if (index == -1 || !books[index].isIssued()) {
            return false;
        }

        books[index].returnBook();
        return true;
    }

    void displayBooks() const {
        cout << "\n--- Library Books ---\n";
        if (books.empty()) {
            cout << "No books are currently registered.\n";
            return;
        }

        cout << left << setw(8) << "ID"
             << setw(30) << "Title"
             << setw(25) << "Author"
             << setw(8) << "Year"
             << "Status\n";
        cout << string(84, '-') << '\n';

        for (const Book& book : books) {
            printBook(book);
        }
    }

    void searchBooks(const string& query) const {
        const string searchTerm = lowerCase(query);
        bool found = false;

        cout << "\n--- Search Results ---\n";
        for (const Book& book : books) {
            if (lowerCase(book.getTitle()).find(searchTerm) != string::npos ||
                lowerCase(book.getAuthor()).find(searchTerm) != string::npos) {
                if (!found) {
                    cout << left << setw(8) << "ID"
                         << setw(30) << "Title"
                         << setw(25) << "Author"
                         << setw(8) << "Year"
                         << "Status\n";
                    cout << string(84, '-') << '\n';
                }
                printBook(book);
                found = true;
            }
        }

        if (!found) {
            cout << "No book matched the title or author search.\n";
        }
    }

    bool hasBook(int bookId) const { return findBookIndex(bookId) != -1; }
    bool isBookIssued(int bookId) const {
        const int index = findBookIndex(bookId);
        return index != -1 && books[index].isIssued();
    }
};

void printMenu() {
    cout << "\n==============================================\n"
         << "          LIBRARY MANAGEMENT SYSTEM          \n"
         << "==============================================\n"
         << "1. Add book\n"
         << "2. View books\n"
         << "3. Search book by title or author\n"
         << "4. Update book information\n"
         << "5. Delete book\n"
         << "6. Issue book\n"
         << "7. Return book\n"
         << "8. Save data to file\n"
         << "9. Load data from file\n"
         << "0. Exit\n"
         << "==============================================\n";
}

void addBook(Library& library) {
    cout << "\n--- Add Book ---\n";
    const int id = readNumber("Book ID: ", 1, 1000000);

    if (library.hasBook(id)) {
        cout << "A book with that ID already exists.\n";
        return;
    }

    const string title = readText("Title: ");
    const string author = readText("Author: ");
    const int year = readNumber("Publication year (0 if unknown): ", 0, 3000);

    if (library.addBook(Book(id, title, author, year))) {
        library.saveToFile();
        cout << "Book added successfully.\n";
    }
}

void updateBook(Library& library) {
    cout << "\n--- Update Book Information ---\n";
    const int id = readNumber("Book ID: ", 1, 1000000);

    if (!library.hasBook(id)) {
        cout << "No book was found with that ID.\n";
        return;
    }

    const string title = readText("New title: ");
    const string author = readText("New author: ");
    const int year = readNumber("New publication year (0 if unknown): ", 0, 3000);

    library.updateBook(id, title, author, year);
    library.saveToFile();
    cout << "Book information updated.\n";
}

void deleteBook(Library& library) {
    cout << "\n--- Delete Book ---\n";
    const int id = readNumber("Book ID: ", 1, 1000000);

    if (library.deleteBook(id)) {
        library.saveToFile();
        cout << "Book deleted successfully.\n";
    } else {
        cout << "No book was found with that ID.\n";
    }
}

void issueBook(Library& library) {
    cout << "\n--- Issue Book ---\n";
    const int id = readNumber("Book ID: ", 1, 1000000);

    if (!library.hasBook(id)) {
        cout << "No book was found with that ID.\n";
    } else if (library.isBookIssued(id)) {
        cout << "That book is already issued.\n";
    } else {
        library.issueBook(id);
        library.saveToFile();
        cout << "Book issued successfully.\n";
    }
}

void returnBook(Library& library) {
    cout << "\n--- Return Book ---\n";
    const int id = readNumber("Book ID: ", 1, 1000000);

    if (!library.hasBook(id)) {
        cout << "No book was found with that ID.\n";
    } else if (!library.isBookIssued(id)) {
        cout << "That book is already available.\n";
    } else {
        library.returnBook(id);
        library.saveToFile();
        cout << "Book returned successfully.\n";
    }
}

int main() {
    Library library;
    library.loadFromFile();

    cout << "Library data loaded.\n";

    while (true) {
        printMenu();
        const int choice = readNumber("Choose an option: ", 0, 9);

        switch (choice) {
            case 1:
                addBook(library);
                break;
            case 2:
                library.displayBooks();
                break;
            case 3:
                library.searchBooks(readText("Enter a title or author keyword: "));
                break;
            case 4:
                updateBook(library);
                break;
            case 5:
                deleteBook(library);
                break;
            case 6:
                issueBook(library);
                break;
            case 7:
                returnBook(library);
                break;
            case 8:
                cout << (library.saveToFile() ? "Data saved successfully.\n"
                                               : "Unable to save library data.\n");
                break;
            case 9:
                library.loadFromFile();
                cout << "Data loaded successfully.\n";
                break;
            case 0:
                library.saveToFile();
                cout << "Library data saved. Goodbye!\n";
                return 0;
        }
    }
}
