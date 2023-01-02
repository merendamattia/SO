# Progetto: Processes vs Threads
Realizzato da [Merenda Saverio Mattia](https://www.merendamattia.com) [MAT: 330503] 

---

Sono presenti due versioni, una Linux e una MacOs, ognuna oresente nella rispettiva cartella.

---
## Come compilare il programma (su Linux)
```
gcc -o main main.c -pthread
```

## Come compilare il programma (su MacOs)
```
make main
```

## Elenco delle funzioni (che non conoscevo) che ho utilizzato
| Metodi  | Descrizione |
| ------------- | ------------- |
| `int sem_init()`  | Viene utilizzata per inizializzare un semaforo.  Passiamo tre parametri: il primo è un puntatore a una struttura `sem_t` che rappresenta il semaforo da inizializzare; il secondo specifica se il semaforo è condiviso tra processi o solo all'interno di un singolo processo; e infine il terzo è il valore iniziale del semaforo.  La funzione restituisce `0` in caso di successo, oppure un valore negativo in caso di errore. (deprecato in macOs) |
| `sem_t sem_open()`  | È l'equivalente di `sem_init` in macOs. La funzione `sem_open` accetta come argomenti il nome del semaforo, un set di flag per il controllo dell'apertura del semaforo e i permessi di accesso al semaforo. Se il semaforo non esiste, viene creato con i permessi specificati e il valore iniziale specificato. Se il semaforo esiste già, viene aperto e utilizzato come indicato. Restituisce un puntatore a una variabile di tipo `sem_t` in caso di successo, mentre restituisce `SEM_FAILED` in caso di errore. |
| `void sem_close()`  | È l'equivalente di `sem_destroy` in macOs. Con questa funzione il semaforo viene chiuso. Passo come parametro il puntatore al semaforo. |
| `void sem_unlink()`  | Il semaforo viene rimosso dal sistema. Passo come parametro il puntatore al semaforo. |
| `clock_t clock()`  | Restituisce il numero di clock del processore utilizzati dal programma dal momento in cui è stato avviato. Il valore restituito è espresso in clock del processore e dipende dalla velocità del processore. Viene utilizzata per misurare il tempo di esecuzione di un programma o di una porzione di codice. Tuttavia, poiché il valore restituito dipende dalla velocità del processore, non è una misura precisa del tempo trascorso e può variare su diversi sistemi. |
| `void exit()`  | Viene utilizzata per terminare un processo. Termina solo il processo corrente e non gli altri processi creati dal metodo `fork`.  Passiamo un parametro: è un codice di uscita che indica lo stato del processo al momento della terminazione. Questo codice può essere utilizzato da altri processi per determinare se il processo è stato terminato correttamente o meno. |
| `pid_t wait()`  | Viene utilizzata per sospendere l'esecuzione di un processo fino a quando uno dei suoi figli non viene terminato.  Passiamo un parametro: è un puntatore a una variabile intera in cui viene memorizzato lo stato di uscita del processo figlio terminato. La funzione restituisce l'identificatore del processo figlio terminato, oppure `-1` in caso di errore. |
| `int pipe()`  | Viene utilizzata per creare una pipe. Una pipe è costituita da due file descriptor, uno per la scrittura e uno per la lettura, che possono essere utilizzati dai processi per inviare e ricevere i dati. La funzione `pipe()` prende come parametro un array di due interi e restituisce `0` in caso di successo o `-1` in caso di errore. Gli interi contenuti nell'array sono i file descriptor della pipe, il primo per la scrittura e il secondo per la lettura. |
| `int read()`  | Viene utilizzata per leggere dati da un file descriptor, come ad esempio una pipe. Prende come parametri il file descriptor da cui leggere, un buffer in cui scrivere i dati letti e il numero di byte da leggere. Restituisce il `numero di byte` effettivamente letti o `-1` in caso di errore. La funzione `read()` è bloccante, ovvero il programma viene sospeso fino a quando non vengono letti i dati dal file descriptor o si verifica un errore.  |
| `int write()`  | Viene utilizzata per scrivere dati su un file descriptor, come ad esempio una pipe. La funzione `write()` prende come parametri il file descriptor su cui scrivere, un puntatore ai dati da scrivere e la quantità di dati da scrivere, espresso in `byte`. La funzione `write()` restituisce il numero di byte scritti, che può essere inferiore alla quantità di dati richiesta in caso di errore. In caso di errore, la funzione restituisce `-1`.  |
| `key_t ftok()`  | Viene utilizzata per creare una chiave di sincronizzazione per l'accesso a una area di memoria condivisa.  Il primo parametro passato è il percorso del file da utilizzare per generare la chiave e il secondo è un identificatore di progetto. |
| `void* shmat()`  | Consente a un processo di accedere ai dati presenti in una area di memoria condivisa, rendendoli disponibili per la lettura e la scrittura.  Passiamo tre parametri: il primo è l'identificatore dell'area di memoria condivisa da collegare; il secondo è l'indirizzo in cui collegare l'area di memoria condivisa; e infine il terzo è una maschera di flag che determina le modalità di accesso all'area di memoria condivisa.  La funzione restituisce l'indirizzo del segmento di memoria del processo dove è stato collegato l'area di memoria condivisa |
| `int shmdt()`  | Consente a un processo di interrompere l'accesso ai dati presenti in una area di memoria condivisa, rendendoli non più disponibili per la lettura e la scrittura.  Passiamo solo un parametro: è l'indirizzo del segmento di memoria del processo dove è collegata l'area di memoria condivisa da scollegare.  La funzione restituisce `0` in caso di successo, oppure un valore negativo in caso di errore.  |
