#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>


void create_arbre(int niveau , int k){

    if(niveau > 0 ){
    printf(" %d -> %d; \n",getppid() , getpid());
    }
    if (niveau==k){
        return;
    }
    int pid_fils1=fork();
    if(pid_fils1<0){
        printf("erreur lors du forkage");
        exit(1);
    }
    if(pid_fils1==0){
        create_arbre(niveau+1,k);
        exit(0);
    }
    int pid_fils2=fork();
    if(pid_fils2<0){
        printf("erreur lors du forkage");
        exit(1);
    }
    if(pid_fils2==0){
        create_arbre(niveau+1,k);
        exit(0);
    }
    waitpid(pid_fils1,NULL,0);
    waitpid(pid_fils2,NULL,0);
}

int main (int argc , char*argv[]){

    int pid_fils;
    int nb_fork;
    if(argc < 2){
        printf("erreur nombre dargument insuffisant \n");
        return  1;
    }
    
    nb_fork= atoi(argv[1]);
    printf("la valeur de l'arbre apres les forkages\n");
    create_arbre(0,nb_fork);
    return 0;
}