#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

int main(int argc, char *argv[]) {
    char cmd[255];
    char *argvExec[255];
    char *commande;
    int i;
    char *p1, *p2;

    while (1) {
        printf("Minishell > ");
        if (fgets(cmd, sizeof(cmd), stdin) == NULL)
            break;

        cmd[strcspn(cmd, "\n")] = '\0';
        commande = strtok_r(cmd, ";", &p1);
        while (commande != NULL) {
            for (i = 0; i < 255; i++)
                argvExec[i] = NULL;
            i = 0;
            argvExec[i] = strtok_r(commande, " ", &p2);
            while (argvExec[i] != NULL) {
                i++;
                argvExec[i] = strtok_r(NULL, " ", &p2);
            }

            if (argvExec[0] != NULL && strcmp(argvExec[0], "quit") == 0){
                return 0;
            }

            if (argvExec[0] != NULL) {
                execute(argvExec);
            }
            commande = strtok_r(NULL, ";", &p1);
        }
    }

    return 0;
}


void execute(char *argv[]){
  int pidfils;
  pidfils= fork();
  switch(pidfils){
    case -1: 
      perror("erreur de création du processus");
      break;
    case 0:
      execvp(argv[0], argv);
      perror("erreur d'execution de la commande");
      break;
    default:
      wait(NULL);
}

}





