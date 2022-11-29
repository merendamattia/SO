# Esempi di Sincronizzazione (Cap. 7)
Indice
1.  Classici problemi di Sincronizzazione
    - Produttore - consumatore con memoria limitata
    - Problema dei lettori - scrittori
    - Problema dei cinque filosofi
        - Soluzione con uso dei semafori
        - Soluzione con uso dei monitor
2. Sincronizzazione all'interno del Kernel
    - Windows
    - Linux
3. Sincronizzazione Posix
    - Lock mutex Posix
    - Semafori Posix
        - Semafori named
        - Semafori unnamed
    - Variabili condizionali Posix
    
##  Classici problemi di Sincronizzazione
Questi problemi sono utili per verificare quasi tutte le nuove proposte di schemi di sincronizzazione.

### Produttore - consumatore con memoria limitata
Nel nostro problema produttore e consumatore condividono le seguenti strutture dati:

```c++
int n;
semaphore mutex = 1; 
semaphore empty = n; 
semaphore full = 0;
```

Si supponga di disporre di una certa quantità di memoria rappresentata da un buffer con n posizioni, ciascuna capace di contenere un elemento.  
Il semaforo `mutex` garantisce la mutua esclusione degli accessi al buffer ed è inizializzato al valore 1.  
I semafori `full` ed `empty` conteggiano rispettivamente il numero di posizioni vuote e il numero di posizioni piene nel buffer.  
Il semaforo `empty` si inizializza al valore `n`; il semaforo `full` si inizializza al valore 0.  

```c++
// Produttore
while (true) {
  ...
  // produci un elemento in next_produced 
  ...
  wait(empty);
  wait(mutex);
  ...
  // inserisci next_produced in buffer
  ...
  signal(mutex);
  signal(full);
}
```

```c++
// Consumatore
while (true) { 
  wait(full); 
  wait(mutex); 
  ...
  // rimuovi un elemento da buffer e mettilo in next_consumed 
  ...
  signal(mutex);
  signal(empty);
  ...
  // consuma l’elemento contenuto in next_consumed  
  ...
}
```

È interessante notare la simmetria esistente tra il produttore e il consumatore.   
Il codice si può interpretare nel senso di produzione, da parte del produttore, di posizioni piene per il consumatore; oppure di produzione, 
da parte del consumatore, di posizioni vuote per il produttore.
