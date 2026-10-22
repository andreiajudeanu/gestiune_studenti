# Gestiune studenți – fișier binar cu acces relativ (C/C++)

Temă pentru materia **Algoritmi și Tehnici de Programare**. Programul gestionează o evidență de studenți stocată într-un **fișier binar cu organizare relativă**, folosind doar biblioteca standard (în stil C), într-un proiect Visual Studio.

## Cuprins

- [Ce face programul](#ce-face-programul)
- [Structura repository-ului](#structura-repository-ului)
- [Cum funcționează un fișier relativ](#cum-funcționează-un-fișier-relativ)
- [Structura unui articol](#structura-unui-articol)
- [Funcționalități](#funcționalități)
- [Compilare și rulare](#compilare-și-rulare)
- [Exemplu de utilizare](#exemplu-de-utilizare)
- [Fișiere generate](#fișiere-generate)
- [Limitări cunoscute](#limitări-cunoscute)

## Ce face programul

Aplicația oferă un meniu în consolă cu care poți crea fișierul de studenți, adăuga înregistrări, căuta direct după număr matricol, genera rapoarte text și „exmatricula” studenți (ștergere logică).

## Structura repository-ului

```
.
├── gestiune_studenti.cpp             # codul sursă
├── gestiune_studenti.slnx            # soluția Visual Studio
├── gestiune_studenti.vcxproj         # setările proiectului
├── gestiune_studenti.vcxproj.filters # organizarea fișierelor în proiect
├── .gitignore
└── README.md
```

Folderele `x64/`, `.vs/` și fișierele `studenti.dat`, `studenti.txt`, `grupa_*.txt` sunt generate local și sunt excluse prin `.gitignore`.

## Cum funcționează un fișier relativ

Într-un fișier relativ, **poziția unui articol în fișier este calculată direct din cheie**. Aici cheia este numărul matricol:

```
offset (în octeți) = nrMatricol * sizeof(Student)
```

Astfel, pentru a găsi studentul cu numărul matricol `N` nu se parcurge tot fișierul, ci se face un salt direct cu `fseek` la poziția respectivă (acces direct, complexitate O(1)).

Consecințe:

- Pozițiile care nu corespund niciunui student sunt umplute cu articole „goale” (`stare = '0'`).
- Poziția 0 nu este folosită (numerele matricole încep de la 1).
- Fișierul crește până la cel mai mare număr matricol introdus.

### Ștergerea logică

Studentul nu este eliminat fizic din fișier. La exmatriculare, câmpul `stare` se schimbă din `'1'` (ocupat) în `'0'` (liber), iar articolul rămâne pe loc. Toate listările ignoră articolele cu `stare = '0'`.

## Structura unui articol

```c
typedef struct {
    char  stare;       // '0' = liber, '1' = ocupat
    int   nrMatricol;  // cheia relativă
    char  nume[20];
    int   an;
    int   grup;
    float note[10];
} Student;
```

Toate articolele au aceeași dimensiune (`sizeof(Student)`), condiție necesară pentru calculul poziției prin `fseek`.

## Funcționalități

| Opțiune | Funcție | Descriere |
|---|---|---|
| 1 | `creareFisier()` | Creează `studenti.dat` de la zero (modul `wb`) din datele citite de la tastatură. **Suprascrie** fișierul existent. |
| 2 | `adaugareFisier()` | Adaugă studenți într-un fișier existent (`rb+`); dacă nu există, îl creează (`wb+`). Extinde fișierul cu articole goale dacă e nevoie. |
| 3 | `listareFisierText()` | Scrie toți studenții activi, sub formă de tabel cu media, în `studenti.txt`. |
| 4 | `afisareDupaNrMatricol()` | Afișează un student prin acces direct, pe baza numărului matricol. |
| 5 | `listarePeGrupe()` | Pentru fiecare grupă introdusă, generează un fișier `grupa_<nr>.txt` cu situația studenților din grupă. |
| 6 | `exmatriculare()` | Marchează studenții indicați ca inactivi (ștergere logică). |
| 0 | – | Ieșire din program. |

Introducerea datelor se oprește la un număr matricol (sau grupă) `<= 0`.

### Moduri de deschidere folosite

Fișierele se deschid cu `fopen_s`, varianta sigură din Visual C++.

| Mod | Semnificație |
|---|---|
| `wb` | scriere binară, creează/golește fișierul |
| `rb` | citire binară |
| `rb+` | citire + scriere binară pe fișier existent |
| `wb+` | citire + scriere binară, fișier nou |

## Compilare și rulare

Codul folosește funcțiile „sigure” `fopen_s`, `scanf_s` și `sprintf_s`, deci se compilează cu **Microsoft Visual C++** (Visual Studio).

### Din Visual Studio

1. Clonează repository-ul:
   ```bash
   git clone https://github.com/USERNAME/NUME-REPO.git
   ```
2. Deschide `gestiune_studenti.slnx` cu Visual Studio (format de soluție disponibil în Visual Studio 2022 versiunea 17.13 sau mai nouă).
3. Alege configurația `Debug` / `x64` și apasă **Ctrl+F5** (Start Without Debugging).

Fișierele `studenti.dat`, `studenti.txt` și `grupa_*.txt` vor fi create în directorul de lucru al proiectului.

### Din linia de comandă

Într-un *Developer Command Prompt for VS*:

```bash
cl /EHsc gestiune_studenti.cpp
gestiune_studenti.exe
```

> Notă: GCC/Clang pe Linux și macOS nu includ de obicei `fopen_s`, `scanf_s` și `sprintf_s`, deci programul nu se compilează acolo fără modificări (se folosesc `fopen`, `scanf`, `snprintf`).

## Exemplu de utilizare

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

## Fișiere generate

- `studenti.dat` – fișierul binar relativ (datele principale)
- `studenti.txt` – listarea tabelară a tuturor studenților activi
- `grupa_<nr>.txt` – situația fiecărei grupe cerute

Acestea apar la rulare și nu sunt urcate pe GitHub (vezi `.gitignore`).

## Limitări cunoscute

- Numele se citește cu `scanf_s("%s", ...)` și dimensiunea bufferului: nu acceptă spații, iar un nume mai lung de 19 caractere nu este citit (citirea eșuează, deci nu apare depășire de buffer).
- La creare, bufferul intern reține maxim 100 de studenți.
- Nu se validează tipul datelor introduse (o literă în loc de număr poate bloca citirea).
- Un număr matricol foarte mare face fișierul foarte mare, deoarece toate pozițiile intermediare sunt umplute cu articole goale.
- Fișierul `.dat` depinde de platformă (dimensiunea structurii și ordinea octeților), deci nu e garantat portabil între sisteme diferite.

## Autor

Andrei – temă pentru Algoritmi și Tehnici de Programare.
