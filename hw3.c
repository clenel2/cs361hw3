

// execexample.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

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
     while (word) {
       // copy a word to the arg array
       if(strcmp(word, "|") == 0) {
         first = 0;
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

     // print out our array
     int j = 0;
     for (;j < i; j++){
       printf("argsarray1[%d]: %s\n", j, argsarray1[j]);
     }
     // exit shell program
     if(strcmp(argsarray1[0], "exit") == 0)
      break;

     // null terminate arguments
     argsarray1[i] = 0;
     argsarray2[k] = 0;
     int pid = fork();
     if (pid == 0) {
       //printf("I am the child!  I have pid %d. I'm gonna exec %s!\n", getpid(), argsarray1[0]);
       sprintf(line, "/bin/%s", argsarray1[0]);
       //printf("%s\n", line);
       execv(line, argsarray1);
       // should only be here if exec didn't work
       printf("pid %d did not exec\n", getpid());
       exit(1); // kill child
       }
       else {
       //printf("I am the parent.  I am waiting for my child %d to die.\n", pid);
       int status;
       wait(&status);
       //printf("My child has died with status %d.\n", WEXITSTATUS(status));
       printf("pid:%d status:%d\n", pid, WEXITSTATUS(status));
     }
     free(argsarray1[i]);
     free(argsarray2[k]);
   }
/*
    int i = 99;
    while(1){
      printf("%d bottles of beer on the wall\n", i);
      printf("%d bottles of beer\n", i);
      printf("Take one down, pass it around\n");
      i = i - 1;
      printf("%d bottles of beer on the wall\n", i);
    }
// execexample.c
  char *argsarray[] = {"/bin/ls", "-l", (char *)0};

  int pid = fork();
  if (pid == 0) {
    printf("Child with pid %d, about to exec ls\n", getpid());
    execv(argsarray[0], argsarray);
  } else {
    printf("I am the parent.  I am waiting for my child %d to die.\n", pid);
    int status;
    wait(&status);
    printf("My child has died with status %d. :(\n", WEXITSTATUS(status));
  }

// forkexample.c
  int pid = fork();
  if (pid == 0) {
    printf("I am the child!  I have pid %d.\n", getpid());
    exit(6);
  } else {
    printf("I am the parent.  I am waiting for my child %d to die.\n", pid);
    int status;
    wait(&status);
    printf("My child has died with status %d. :(\n", WEXITSTATUS(status));
  }
*/

/*
// ioredirect.c
  char buf1[20];
  char buf2[20];
  char buf3[20];
  char buf4[20];

  int fd1 = open("sampletext1.txt", O_RDONLY);
  int fd2 = open("sampletext2.txt", O_RDONLY);

  read(fd1, buf1, 19);
  buf1[19] = '\0';
  printf("%s\n", buf1);

  read(fd2, buf2, 19);
  buf2[19] = '\0';
  printf("%s\n", buf2);

  dup2(fd1, fd2);

  read(fd1, buf3, 19);
  buf3[19] = '\0';
  printf("%s\n", buf3);

  read(fd2, buf4, 19);
  buf4[19] = '\0';
  printf("%s\n", buf4);
  */
}
