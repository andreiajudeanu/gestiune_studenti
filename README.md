🇷🇴 [Versiunea în română](README.ro.md)

# Student Management – Binary Relative File (C/C++)

Assignment for the **Algorithms and Programming Techniques** course. The program manages a student registry stored in a **binary file with relative organization**, using only the standard library (C style) in a Visual Studio project.

> Note: identifiers in the source code and the console messages are in Romanian.

## Table of contents

- [What the program does](#what-the-program-does)
- [Repository structure](#repository-structure)
- [How a relative file works](#how-a-relative-file-works)
- [Record structure](#record-structure)
- [Features](#features)
- [Build and run](#build-and-run)
- [Usage example](#usage-example)
- [Generated files](#generated-files)
- [Known limitations](#known-limitations)

## What the program does

The application provides a console menu to create the student file, add records, look up a student directly by registration number, generate text reports, and "expel" students (logical deletion).

## Repository structure

```
.
├── gestiune_studenti.cpp             # source code
├── gestiune_studenti.slnx            # Visual Studio solution
├── gestiune_studenti.vcxproj         # project settings
├── gestiune_studenti.vcxproj.filters # file organization in the project
├── .gitignore
├── README.md
└── README.ro.md
```

The `x64/` and `.vs/` folders and the files `studenti.dat`, `studenti.txt`, `grupa_*.txt` are generated locally and excluded through `.gitignore`.

## How a relative file works

In a relative file, **the position of a record is computed directly from its key**. Here the key is the registration number (`nrMatricol`):

```
offset (in bytes) = nrMatricol * sizeof(Student)
```

To find the student with registration number `N`, the program does not scan the whole file. It jumps straight to that position with `fseek` (direct access, O(1)).

Consequences:

- Positions that do not correspond to any student are filled with "empty" records (`stare = '0'`).
- Position 0 is not used (registration numbers start at 1).
- The file grows up to the largest registration number entered.

### Binary vs. relative

"Binary" and "relative" describe different things. *Binary* refers to how data is written (`fwrite`/`fread`, raw bytes as in memory, not text). *Relative* refers to how records are organized (position computed from the key). This file is binary in format and relative in organization.

### Logical deletion

A student is not physically removed from the file. When expelled, the `stare` field changes from `'1'` (occupied) to `'0'` (free), and the record stays in place. All listings ignore records with `stare = '0'`.

## Record structure

```c
typedef struct {
    char  stare;       // '0' = free, '1' = occupied
    int   nrMatricol;  // relative key (registration number)
    char  nume[20];    // name
    int   an;          // year of study
    int   grup;        // group
    float note[10];    // 10 grades
} Student;
```

All records have the same size (`sizeof(Student)`), which is required for computing positions with `fseek`.

## Features

| Option | Function | Description |
|---|---|---|
| 1 | `creareFisier()` | Creates `studenti.dat` from scratch (mode `wb`) from keyboard input. **Overwrites** an existing file. |
| 2 | `adaugareFisier()` | Adds students to an existing file (`rb+`); creates it if missing (`wb+`). Extends the file with empty records when needed. |
| 3 | `listareFisierText()` | Writes all active students, as a table with their average, to `studenti.txt`. |
| 4 | `afisareDupaNrMatricol()` | Displays a student through direct access by registration number. |
| 5 | `listarePeGrupe()` | For each group entered, generates a `grupa_<nr>.txt` file with the students of that group. |
| 6 | `exmatriculare()` | Marks the given students as inactive (logical deletion). |
| 0 | – | Exit. |

Input stops when a registration number (or group) `<= 0` is entered.

### File open modes used

Files are opened with `fopen_s`, the secure Visual C++ variant.

| Mode | Meaning |
|---|---|
| `wb` | binary write, creates/truncates the file |
| `rb` | binary read |
| `rb+` | binary read + write on an existing file |
| `wb+` | binary read + write, new file |

## Build and run

The code uses the "secure" functions `fopen_s`, `scanf_s` and `sprintf_s`, so it builds with **Microsoft Visual C++** (Visual Studio).

### From Visual Studio

1. Clone the repository:
   ```bash
   git clone https://github.com/USERNAME/REPO-NAME.git
   ```
2. Open `gestiune_studenti.slnx` in Visual Studio (solution format available in Visual Studio 2022 version 17.13 or newer).
3. Select the `Debug` / `x64` configuration and press **Ctrl+F5** (Start Without Debugging).

The files `studenti.dat`, `studenti.txt` and `grupa_*.txt` are created in the project's working directory.

### From the command line

In a *Developer Command Prompt for VS*:

```bash
cl /EHsc gestiune_studenti.cpp
gestiune_studenti.exe
```

> Note: GCC/Clang on Linux and macOS usually do not provide `fopen_s`, `scanf_s` and `sprintf_s`, so the program will not compile there without changes (use `fopen`, `scanf`, `snprintf`).

## Usage example

```
Optiunea: 1
Numar matricol: 5
Nume: Popescu
An: 2
Grup: 301
Nota 1: 9
...
Numar matricol: 0

Optiunea: 4
Nr. matricol: 5
  Nume: Popescu
  An: 2, Grup: 301
  Media: 8.70
```

## Generated files

- `studenti.dat` – the binary relative file (main data)
- `studenti.txt` – table listing of all active students
- `grupa_<nr>.txt` – the report for each requested group

These appear at runtime and are not uploaded to GitHub (see `.gitignore`).

## Known limitations

- The name is read with `scanf_s("%s", ...)` plus the buffer size: it does not accept spaces, and a name longer than 19 characters is not read (the read fails, so there is no buffer overflow).
- On creation, the internal buffer holds at most 100 students.
- The type of the input is not validated (a letter instead of a number can block reading).
- A very large registration number makes the file very large, since all intermediate positions are filled with empty records.
- The `.dat` file depends on the platform (struct size and byte order), so it is not guaranteed to be portable across different systems.

## Author

Andrei – assignment for Algorithms and Programming Techniques.
