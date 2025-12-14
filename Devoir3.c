#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

typedef struct {
    char* nom;
    mode_t mode;
    off_t taille;
} info_fichier;

#define MAX_FICHIER 100

int main(int argc, char *argv[]) {

// Le code de désarchivage
    if (argc == 3 && strcmp(argv[1], "-e") == 0) {

        FILE *f = fopen(argv[2], "r");
        if (!f) {
            perror("Erreur ouverture archive");
            return 1;
        }

        int nb_fichiers;
        fscanf(f, "le nombre de fichiers est: %d\n", &nb_fichiers);

        info_fichier infos[MAX_FICHIER];
        char ligne[1024];
        fgets(ligne, sizeof(ligne), f);
        fgets(ligne, sizeof(ligne), f);

        for (int i = 0; i < nb_fichiers; i++) {
            infos[i].nom = malloc(256);
            fscanf(f, "%s %o %ld\n",
                   infos[i].nom,
                   &infos[i].mode,
                   &infos[i].taille);
        }
        while (fgets(ligne, sizeof(ligne), f)) {
            if (strcmp(ligne, "Le contenu de fichier archivé\n") == 0)
                break;
        }
        for (int i = 0; i < nb_fichiers; i++) {

            FILE *out = fopen(infos[i].nom, "w");
            if (!out) {
                perror(infos[i].nom);
                continue;
            }
            for (off_t j = 0; j < infos[i].taille; j++) {
                if (fgets(ligne, sizeof(ligne), f) != NULL) {
                    fputs(ligne, out);   
                }
            }
            fclose(out);
            chmod(infos[i].nom, infos[i].mode);
            free(infos[i].nom);
        }
        fclose(f);
        printf("Desarchivage termine avec succes\n");
        return 0;
    }

// Le code de l'sarchivage
    if (argc < 3) {
        printf("Usage archivage   : %s archive fichier1 [fichier2 ...]\n", argv[0]);
        printf("Usage du desarchivage: %s -e archive\n", argv[0]);
        return 1;
    }
    int nb_arg = argc - 2;
    const char *fdarchive = argv[1];

    FILE *f = fopen(fdarchive, "w");
    if (!f) {
        perror("Erreur creation archive");
        return 1;
    }
    fprintf(f, "le nombre de fichiers est: %d\n", nb_arg);
    fprintf(f, "Nom_du_fichier\tMode_fichier\tTaille_fichier\n");
    fprintf(f, "--------------------------------------------\n");

    for (int i = 2; i < argc; i++) {
        struct stat st;
        if (stat(argv[i], &st) == -1) {
            perror(argv[i]);
            continue;
        }
        fprintf(f, "%s %o %ld\n",
                argv[i], st.st_mode, (long)st.st_size);
    }
    fprintf(f, "Le contenu de fichier archivé\n");

    char buffer[1024];
    for (int i = 2; i < argc; i++) {
        FILE *src = fopen(argv[i], "r");
        if (!src) {
            perror(argv[i]);
            continue;
        }
        while (fgets(buffer, sizeof(buffer), src)) {
            fputs(buffer, f); 
        }
        fclose(src);
    }
    fclose(f);
    printf("Archivage termine avec succes dans %s\n", fdarchive);

    return 0;
}
