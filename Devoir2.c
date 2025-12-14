#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

extern char *optarg;
extern int optind, opterr, optopt;
int demande_de_creer = 0;
int demande_affichage = 0;
int mode_binaire = 1;     
char *nomfichier = NULL;

void remplir_matr(int **matrice,int dim){
    srand(time(NULL));
    printf("remplissage de la matrice reussi\n");
    for(int i=0 ; i < dim;i++){
        for(int j=0 ; j < dim;j++){
                matrice[i][j]=rand() % 100;
        }

    }
}

void afficher_matr(int **matrice , int dim){
    for(int i=0 ; i < dim;i++){
        for(int j=0 ; j < dim;j++){
                printf("%3d",matrice[i][j]);
        }
        printf("\n");

    }
}

void write_binaire(char *nomfichier, int **mat, int dim) {
    FILE *f = fopen(nomfichier, "wb");
    if (!f) {
        printf("Erreur ouverture du fichier en mode binaire.\n");
        exit(1);
    }

    fwrite(&dim, sizeof(int), 1, f); 
    for (int i = 0; i < dim; i++)
        fwrite(mat[i], sizeof(int), dim, f); 

    fclose(f);
}

void write_texte(char *nomfichier, int **mat, int dim) {
    FILE *f = fopen(nomfichier, "w");
    if (!f) {
        printf("Erreur ouverture du fichier en mode texte.\n");
        exit(1);
    }

    fprintf(f, "%d\n", dim); 

    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++)
            fprintf(f, "%d ", mat[i][j]);
        fprintf(f, "\n");
    }

    fclose(f);

}

int** read_binaire(char *nomfichier, int *n) {
    FILE *f = fopen(nomfichier, "rb");
    if (!f) {
        printf("Erreur ouverture fichier en mode binaire.\n");
        exit(1);
    }

    fread(n, sizeof(int), 1, f);

    int **mat = malloc(*n * sizeof(int*));
    for (int i = 0; i < *n; i++)
        mat[i] = malloc(*n * sizeof(int));

    for (int i = 0; i < *n; i++)
        fread(mat[i], sizeof(int), *n, f);

    fclose(f);
    return mat;
}

int** read_texte(char *nomfichier, int *n) {
    FILE *f = fopen(nomfichier, "r");
    if (!f) {
        printf("Erreur ouverture fichier en mode texte.\n");
        exit(1);
    }

    fscanf(f, "%d", n);

    int **mat = malloc(*n * sizeof(int*));
    for (int i = 0; i < *n; i++)
        mat[i] = malloc(*n * sizeof(int));

    for (int i = 0; i < *n; i++)
        for (int j = 0; j < *n; j++)
            fscanf(f, "%d", &mat[i][j]);

    fclose(f);
    return mat;
}

int main(int argc , char*argv[]){
    int opt;
    int **matrice=NULL;
    int dimension=0;
    srand(time(NULL));

// d'apres mon comprehension -c c pour faire une demande de creation de matrice donc j'ai juste ecrit demande 
// de creation d'une matrice 

    while((opt = getopt (argc , argv , "cd:abtf:")) != -1){

        switch(opt){
            case 'c':
            printf("demande de creation dune matrice reussi \n");
            demande_de_creer=1;
            break;
            case 'd':
            if(!demande_de_creer){
                printf("erreur on a pas demander a creer une matrice \n ");
                return 1;
            }
            dimension=atoi(optarg);
            matrice=(int**)malloc(dimension * sizeof(int*));
            if(matrice==NULL){
                printf("erreur lors dallocation \n");
                return 1;
            }
            for(int i=0; i < dimension ;i++){
                matrice[i]=(int*)malloc(dimension * sizeof(int));
                if(matrice[i]==NULL){
                    printf("erreur lors dallocation \n");
                    return 1;
                }
            }
            remplir_matr(matrice,dimension);

            break;
            case 'a': 
            demande_affichage = 1;
            break;
            case 'b':
            mode_binaire = 1;
            printf("Mode binaire chois (par defaut aussi).\n");
            break;
            case 't':
            mode_binaire = 0;
            printf("Mode texte choisi.\n");
            break;
            case 'f':
            nomfichier = optarg;
            printf("le fichier selectionner est : %s\n", nomfichier);
            break;
            case '?':
            printf("erreur : option -%c inconnue\n",optopt);

        }

    }

    if (demande_de_creer) {
        if (nomfichier == NULL) {
            printf("Erreur : -f nomfichier est obligatoire pour creer une matrice.\n");
            return 1;
        }

        if (mode_binaire)
            write_binaire(nomfichier, matrice, dimension);
        else
            write_texte(nomfichier, matrice, dimension);
    }
    if (demande_affichage) {
        if (nomfichier == NULL) {
            printf("Erreur : aucun fichier nest fourni (-f).\n");
            return 1;
        }

        if (mode_binaire)
            matrice = read_binaire(nomfichier, &dimension);
        else
            matrice = read_texte(nomfichier, &dimension);

        afficher_matr(matrice, dimension);
    }
    return 0;
}