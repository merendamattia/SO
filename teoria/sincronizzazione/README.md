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


### Problema dei lettori - scrittori
Si supponga che una base di dati sia da condividere tra numerosi processi concorrenti.  
Alcuni processi possono richiedere solo la lettura del contenuto della base dati (lettori), mentre altri ne possono richiedere un aggiornamento, vale a dire una lettura e una scrittura (scrittori).  
Se due lettori accedono nello stesso momento all’insieme di dati condiviso, non si ha alcun effetto negativo.  
Se uno scrittore e un altro processo (lettore o scrittore) accedono contemporaneamente alla stessa base di dati, ne può derivare il caos.  
Per impedire l’insorgere di difficoltà di questo tipo è necessario che gli scrittori abbiano un accesso esclusivo in fase di scrittura alla base di dati condivisa.  
Il problema dei lettori-scrittori ha diverse varianti, che implicano tutte l’esistenza di priorità.  
La più semplice richiede che nessun lettore attenda, a meno che uno scrittore abbia già ottenuto il permesso di usare l’insieme di dati condiviso.   
In altre parole, nessun lettore deve attendere che altri lettori terminino l’operazione solo perché uno scrittore attende l’accesso ai dati.  
Il secondo problema richiede che uno scrittore, una volta pronto, esegua il proprio compito di scrittura al più presto.  In altre parole, se uno scrittore attende l’accesso all’insieme di dati, nessun nuovo lettore deve iniziare la lettura.  
La soluzione del primo problema e quella del secondo possono condurre a uno stato d’attesa indefinita (starvation), degli scrittori, nel primo caso; dei lettori, nel secondo.

La soluzione del primo problema dei lettori - scrittori prevede dunque la condivisione da parte dei processi lettori delle seguenti strutture dati:

```c++
semaphore rw_mutex = 1; 
semaphore mutex = 1; 
int read_count = 0;
```

I semafori binari `mutex` e `rw_mutex` sono inizializzati a 1; `read_count` è inizializzato a 0.  
Il semaforo `rw_mutex` è comune a entrambi i tipi di processi (lettori e scrittori).  
Il semaforo `mutex` si usa per assicurare la mutua esclusione al momento dell’aggiornamento di `read_count`.  
La variabile `read_count` contiene il numero dei processi che stanno attualmente leggendo l’insieme di dati.  
Il semaforo `rw_mutex` funziona come semaforo di mutua esclusione per gli scrittori e serve anche al primo o all’ultimo lettore che entra o esce dalla sezione critica.  
Non serve, invece, ai lettori che entrano o escono mentre altri lettori si trovano nelle rispettive sezioni critiche.  

```c++
// Scrittore
while (true) {
    wait(rw_mutex);
    ...
    // esegui l’operazione di scrittura
    ...
    signal(rw_mutex); 
}
```

```c++
// Lettore
while (true) { 
    wait(mutex); 
    read_count++;
    if (read_count == 1) 
        wait(rw_mutex); 
    signal(mutex); 
    ...
    // esegui l’operazione di lettura  
    ...
    wait(mutex);
    read_count--;
    if (read_count == 0)   
        signal(rw_mutex); 
    signal(mutex);
}
```

Occorre notare che se uno scrittore si trova nella sezione critica e `n` lettori attendono di entrarvi, si accoda un lettore a `rw_mutex` e `n` – 1 lettori a `mutex`.  
Inoltre, se uno scrittore esegue `signal(rw_mutex)` si può riprendere l’esecuzione dei lettori in attesa, oppure di un singolo scrittore in attesa.  
La scelta è fatta dallo scheduler.

Le soluzioni al problema dei lettori-scrittori sono state generalizzate su alcuni sistemi in modo da fornire lock di lettura-scrittura.  
Per acquisire un tale lock è necessario specificarne la modalità, scrittura o lettura: se il processo desidera solo leggere i dati condivisi, richiede un lock di lettura-scrittura in modalità lettura; se invece desidera anche modificare i dati, lo richiede in modalità scrittura.  
È permesso a più processi di acquisire lock di lettura-scrittura in modalità lettura, ma solo un processo alla volta può avere il lock di lettura-scrittura in modalità scrittura, visto che nel caso della scrittura è necessario garantire l’accesso esclusivo.

I lock di lettura - scrittura sono utili soprattutto nelle situazioni seguenti:
- Nelle applicazioni in cui è facile identificare i processi che si limitano alla lettura di dati condivisi e quelli che si limitano alla scrittura di dati condivisi.
- Nelle applicazioni che prevedono più lettori che scrittori. Infatti, i lock di lettura- scrittura comportano in genere un carico di lavoro aggiuntivo rispetto ai semafori o ai lock mutex, compensato però dalla possibilità di eseguire molti lettori in concorrenza.


### Problema dei cinque filosofi (dining philosophers)
Si considerino cinque filosofi che trascorrono la loro esistenza pensando e mangiando.  
I filosofi condividono un tavolo rotondo circondato da cinque sedie, una per ciascun filosofo.  
Al centro del tavolo si trova una zuppiera colma di riso, e la tavola è apparecchiata con cinque bacchette

** FOTO TAVOLA ROTONDA **

