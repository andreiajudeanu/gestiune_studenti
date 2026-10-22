#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char stare;
    int nrMatricol;
    char nume[20];
    int an;
    int grup;
    float note[10];
} Student;

const char* FISIER_BINAR = "studenti.dat";


void creareFisier() {
    FILE* f = NULL;
    Student s;
    Student gol;
    int i;
    int maxNrMat = 0;
    int nrStudenti = 0;
    Student buffer[100];

    if (fopen_s(&f, FISIER_BINAR, "wb") != 0 || f == NULL) {
        printf("Eroare la crearea fisierului!\n");
        return;
    }

    printf("=== CREARE FISIER RELATIV ===\n");
    printf("Introduceti datele studentilor (nr. matricol <= 0 pentru stop):\n\n");

    while (1) {
        printf("Numar matricol: ");
        scanf_s("%d", &s.nrMatricol);

        if (s.nrMatricol <= 0)
            break;

        s.stare = '1';

        printf("Nume: ");
        scanf_s("%s", s.nume, (unsigned)sizeof(s.nume));

        printf("An: ");
        scanf_s("%d", &s.an);

        printf("Grup: ");
        scanf_s("%d", &s.grup);

        printf("Introduceti cele 10 note:\n");
        for (i = 0; i < 10; i++) {
            printf("  Nota %d: ", i + 1);
            scanf_s("%f", &s.note[i]);
        }

        buffer[nrStudenti] = s;

        if (s.nrMatricol > maxNrMat)
            maxNrMat = s.nrMatricol;

        nrStudenti++;
        printf("\n");
    }

    memset(&gol, 0, sizeof(Student));
    gol.stare = '0';

    for (i = 0; i <= maxNrMat; i++) {
        fwrite(&gol, sizeof(Student), 1, f);
    }

    for (i = 0; i < nrStudenti; i++) {
        fseek(f, (long)buffer[i].nrMatricol * sizeof(Student), SEEK_SET);
        fwrite(&buffer[i], sizeof(Student), 1, f);
    }

    fclose(f);
    printf("\nFisierul a fost creat cu succes! (%d studenti)\n", nrStudenti);
}


void adaugareFisier() {
    FILE* f = NULL;
    Student s;
    Student gol;
    int i;
    long dimFisier;
    int nrArticole;

    if (fopen_s(&f, FISIER_BINAR, "rb+") != 0 || f == NULL) {
        printf("Fisierul nu exista. Se creeaza unul nou.\n");

        if (fopen_s(&f, FISIER_BINAR, "wb+") != 0 || f == NULL) {
            printf("Eroare la crearea fisierului!\n");
            return;
        }
    }

    printf("=== ADAUGARE STUDENTI ===\n");
    printf("Introduceti datele studentilor (nr. matricol <= 0 pentru stop):\n\n");

    fseek(f, 0, SEEK_END);
    dimFisier = ftell(f);
    nrArticole = dimFisier / sizeof(Student);

    memset(&gol, 0, sizeof(Student));
    gol.stare = '0';

    while (1) {
        printf("Numar matricol: ");
        scanf_s("%d", &s.nrMatricol);

        if (s.nrMatricol <= 0)
            break;

        s.stare = '1';

        printf("Nume: ");
        scanf_s("%s", s.nume, (unsigned)sizeof(s.nume));

        printf("An: ");
        scanf_s("%d", &s.an);

        printf("Grup: ");
        scanf_s("%d", &s.grup);

        printf("Introduceti cele 10 note:\n");
        for (i = 0; i < 10; i++) {
            printf("  Nota %d: ", i + 1);
            scanf_s("%f", &s.note[i]);
        }

        if (s.nrMatricol >= nrArticole) {
            fseek(f, 0, SEEK_END);
            for (i = nrArticole; i <= s.nrMatricol; i++) {
                fwrite(&gol, sizeof(Student), 1, f);
            }
            nrArticole = s.nrMatricol + 1;
        }

        fseek(f, (long)s.nrMatricol * sizeof(Student), SEEK_SET);
        fwrite(&s, sizeof(Student), 1, f);

        printf("Student adaugat cu succes!\n\n");
    }

    fclose(f);
    printf("Adaugare terminata.\n");
}


