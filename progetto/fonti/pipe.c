#include <stdio.h>
#include <unistd.h>

/*!
\mainpage Esempio Pipe
\section intro Introduzione
Esempio di creazione pipe per lanciare ls | sort\n

\date 02/05/2006
\version   0.0.0.1
\author    A.Dal Palu
*/


int main() {
  int pid;
  int fd[2];

  pipe(fd);

  if ((pid = fork()) == 0) { /* figlio */
    /* chiusura lettura da pipe */
    close(fd[0]);
    /* redirezione stdout a pipe */
    dup2(fd[1], 1);
    execlp("ls", "ls", NULL);
  }
  else if (pid > 0) { /* padre */
    /* chiusura scrittura su pipe */
    close(fd[1]);
    /* redirezione stdin a pipe */
    dup2(fd[0],0);
    execlp("sort", "sort", NULL);
  }
}