Quando un filosofo pensa, non interagisce con i colleghi.  
Quando gli viene fame, tenta di prendere le bacchette più vicine: quelle che si trovano tra lui e i commensali alla sua destra e alla sua sinistra.  
Un filosofo può prendere una bacchetta alla volta e non può prendere una bacchetta che si trova già nelle mani di un suo vicino.  
Quando un filosofo affamato tiene in mano due bacchette contemporaneamente, mangia senza lasciare le bacchette.  
Terminato il pasto, le posa e riprende a pensare.

Il problema dei cinque filosofi è considerato un classico problema di sincronizzazione, non certo per la sua importanza pratica, e neanche per antipatia verso i filosofi da parte degli informatici, ma perché rappresenta una vasta classe di problemi di controllo della concorrenza, in particolare i problemi caratterizzati dalla necessità di assegnare varie risorse a diversi processi evitando situazioni di stallo e d’attesa indefinita.

#### Soluzione con uso di semafori
Una semplice soluzione consiste nel rappresentare ogni bacchetta con un semaforo: un filosofo tenta di afferrare ciascuna bacchetta eseguendo un’operazione `wait()` su quel semaforo e la posa eseguendo operazioni `signal()` sui semafori appropriati.  
Quindi, i dati condivisi sono
```c++
semaphore chopstick[5];
```
dove tutti gli elementi chopstick sono inizializzati a 1.

```c++
// Struttura del filosofo i
while (true) { 
    wait(chopstick[i]); 
    wait(chopstick[(i + 1) % 5]); 
    ...
    /* mangia */
    ...
    signal(chopstick[i]); 
    signal(chopstick[(i + 1) % 5]); 
    ...
    /* pensa */ 
    ...
}
```

Questa soluzione garantisce che due vicini non mangino contemporaneamente, ma è insufficiente poiché non esclude la possibilità che si abbia una situazione di stallo.  
Si supponga che tutti e cinque i filosofi abbiano fame contemporaneamente e che ciascuno afferri la bacchetta di sinistra.  
Tutti gli elementi di chopstick diventano uguali a zero, perciò ogni filosofo che tenta di afferrare la bacchetta di destra entra in stallo.  
Tali situazioni di stallo possono essere evitate con i seguenti espedienti:
- Solo quattro filosofi possono stare contemporaneamente a tavola;
- Un filosofo può prendere le sue bacchette solo se sono entrambe disponibili (quest’operazione si deve eseguire in una sezione critica);
- Si adotta una soluzione asimmetrica: un filosofo dispari prende prima la bacchetta di sinistra e poi quella di destra, invece un filosofo pari prende prima la bacchetta di destra e poi quella di sinistra.

Si noti tuttavia che qualsiasi soluzione soddisfacente per il problema dei cinque filosofi deve escludere la possibilità di situazioni d’attesa indefinita, in altre parole che uno dei filosofi muoia di fame (da qui il termine starvation) – una soluzione immune alle situazioni di stallo non esclude necessariamente la possibilità di situazioni d’attesa indefinita.

#### Soluzione con uso dei monitor
La soluzione impone il vincolo che un filosofo possa prendere le sue bacchette solo quando siano entrambe disponibili.  
Per codificare questa soluzione si devono distinguere i tre diversi stati in cui può trovarsi un filosofo.  
A tale scopo si introduce la seguente struttura dati:

```c++
enum {THINKING, HUNGRY, EATING} state[5];
```

Il filosofo `i` può impostare la variabile `state[i] = EATING` solo se i suoi due vicini non stanno mangiando:  

```c++
if((state[(i + 4) % 5] != EATING) && (state[(i + 1) % 5] != EATING))
    state[i] = EATING
```

Inoltre, occorre dichiarare la seguente struttura dati:

```c++
condition self[5];
```

che permette al filosofo i di ritardare se stesso quando ha fame, ma non riesce a ottenere le bacchette di cui ha bisogno.  

```c++
monitor DiningPhilosophers {
    enum {THINKING, HUNGRY, EATING} state[5]; 
    condition self[5];
    
    void pickup(int i) {
        state[i] = HUNGRY; 
        test(i);
        if (state[i] != EATING)
            self[i].wait();
    }
    
    void putdown(int i) {
        state[i] = THINKING; 
        test((i + 4) % 5); 
        test((i + 1) % 5);
    }
    
    void test(int i) {
        if ((state[(i + 4) % 5] != EATING) && (state[i] == HUNGRY) && (state[(i + 1) % 5] != EATING)) {
            state[i] = EATING; 
            self[i].signal();
        } 
    }
    
    Initialization_code() {
        for (int i = 0; i < 5; i++)
            state[i] = THINKING;
    } 
}
```

A questo punto si può descrivere la soluzione al problema dei cinque filosofi. La distribuzione delle bacchette è controllata dal monitor `DiningPhilosophers`.  
Ciascun filosofo, prima di cominciare a mangiare, deve invocare l’operazione `pickup()`; ciò può determinare la sospensione del processo filosofo.  
Completata con successo l’operazione, il filosofo può mangiare; in seguito, il filosofo invoca l’operazione `putdown()` e comincia a pensare.  

```c++
DiningPhilosophers.pickup(i); 
...
eat
... 
DiningPhilosophers.putdown(i);
```

È facile dimostrare che questa soluzione assicura che due vicini non mangino contemporaneamente e che non si verifichino situazioni di stallo.  
Occorre però notare che un filosofo può attendere indefinitamente.  
La soluzione di questo problema è lasciata come esercizio per il lettore.


## Sincronizzazione all'interno del Kernel













