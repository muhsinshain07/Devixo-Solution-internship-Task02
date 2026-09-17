# Library Management System — Project Report

## 1. Overview

The Library Management System is a console application written in standard C++. It maintains a collection of books and provides the operations requested in Task 02: adding, viewing, searching, updating, deleting, issuing, returning, saving, and loading records.

## 2. Design approach

The application uses two classes. The `Book` class models one library book and stores its ID, title, author, publication year, and issue status. Its constructor initializes a complete object. Methods are provided to update details, issue a book, return a book, and convert a record to or from the file format.

The `Library` class owns a vector of `Book` objects. It handles collection-level behavior such as locating a book by ID, displaying records, searching, adding, updating, deleting, issuing, returning, and reading or writing the data file. This separation keeps book data and library operations organized.

## 3. User interaction

The main function loads existing data and enters a menu loop. Each menu operation is handled by a focused function or class method. Numeric input is checked for valid ranges. Text input rejects blank values and the pipe character used as the file delimiter. Book IDs are unique, and the program prevents issuing an already-issued book or returning a book that is already available.

## 4. Search functionality

The search feature satisfies the bonus requirement. It converts both the stored text and the user's keyword to lowercase, then checks whether the keyword appears in either the title or author field. This makes the search case-insensitive and allows partial matches.

## 5. File handling

Records are stored in `library_books.txt`. Each line contains five fields separated by `|`: book ID, title, author, publication year, and issue status. The program loads valid lines when it starts, ignores malformed lines, and saves after changes as well as when the user selects the save or exit option.

## 6. Requirements coverage

| Requirement | Implementation |
| --- | --- |
| Classes and objects | `Book` and `Library` classes. |
| Constructors | `Book` constructor initializes each book. |
| Functions | Input, menu, CRUD, issue/return, search, and persistence functions. |
| File handling | `loadFromFile`, `saveToFile`, and `Book::fromFileLine`. |
| Arrays or vectors | `vector<Book>` stores the current collection. |
| Menu-driven programming | A validated loop in `main`. |
| Input validation | Range checks, blank-input checks, duplicate ID checks, and file validation. |
| Bonus search | Case-insensitive title and author keyword search. |

## 7. Testing

The program was compiled with C++17 and warnings enabled. A scripted test covered adding a book, viewing it, searching by author, updating its details, issuing it, returning it, deleting it, and saving and reloading the data file. Duplicate IDs and invalid numeric values are rejected by the input layer.

## 8. Conclusion

The completed program meets the task requirements while keeping the implementation readable and maintainable. The use of classes, a vector, separate functions, and a simple text format provides a practical introduction to object-oriented programming and file handling in C++.
