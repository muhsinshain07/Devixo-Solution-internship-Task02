# Library Management System

This console-based C++ application implements Task 02 of the Devixo Solutions C++ Programming Internship. It demonstrates classes and objects, constructors, functions, file handling, vectors, menu-driven programming, and input validation.

## Features

The program can add, view, search, update, and delete books. It also supports issuing and returning books, manual save/load operations, and automatic persistence in `library_books.txt`.

The bonus requirement is included: searching is case-insensitive and checks both the book title and author. A partial keyword can be used.

## Requirements

- C++17-compatible compiler
- VS Code or another C++ development environment

## Build

```bash
g++ -std=c++17 -Wall -Wextra -pedantic library_management_system.cpp -o library_management_system
```

## Run

Linux or macOS:

```bash
./library_management_system
```

Windows PowerShell:

```powershell
.\library_management_system.exe
```

## Menu

1. Add a book.
2. View all books.
3. Search by title or author.
4. Update book information.
5. Delete a book.
6. Issue a book.
7. Return a book.
8. Save data to the file.
9. Reload data from the file.
0. Save and exit.

## Data file

The application uses `library_books.txt` in its current working directory. Each record is stored as a pipe-separated line containing the ID, title, author, publication year, and issue status. The file is created automatically after the first successful save.

## Project files

- `library_management_system.cpp` — complete source code.
- `README.md` — setup and usage instructions.
- `report.md` — project report.
- `sample_output.txt` — representative console transcript.
