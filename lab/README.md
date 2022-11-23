# Comandi
Guida avanzata di [scripting Bash](http://www.pluto.it/files/ildp/guide/abs/index.html) (consigliata dal prof. Bertini).

## Lista di comandi visti a lezione
| Command           					| Description |
| ---               					| --- |
| clear             					| pulizia interfaccia |
| man [comando]     					| manuale shell |
| man -wa [comando] 					| trova dove sta il comando nel manuale |
| who               					| visualizza gli utenti interattivi connessi al sistema |
| uname -a          					| mosta informazioni sul sistema |
| ps                					| visualizza i processi attivi dell’utente |
| ps -f             					| visualizza tutti i processi |
| ps ef             					| visualizza le bash attive |
| man passwd        					| permette di cambiare la password utente |
| history           					| storia di tutti i comandi eseguiti |
| ![numero riga]    					| riesegue comando a quella riga della history |
| rm [nome file]    					| elimina file |
| cd /area_comune/docenti/bertini_so 	| connessione area comune prof Bertini |
| cp [dir source] [dir destination] 	| copia file e lo incolla nella nuova directory |
| cat [nome file]   					| apre il file |
| nano [nome file].[estensione] 		| crea file e apre editor di testo |
| make [nome file] 						| compila file e crea file oggetto (testato con C e C++) |
| ./[nome file oggetto] 				| esegue programma |
| pushd [directory] 					| salva nello stack l'indirizzo della directory |
| popd 									| estrae dallo stack l'indirizzo della directory |
| pwd 									| ritorna il percorso della directory in cui ci troviamo |
| rm -rf [directory] 					| rimozione forzata di una cartella |
| touch [nome file].[estensione] 		| crea file vuoto |
| ln -s [nome file] [nome link] 		| creo link simbolico (soft link) (collegamento) |
| ls -li 								| visualizzazione dettagliata con high number |
| ln [nome file] [nome link]			| duplicazione file (se ne modifico uno si modifica anche l'altro) (copia che si aggiorna) (hard link) (li puoi creare solo sullo stesso file system) |
| chmod 666 [nome file] 				| cambio permessi a un file (666: do' a tutti pemesso di scrittura) |
| chmod u=wx, g=x, o=w [nome file] 		| user: write/exec, group: exec, other: write (maggiori informazioni slide 26 di 'Lab_Lezione_1.pdf') |
| chown [nome user destinatario] [nome file] | cambio proprietà file |
| scp [file locale] [host:path] 			| Copia un file da macchina locale a macchina remota |
| scp [host:path_file] [directory locale] 	| Copia un file da macchina remota a macchina locale |
| alias [nome alias]='[istruzione]' 		| creo alias |
| unalias [nome alias] 						| elimino alias |
| alias 									| restituisce tutti gli alias esistenti |
| cd ~user 									| equivale a scrivere cd /home/user |
| nano ~/.bashrc 							| modificare file configurazione bash |
| [nome variabile]='[stringa]' 				| creo una variabile, visibile solo nella bash locale |
| echo/cat $[nome variabile] 				| visualizzare contenuto variabile |
| set 										| mostra tutte le variabili dichiarate in shell |
| export [nome variabile] 					| per visualizzare una variabile in tutte le bash |
| bash 										| creo nuova bash |
| printev 									| mostra tutte le variabili presenti in tutte le bash |
| which [nome alias/comando] 				| cerca il path completo o l'alias di un comando |
| type [nome alias] 						| ti dice il contenuto dell'alias |
| file [nome file] 							| informazioni generali del file |
| tar -cf [nome archivio].tar [nomi file] 	| crea archivio |
| tar -tf [nome archivio].tar 				| mostra contenuto archivio |
| tar -xf [nome archivio].tar 				| estrae un archivio |
| gzip [nome file] 							| comprime un file creando un file [nome file].gz |
| gunzip [nome file].gz 					| unzippa il file |
| tar -czf [nome archivio].tar [nomi file] 	| crea archivio e lo comprime con gzip |
| ls [dir] > [nome file] 					| ridireziono l’output su file |
| wc -l 									| conta numero di righe |
| wc -l < [nome file] 						| ridirezione l'input, stampa quante righe ci sono in un file |
|  |  |
|  |  |
