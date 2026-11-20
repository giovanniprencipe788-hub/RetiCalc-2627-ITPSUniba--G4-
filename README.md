# Consegna TCP - Reti di Calcolatori (a.a 2026-27)

## Informazioni generali
- Gruppo: G4
- Sistema operativo: Windows
- Ambiente di sviluppo: Eclipse CDT

## Struttura dei progetti
- serverTCP_G4: progetto Eclipse del server TCP( resta in ascolto, accetta connessioni, stampa l'IP del client, invia "connessione avvenuta", riceve n1, n2, n3, calcola n1+n2-n3, stampa "la somma di n1+n2-n3=res" e invia "QUIT" se il risultato è 100, altrimenti il risultato).
- clientTCP_G4: progetto Eclipse del client TCP( legge IP e porta da tastiera, si connette, stampa "connessione avvenuta", invia tre interi n1, n2, n3, stampa la risposta del server e termina chiudendo la connessione se riceve "QUIT"). 

## Compilazione ed esecuzione
1. Compilare entrambi i progetti collegando la libreria ws2_32.
2. Avviare prima il server (serverTCP_G4).
3. Avviare il client(clientTCP_G4) e inserire da tastiera l'indirizzo IP e la porta del server. 
4. Inserire tre numeri interi alla volta fino a quando il calcolo n1 + n2 - n3 restituisce 100 e il server invia "QUIT". 
