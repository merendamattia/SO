# ???
## Connessione shell da MacOs 
1. Accedere alla VPN UniPR con FORTICLIENT ([guida installazione VPN](https://www.biblioteche.unipr.it/it/node/2366))
2. Aprire il Terminale e digitare la seguente riga di codice
```
ssh nome.cognome@studenti.unipr.it@didattica-linux.unipr.it
```
3. Inserire la password della propria email di ateneo



## Scrittura su macchina remota UNIPR da locale
1. Accedere alla VPN di ateneo
2. Aprire il Terminale e digitare la seguente riga di codice
```
scp [file locale] nome.cognome@studenti.unipr.it@didattica-linux.unipr.it:/home/[path directory]
```
3. Inserire la password della propria email di ateneo
