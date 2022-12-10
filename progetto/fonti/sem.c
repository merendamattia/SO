#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>

/*!
\mainpage Esempio Semafori
\section intro Introduzione
Esempio di utilizzo semafori

\date 02/05/2006
\version   0.0.0.1
\author    A.Dal Palu
*/


#define DOWN (-1)
#define UP (1)
#define MYCODE 'a'

// Definizione della struttura semun
#ifdef _SEM_SEMUN_UNDEFINED
#define _SEM_SEMUN_UNDEFINED  1
union semun /* definita in bits/sem.h */
{
  int val;                              /* value for SETVAL */
  struct semid_ds *buf;                 /* buffer for IPC_STAT & IPC_SET */
  unsigned short int *array;            /* array for GETALL & SETALL */
  struct seminfo *__buf;                /* buffer for IPC_INFO */
};
#endif



/************************  sem_up() ***********************/
/*!
    \brief  Dello zucchero sintattico per fare la UP
    \param  ipcid Riceve il numero dell`oggetto IPC
    \param  nsem Il semaforo su cui fare UP
    
    Questa chiamata maschera i dettagli implementativi sul buffer delle operazioni IPC
*/
void sem_up(int ipcid, int nsem)
{
    struct sembuf sops[1];
    sops[0].sem_num = nsem;
    sops[0].sem_op = UP;
    sops[0].sem_flg = 0;
    if (semop(ipcid, sops, 1) == -1) {
      perror("Errore in semop");
      exit(3);
    }
}

/************************  sem_up() ***********************/
/*!
    \brief  Dello zucchero sintattico per fare la DOWN
    \param  ipcid Riceve il numero dell`oggetto IPC
    \param  nsem Il semaforo su cui fare DOWN
    
    Questa chiamata maschera i dettagli implementativi sul buffer delle operazioni IPC
*/void sem_down(int ipcid, int nsem)
{
    struct sembuf sops[1];
    sops[0].sem_num = nsem;
    sops[0].sem_op = DOWN;
    sops[0].sem_flg = 0;
    if (semop(ipcid, sops, 1) == -1) {
      perror("Errore in semop");
      exit(3);
    }
}

/************************  sem_set() ***********************/
/*!
    \brief  Dello zucchero sintattico per impostare il valore di un semaforo
    \param  ipcid Riceve il numero dell`oggetto IPC
    \param  nsem Il semaforo da impostare
    \param  initial Il valore iniziale del semaforo
    
    Questa chiamata maschera i dettagli implementativi sul buffer delle operazioni IPC
*/
void sem_set(int ipcid, int nsem, int initial)
{
  union semun arg;
  arg.val = initial;
  if (semctl(ipcid, nsem, SETVAL, arg) == -1) {
    perror("set value sem");
    exit(2);
  }
}

/************************  padre() ***********************/
/*!
    \brief  Codice di prova per il padre
    \param  ipcid Riceve il numero dell`oggetto IPC
    
    Il Padre fa Ping e fa UP sul semaforo del figlio
*/
void padre(int ipcid)
{
  int i;
  for (i=0;i<10;i++){
    sem_down(ipcid,0);
    printf("Ping\n");
    sem_up(ipcid,1);
  }
}

/************************  figlio() ***********************/
/*!
    \brief  Codice di prova per il figlio
    \param  ipcid Riceve il numero dell`oggetto IPC
    
    Il figlio fa Pong e fa UP sul semaforo del padre
*/
void figlio(int ipcid)
{
  int i;
  for (i=0;i<10;i++){
    sem_down(ipcid,1);
    printf("Pong\n");
    sem_up(ipcid,0);
  }
}


int main()
{
  key_t key;
  int npro, sid, val, pid;
  char s[60];
  union semun arg;
  struct sembuf sops[2];

  if ((key = ftok(".", MYCODE)) == -1) { /* ottieni chiave IPC
					       da pathname (".") e 
					       codice (MYCODE) */
    perror("ftok");
    exit(1);
  }
  if ((sid = semget(key, 2, 0600 | IPC_CREAT )) == -1) {
    perror("creazione 2 semafori");
    exit(1);
  }

  sem_set(sid,0,1);  /// inizializza il semaforo 0 a 1
  sem_set(sid,1,0);  /// inizializza il semaforo 1 a 0

  switch (pid = fork()) {
  case -1:
    perror("fork");
    exit(2);
  case 0: /* Figlio: eredita indice del semaforo */
    printf("Figlio\n");
    figlio(sid);
    printf("Figlio: mi sono sganciato, termino\n");
    exit(0);
  default: /*Padre: osserva e sgancia il semaforo */
    printf("Padre\n");
    padre(sid);
    printf("Padre: attendo terminazione figlio \n");
    wait(&val);
    if (semctl(sid, 0, IPC_RMID, 0) == -1)
      perror("rimozione sem");
    printf("Padre: finito\n");
  }
}