void listareFisierText() {
    FILE* f = NULL;
    FILE* ftext = NULL;
    Student s;
    int i;
    float medie;

    if (fopen_s(&f, FISIER_BINAR, "rb") != 0 || f == NULL) {
        printf("Eroare: fisierul binar nu exista!\n");
        return;
    }

    if (fopen_s(&ftext, "studenti.txt", "w") != 0 || ftext == NULL) {
        printf("Eroare la crearea fisierului text!\n");
        fclose(f);
        return;
    }

    printf("=== LISTARE IN FISIER TEXT ===\n");

    fprintf(ftext, "==================================================================================\n");
    fprintf(ftext, "| %-4s | %-6s | %-18s | %-2s | %-4s | %-6s |",
        "Stare", "NrMat", "Nume", "An", "Grup", "Media");

    for (i = 0; i < 10; i++) {
        fprintf(ftext, " N%-2d |", i + 1);
    }
    fprintf(ftext, "\n");
    fprintf(ftext, "==================================================================================\n");

    while (fread(&s, sizeof(Student), 1, f) == 1) {
        if (s.stare == '1') {
            medie = 0;
            for (i = 0; i < 10; i++) {
                medie += s.note[i];
            }
            medie /= 10.0;

            fprintf(ftext, "| %-4c | %-6d | %-18s | %-2d | %-4d | %-6.2f |",
                s.stare, s.nrMatricol, s.nume, s.an, s.grup, medie);

            for (i = 0; i < 10; i++) {
                fprintf(ftext, "%4.1f |", s.note[i]);
            }
            fprintf(ftext, "\n");
        }
    }

    fprintf(ftext, "==================================================================================\n");

    fclose(f);
    fclose(ftext);
    printf("Listarea a fost scrisa in 'studenti.txt'.\n");
}


void afisareDupaNrMatricol() {
    FILE* f = NULL;
    Student s;
    int nrMat;
    int i;
    float medie;

    if (fopen_s(&f, FISIER_BINAR, "rb") != 0 || f == NULL) {
        printf("Eroare: fisierul nu exista!\n");
        return;
    }

    printf("=== AFISARE DUPA NR. MATRICOL ===\n");
    printf("Introduceti numere matricole (<=0 pentru stop):\n\n");

    while (1) {
        printf("Nr. matricol: ");
        scanf_s("%d", &nrMat);

        if (nrMat <= 0)
            break;

        fseek(f, (long)nrMat * sizeof(Student), SEEK_SET);

        if (fread(&s, sizeof(Student), 1, f) == 1) {
            if (s.stare == '1') {
                printf("  Nume: %s\n", s.nume);
                printf("  An: %d, Grup: %d\n", s.an, s.grup);
                printf("  Note: ");
                medie = 0;
                for (i = 0; i < 10; i++) {
                    printf("%.1f ", s.note[i]);
                    medie += s.note[i];
                }
                medie /= 10.0;
                printf("\n  Media: %.2f\n\n", medie);
            }
            else {
                printf("  Nu exista student cu nr. matricol %d (pozitie libera).\n\n", nrMat);
            }
        }
        else {
            printf("  Nr. matricol %d nu exista in fisier.\n\n", nrMat);
        }
    }

    fclose(f);
}


