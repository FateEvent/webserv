#include <stdio.h>
#include <stdlib.h>

#define MAX_NUMBERS 12000

int main() {
    int numbers[MAX_NUMBERS] = {0}; // initialisation du tableau à 0
    int count[MAX_NUMBERS] = {0}; // initialisation du tableau à 0
    int num, i, j;
    int num_count = 0;

    // Ouverture du fichier
    FILE *fp = fopen("test.txt", "r");
    if (fp == NULL) {
        printf("Impossible d'ouvrir le fichier\n");
        exit(1);
    }

    // Lecture des nombres dans le fichier et comptage des occurrences
    while (fscanf(fp, "%d", &num) != EOF) {
        // Vérification si le nombre a déjà été rencontré
        for (i = 0; i < num_count; i++) {
            if (num == numbers[i]) {
                count[i]++;
                break;
            }
        }
        // Ajout du nombre au tableau s'il n'a jamais été rencontré
        if (i == num_count) {
            numbers[num_count] = num;
            count[num_count]++;
            num_count++;
        }
    }

    // Affichage des nombres qui n'ont été rencontrés qu'une seule fois
    printf("Nombres qui n'ont pas été répétés :\n");
    for (i = 0; i < num_count; i++) {
        if (count[i] == 1) {
            printf("%d\n", numbers[i]);
        }
    }

    // Fermeture du fichier
    fclose(fp);

    return 0;
}