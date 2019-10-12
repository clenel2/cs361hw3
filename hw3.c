

// execexample.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

void sigint_handler(int sig){
  char msg[] = "\ncaught sigint\n";
  write(1, msg, sizeof(msg));
  //exit(0);
}

void sigtstp_handler(int sig){
  char msg[] = "\ncaught sigstp\n";
  write(1, msg, sizeof(msg));
  //exit(0);
}

int main() {

// sighandle.c
  signal(SIGINT, sigint_handler);
  signal(SIGTSTP, sigtstp_handler);

  // stringparsing.c


  while(1) {
    //create some space for our strings
     char line[500];
     char *argsarray1[10];
     char *argsarray2[10];
     for(int j = 0; j < 10; j++) {
       argsarray1[j] = malloc(500*sizeof(char));
       argsarray2[j] = malloc(500*sizeof(char));
     }
     // print prompt
     printf("CS361 >");
     // read line from terminal
     fgets(line, 500, stdin);
     // get rid of endline
     size_t len = strlen(line);
     if(len > 0 && line [len-1] == '\n') {
       line[--len] = '\0';
     }
     // break the string up into words
     char *word = strtok(line, " ");
     int i = 0; // first argsarray counter
     int k = 0; // second argsarray counter
     int first = 1; // check if we should put the arg into the first or second argsarray
     int pipe = 0; // check if there was a '|'
     int semi = 0; // check if there was a ';'
     while (word) {
       // copy a word to the arg array
       if((strcmp(word, "|") == 0) && first == 1) {
         first = 0;
         pipe = 1;
       }
       else if((strcmp(word, ";") == 0) && first == 1) {
         first = 0;
         semi = 1;
       }
       else if(first == 1) {
         strcpy(argsarray1[i], word);
         i = i + 1;
       }
       else {
         strcpy(argsarray2[k], word);
         k = k + 1;
       }
       // get next word
       word = strtok(NULL, " ");
     }  // end adding to argsarrays

     // exit shell program
     if(strcmp(argsarray1[0], "exit") == 0)
      break;

     // null terminate arguments
     argsarray1[i] = 0;
     argsarray2[k] = 0;
     int pid = fork();
     int pid2 = -1;
     if(semi == 1) {
       pid2 = fork();
     }
     if(pipe == 1){}
     if (pid == 0) {
       execv(argsarray1[0], argsarray1);

       // should only be here if exec didn't work
       printf("pid %d did not exec\n", getpid());
       exit(1); // kill child
       }
       else if(pid2 == 0) {  // execute args after the semicolon
         execv(argsarray2[0], argsarray2);
         printf("pid %d did not exec\n", getpid());
         exit(1); // kill child
       }
       else {
       //printf("I am the parent.  I am waiting for my child %d to die.\n", pid);
       int status;
       wait(&status);
       //printf("My child has died with status %d.\n", WEXITSTATUS(status));
       printf("pid:%d status:%d\n", pid, WEXITSTATUS(status));
       if(semi == 1) { // wait for second command
         wait(&status);
         printf("pid:%d status:%d\n", pid, WEXITSTATUS(status));
       }
     }
     free(argsarray1[i]);
     free(argsarray2[k]);
   }
}