void listarePeGrupe() {
    FILE* f = NULL;
    FILE* ftext = NULL;
    Student s;
    int grupa;
    int i;
    float medie;
    char numeFisier[50];
    int gasit;

    if (fopen_s(&f, FISIER_BINAR, "rb") != 0 || f == NULL) {
        printf("Eroare: fisierul nu exista!\n");
        return;
    }

    printf("=== LISTARE PE GRUPE ===\n");
    printf("Introduceti numere de grupa (<=0 pentru stop):\n\n");

    while (1) {
        printf("Grupa: ");
        scanf_s("%d", &grupa);

        if (grupa <= 0)
            break;

        sprintf_s(numeFisier, sizeof(numeFisier), "grupa_%d.txt", grupa);

        if (fopen_s(&ftext, numeFisier, "w") != 0 || ftext == NULL) {
            printf("Eroare la crearea fisierului %s!\n", numeFisier);
            continue;
        }

        fprintf(ftext, "SITUATIA STUDENTILOR DIN GRUPA %d\n", grupa);
        fprintf(ftext, "============================================================\n");
        fprintf(ftext, "| %-6s | %-18s | %-2s |", "NrMat", "Nume", "An");
        for (i = 0; i < 10; i++) {
            fprintf(ftext, " N%-2d |", i + 1);
        }
        fprintf(ftext, " Media |\n");
        fprintf(ftext, "============================================================\n");

        fseek(f, 0, SEEK_SET);
        gasit = 0;

        while (fread(&s, sizeof(Student), 1, f) == 1) {
            if (s.stare == '1' && s.grup == grupa) {
                gasit = 1;

                medie = 0;
                fprintf(ftext, "| %-6d | %-18s | %-2d |", s.nrMatricol, s.nume, s.an);
                for (i = 0; i < 10; i++) {
                    fprintf(ftext, "%4.1f |", s.note[i]);
                    medie += s.note[i];
                }
                medie /= 10.0;
                fprintf(ftext, " %5.2f |\n", medie);
            }
        }

        if (!gasit) {
            fprintf(ftext, "| Nu exista studenti in aceasta grupa.                     |\n");
        }

        fprintf(ftext, "============================================================\n");
        fclose(ftext);
        printf("Fisierul '%s' a fost creat.\n\n", numeFisier);
    }

    fclose(f);
}


void exmatriculare() {
    FILE* f = NULL;
    Student s;
    int nrMat;

    if (fopen_s(&f, FISIER_BINAR, "rb+") != 0 || f == NULL) {
        printf("Eroare: fisierul nu exista!\n");
        return;
    }

    printf("=== EXMATRICULARE STUDENTI ===\n");
    printf("Introduceti numere matricole (<=0 pentru stop):\n\n");

    while (1) {
        printf("Nr. matricol pentru exmatriculare: ");
        scanf_s("%d", &nrMat);

        if (nrMat <= 0)
            break;

        fseek(f, (long)nrMat * sizeof(Student), SEEK_SET);

        if (fread(&s, sizeof(Student), 1, f) == 1) {
            if (s.stare == '1') {
                s.stare = '0';

                fseek(f, (long)nrMat * sizeof(Student), SEEK_SET);
                fwrite(&s, sizeof(Student), 1, f);
                fflush(f);

                printf("  Studentul '%s' (nr. mat. %d) a fost exmatriculat.\n\n",
                    s.nume, nrMat);
            }
            else {
                printf("  Nu exista student activ cu nr. matricol %d.\n\n", nrMat);
            }
        }
        else {
            printf("  Nr. matricol %d nu exista in fisier.\n\n", nrMat);
        }
    }

    fclose(f);
    printf("Exmatriculare terminata.\n");
}


int main() {
    int optiune;

    while (1) {
        printf("\n");
        printf("========================================\n");
        printf("   GESTIUNE STUDENTI - FISIER RELATIV   \n");
        printf("========================================\n");
        printf("1. Creare fisier relativ\n");
        printf("2. Adaugare studenti\n");
        printf("3. Listare in fisier text (tabel)\n");
        printf("4. Afisare dupa nr. matricol\n");
        printf("5. Listare pe grupe (fisiere text)\n");
        printf("6. Exmatriculare studenti\n");
        printf("0. Iesire\n");
        printf("----------------------------------------\n");
        printf("Optiunea: ");
        scanf_s("%d", &optiune);
        printf("\n");

        switch (optiune) {
        case 1: creareFisier();          break;
        case 2: adaugareFisier();        break;
        case 3: listareFisierText();     break;
        case 4: afisareDupaNrMatricol(); break;
        case 5: listarePeGrupe();        break;
        case 6: exmatriculare();         break;
        case 0:
            printf("La revedere!\n");
            return 0;
        default:
            printf("Optiune invalida!\n");
        }
    }

    return 0;
}
